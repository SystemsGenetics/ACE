#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSignalMapper>
#include <QFormLayout>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QSpinBox>

#include "setupanalyticdialog.h"
#include "abstractanalytic.h"
#include "abstractdatafactory.h"
#include "exception.h"
#include "mainwindow.h"






Ace::SetupAnalyticDialog::SetupAnalyticDialog(EAbstractAnalytic* analytic, QWidget* parent):
   QDialog(parent),
   _analytic(analytic)
{
   // create buttons and connect signals
   _cancelButton = new QPushButton(tr("&Cancel"),this);
   _executeButton = new QPushButton(tr("&Execute"),this);
   connect(_cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancel()));
   connect(_executeButton,SIGNAL(clicked(bool)),this,SLOT(executeButton()));

   // create main layout and add form layout
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addLayout(createInputs());

   // create buttons layout and add to main layout
   QHBoxLayout* buttonsLayout = new QHBoxLayout;
   buttonsLayout->addWidget(_executeButton);
   buttonsLayout->addStretch();
   buttonsLayout->addWidget(_cancelButton);
   layout->addLayout(buttonsLayout);

   // set main layout to widget
   setLayout(layout);
}






void Ace::SetupAnalyticDialog::findFile(int argument)
{
   // use type and role enumerations
   using Type = EAbstractAnalytic::ArgumentType;
   using Role = EAbstractAnalytic::Role;

   // make file dialog and get data factory
   QFileDialog dialog(nullptr,tr("Select File"));
   EAbstractDataFactory& factory = EAbstractDataFactory::getInstance();

   // set file dialog accept mode depending on argument type
   switch (_analytic->getArgumentData(argument))
   {
   case Type::FileIn:
   case Type::DataIn:
      dialog.setAcceptMode(QFileDialog::AcceptOpen);
      break;
   case Type::FileOut:
   case Type::DataOut:
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      break;
   default:
      return;
   }

   // setup file filters for file dialog depending on argument type
   switch (_analytic->getArgumentData(argument))
   {
   case Type::FileIn:
   case Type::FileOut:
      dialog.setNameFilters(_analytic->getArgumentData(argument,Role::FileFilters).toStringList());
      break;
   case Type::DataIn:
   case Type::DataOut:
   {
      // get data type and make sure it is valid
      quint16 type = _analytic->getArgumentData(argument,Role::DataType).toUInt();
      if ( type >= factory.getCount() )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setTitle(tr("Setup Analytic Dialog"));
         e.setDetails(tr("Analytic requesting data type %1 that does not exist(%2 is max).")
                      .arg(type).arg(factory.getCount()-1));
         throw e;
      }

      // set file filter of dialog for data type
      QString filter = tr("%1 data object (*.%2)").arg(factory.getName(type))
            .arg(factory.getFileExtension(type));
      dialog.setNameFilter(filter);
      break;
   }
   default:
      return;
   }

   // execute file dialog
   if ( dialog.exec() )
   {
      // if dialog exits with success, set argument to selected file name
      QDir pwd(QDir::currentPath());
      QStringList files = dialog.selectedFiles();
      QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(argument));
      edit->setText(pwd.relativeFilePath(files.at(0)));
   }
}






void Ace::SetupAnalyticDialog::cancel()
{
   reject();
}






void Ace::SetupAnalyticDialog::executeButton()
{
   // use type and role enumerations
   using Type = EAbstractAnalytic::ArgumentType;
   using Role = EAbstractAnalytic::Role;

   // initialize console command and iterate through all arguments
   QString command = _analytic->getCommandName();
   for (int i = 0; i < _analytic->getArgumentCount() ;++i)
   {
      // add beginning of command line argument
      QString bit(" --");
      bit.append(_analytic->getArgumentData(i,Role::CommandLineName).toString());
      bit.append(" ");

      // set argument value to analytic and add to command line argument dependong on argument type
      switch (_analytic->getArgumentData(i))
      {
      case Type::Bool:
      {
         QCheckBox* edit = qobject_cast<QCheckBox*>(_inputs.at(i));
         _analytic->setArgument(i,QVariant(edit->isChecked()));
         edit->isChecked() ? bit.append("TRUE") : bit.append("FALSE");
         break;
      }
      case Type::Integer:
      {
         QSpinBox* edit = qobject_cast<QSpinBox*>(_inputs.at(i));
         _analytic->setArgument(i,QVariant(edit->value()));
         bit.append(QString::number(edit->value()));
         break;
      }
      case Type::Double:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->setArgument(i,QVariant(edit->text().toDouble()));
         bit.append(edit->text());
         break;
      }
      case Type::String:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->setArgument(i,QVariant(edit->text()));
         bit.append(QString("\"%1\"").arg(edit->text().replace('"',"\\\"")));
         break;
      }
      case Type::Combo:
      {
         QComboBox* edit = qobject_cast<QComboBox*>(_inputs.at(i));
         _analytic->setArgument(i,QVariant(edit->currentText()));
         bit.append(QString("\"%1\"").arg(edit->currentText().replace('"',"\\\"")));
         break;
      }
      case Type::FileIn:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->addFileIn(i,edit->text());
         bit.append(QString("\"%1\"").arg(edit->text().replace('"',"\\\"")));
         break;
      }
      case Type::FileOut:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->addFileOut(i,edit->text());
         bit.append(QString("\"%1\"").arg(edit->text().replace('"',"\\\"")));
         break;
      }
      case Type::DataIn:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->addDataIn(i,edit->text(),_analytic->getArgumentData(i,Role::DataType).toUInt());
         bit.append(QString("\"%1\"").arg(edit->text().replace('"',"\\\"")));
         break;
      }
      case Type::DataOut:
      {
         QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(i));
         _analytic->addDataOut(i,edit->text()
                               ,_analytic->getArgumentData(i,Role::DataType).toUInt());
         bit.append(QString("\"%1\"").arg(edit->text().replace('"',"\\\"")));
         break;
      }
      }

      // add command line argument to command string
      command.append(bit);
   }

   // add string command to main window and set for analytic
   MainWindow::getInstance().addCommand(command);
   _analytic->setCommand(command);

   // signal successful completion
   accept();
}






