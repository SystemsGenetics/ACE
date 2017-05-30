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

#include "setupanalyticdialog.h"
#include "abstractanalytic.h"
#include "abstractdatafactory.h"






Ace::SetupAnalyticDialog::SetupAnalyticDialog(EAbstractAnalytic* analytic, QWidget* parent):
   QDialog(parent),
   _analytic(analytic)
{
   setLayout(createInputs());
}






Ace::SetupAnalyticDialog::~SetupAnalyticDialog()
{
}






void Ace::SetupAnalyticDialog::findFile(int argument)
{
   QFileDialog dialog(nullptr,tr("Select File"));
   EAbstractDataFactory& factory = EAbstractDataFactory::getInstance();
   using Type = EAbstractAnalytic::ArgumentType;
   switch (_analytic->getArgumentType(argument))
   {
   case Type::FileIn:
      dialog.setAcceptMode(QFileDialog::AcceptOpen);
      dialog.setNameFilter(_analytic->getFileArgumentFilters(argument));
      break;
   case Type::FileOut:
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog.setNameFilter(_analytic->getFileArgumentFilters(argument));
      break;
   case Type::DataIn:
      {
         dialog.setAcceptMode(QFileDialog::AcceptOpen);
         quint16 type = _analytic->getDataArgumentType(argument);
         QString filter = tr("%1 data object (*.%2)").arg(factory.getName(type))
               .arg(factory.getFileExtension(type));
         dialog.setNameFilter(filter);
      }
      break;
   case Type::DataOut:
      {
         dialog.setAcceptMode(QFileDialog::AcceptSave);
         quint16 type = _analytic->getDataArgumentType(argument);
         QString filter = tr("%1 data object (*.%2)").arg(factory.getName(type))
               .arg(factory.getFileExtension(type));
         dialog.setNameFilter(filter);
      }
      break;
   default:
      return;
   }
   if ( dialog.exec() )
   {
      QDir pwd(QDir::currentPath());
      QStringList files = dialog.selectedFiles();
      QLineEdit* edit = qobject_cast<QLineEdit*>(_inputs.at(argument));
      edit->setText(pwd.relativeFilePath(files.at(0)));
   }
}






void Ace::SetupAnalyticDialog::cancel()
{
}






void Ace::SetupAnalyticDialog::executeButton()
{
}






// TODO; use hasAcceptableInput() and add bool isArgumentRequired(int argument);
// then add signals for that with the execute button so you can only execute when it is
// doable :d
QFormLayout* Ace::SetupAnalyticDialog::createInputs()
{
   using Type = EAbstractAnalytic::ArgumentType;
   QSignalMapper* mapper = new QSignalMapper(this);
   connect(mapper,SIGNAL(mapped(int)),this,SLOT(findFile(int)));
   QFormLayout* formLayout = new QFormLayout;
   for (int i = 0; i < _analytic->getArgumentCount() ;++i)
   {
      QLabel* label = new QLabel(_analytic->getArgumentTitle(i));
      label->setWhatsThis(_analytic->getArgumentWhatsThis(i));
      QWidget* field;
      switch (_analytic->getArgumentType(i))
      {
      case Type::Bool:
         _inputs.append(new QCheckBox);
         field = _inputs.back();
         break;
      break;
      case Type::Integer:
         {
            QLineEdit* edit = new QLineEdit;
            edit->setValidator(new QIntValidator(this));
            _inputs.append(edit);
            field = edit;
         }
         break;
      case Type::Float:
      case Type::Double:
         {
            QLineEdit* edit = new QLineEdit;
            edit->setValidator(new QDoubleValidator(this));
            _inputs.append(edit);
            field = edit;
         }
         break;
      case Type::String:
         _inputs.append(new QLineEdit);
         field = _inputs.back();
         break;
      case Type::Combo:
         {
            QComboBox* combo = new QComboBox;
            QStringList options = _analytic->getComboValues(i);
            for (auto i = options.constBegin(); i != options.constEnd() ;++i)
            {
               combo->addItem(*i);
            }
            _inputs.append(combo);
            field = combo;
         }
         break;
      case Type::FileIn:
      case Type::FileOut:
      case Type::DataIn:
      case Type::DataOut:
         {
            field = new QWidget;
            QHBoxLayout* fieldLayout = new QHBoxLayout;
            _inputs.append(new QLineEdit);
            fieldLayout->addWidget(_inputs.back());
            QPushButton* findFileButton = new QPushButton(tr("Browse"));
            fieldLayout->addWidget(findFileButton);
            connect(findFileButton,SIGNAL(clicked(bool)),mapper,SLOT(map()));
            field->setLayout(fieldLayout);
            mapper->setMapping(findFileButton,i);
         }
         break;
      }
      formLayout->addRow(label,field);
   }
   return formLayout;
}