QFormLayout* Ace::SetupAnalyticDialog::createInputs()
{
   // use type and role enumerations
   using Type = EAbstractAnalytic::ArgumentType;
   using Role = EAbstractAnalytic::Role;

   // create signal mapper for file dialog arguments and form layout
   QSignalMapper* mapper = new QSignalMapper(this);
   connect(mapper,SIGNAL(mapped(int)),this,SLOT(findFile(int)));
   QFormLayout* formLayout = new QFormLayout;

   // iterate through all arguments
   for (int i = 0; i < _analytic->getArgumentCount() ;++i)
   {
      // create label for argument
      QLabel* label = new QLabel(_analytic->getArgumentData(i,Role::Title).toString());
      label->setWhatsThis(_analytic->getArgumentData(i,Role::WhatsThis).toString());

      // create field for argument
      QWidget* field;
      switch (_analytic->getArgumentData(i))
      {
      case Type::Bool:
      {
         // create new checkbox field for bool argument
         QCheckBox* edit = new QCheckBox;
         edit->setChecked(_analytic->getArgumentData(i,Role::DefaultValue).toBool());
         _inputs.append(edit);
         field = edit;
         break;
      }
      break;
      case Type::Integer:
      {
         // create new spin box field for integer argument, getting min and max value
         QSpinBox* edit = new QSpinBox;
         edit->setValue(_analytic->getArgumentData(i,Role::DefaultValue).toInt());
         edit->setMinimum(_analytic->getArgumentData(i,Role::Minimum).toInt());
         edit->setMaximum(_analytic->getArgumentData(i,Role::Maximum).toInt());
         _inputs.append(edit);
         field = edit;
         break;
      }
      case Type::Double:
      {
         // create new line edit field for double argument, setting double validator with min and
         // max value
         QLineEdit* edit = new QLineEdit;
         edit->setText(QString::number(
                          _analytic->getArgumentData(i,Role::DefaultValue).toDouble()));
         double bottom {_analytic->getArgumentData(i,Role::Minimum).toDouble()};
         double top {_analytic->getArgumentData(i,Role::Maximum).toDouble()};
         int decimals {_analytic->getArgumentData(i,Role::Decimals).toInt()};
         edit->setValidator(new QDoubleValidator(bottom,top,decimals,this));
         _inputs.append(edit);
         field = edit;
         break;
      }
      case Type::String:
      {
         // create new line edit field for string argument
         QLineEdit* edit = new QLineEdit;
         edit->setText(_analytic->getArgumentData(i,Role::DefaultValue).toString());
         _inputs.append(edit);
         field = edit;
         break;
      }
      case Type::Combo:
      {
         // create new combo box field for combo argument, adding all possible items to combo
         QComboBox* combo = new QComboBox;
         QStringList options = _analytic->getArgumentData(i,Role::ComboValues).toStringList();
         for (auto i = options.constBegin(); i != options.constEnd() ;++i)
         {
            combo->addItem(*i);
         }
         _inputs.append(combo);
         field = combo;
         break;
      }
      case Type::FileIn:
      case Type::FileOut:
      case Type::DataIn:
      case Type::DataOut:
      {
         // create new line edit and browse button field for file/data argument, adding the browse
         // button clicked signal to the signal mapper and setting the mapping to its argument
         // number
         field = new QWidget;
         QHBoxLayout* fieldLayout = new QHBoxLayout;
         QLineEdit* edit = new QLineEdit;
         edit->setReadOnly(true);
         _inputs.append(edit);
         fieldLayout->addWidget(_inputs.back());
         QPushButton* findFileButton = new QPushButton(tr("Browse"));
         fieldLayout->addWidget(findFileButton);
         connect(findFileButton,SIGNAL(clicked(bool)),mapper,SLOT(map()));
         field->setLayout(fieldLayout);
         mapper->setMapping(findFileButton,i);
         break;
      }
      }

      // add new argument row to form layout
      formLayout->addRow(label,field);
   }

   // return completed form layout
   return formLayout;
}
