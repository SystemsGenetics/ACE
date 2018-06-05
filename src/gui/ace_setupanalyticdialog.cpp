#include "ace_setupanalyticdialog.h"
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
#include <../core/ace_analytic_abstractmanager.h>
#include <../core/ace_settings.h>
#include <../core/eabstractdatafactory.h>
#include <../core/eabstractanalyticfactory.h>
#include <../core/eexception.h>
#include "ace_mainwindow.h"



using namespace Ace;
//






/*!
 *
 * @param manager  
 *
 * @param type  
 */
SetupAnalyticDialog::SetupAnalyticDialog(Analytic::AbstractManager* manager, quint16 type):
   _manager(manager),
   _type(type)
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());
   setLayout(layout);
   setWindowTitle(tr("Execute %1").arg(EAbstractAnalyticFactory::instance().name(_type)));
}






/*!
 *
 * @param index  
 */
void SetupAnalyticDialog::findFile(int index)
{
   QFileDialog dialog(nullptr,tr("Select File"));
   switch (_manager->type(index))
   {
   case EAbstractAnalytic::Input::FileIn:
      dialog.setAcceptMode(QFileDialog::AcceptOpen);
      break;
   case EAbstractAnalytic::Input::FileOut:
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      break;
   default:
      return;
   }
   dialog.setNameFilters(_manager->data(index,EAbstractAnalytic::Input::FileFilters).toStringList());
   QString path;
   if ( dialog.exec() )
   {
      QStringList files = dialog.selectedFiles();
      path = QDir(QDir::currentPath()).relativeFilePath(files.at(0));
   }
   qobject_cast<QLineEdit*>(_edits.at(index))->setText(path);
}






/*!
 *
 * @param index  
 */
void SetupAnalyticDialog::findDataObject(int index)
{
   QFileDialog dialog(nullptr,tr("Select File"));
   switch (_manager->type(index))
   {
   case EAbstractAnalytic::Input::DataIn:
      dialog.setAcceptMode(QFileDialog::AcceptOpen);
      break;
   case EAbstractAnalytic::Input::DataOut:
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      break;
   default:
      return;
   }
   quint16 type = _manager->data(index,EAbstractAnalytic::Input::DataType).toUInt();
   EAbstractDataFactory& factory = EAbstractDataFactory::instance();
   if ( type >= factory.size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Setup Analytic Dialog"));
      e.setDetails(tr("Analytic requesting data type %1 that does not exist(%2 is max).")
                   .arg(type).arg(factory.size()-1));
      throw e;
   }
   dialog.setNameFilter(tr("%1 data object (*.%2)")
                        .arg(factory.name(type))
                        .arg(factory.fileExtension(type)));
   QString path;
   if ( dialog.exec() )
   {
      QStringList files = dialog.selectedFiles();
      path = QDir(QDir::currentPath()).relativeFilePath(files.at(0));
   }
   qobject_cast<QLineEdit*>(_edits.at(index))->setText(path);
}






/*!
 */
void SetupAnalyticDialog::executeTriggered()
{
   QString command {EAbstractAnalyticFactory::instance().commandName(_type)};
   for (int i = 0; i < _manager->size() ;++i)
   {
      QString bit(" --");
      bit.append(_manager->data(i,EAbstractAnalytic::Input::CommandLineName).toString())
         .append(" ");
      switch (_manager->type(i))
      {
      case EAbstractAnalytic::Input::Type::Boolean:
         bit.append(processBool(i));
         break;
      case EAbstractAnalytic::Input::Type::Integer:
         bit.append(processInteger(i));
         break;
      case EAbstractAnalytic::Input::Type::Double:
         bit.append(processDouble(i));
         break;
      case EAbstractAnalytic::Input::Type::Selection:
         bit.append(processSelection(i));
         break;
      case EAbstractAnalytic::Input::Type::String:
      case EAbstractAnalytic::Input::Type::FileIn:
      case EAbstractAnalytic::Input::Type::FileOut:
      case EAbstractAnalytic::Input::Type::DataIn:
      case EAbstractAnalytic::Input::Type::DataOut:
         bit.append(processString(i));
         break;
      }
      command.append(bit);
   }
   MainWindow::instance().addCommand(command);
   accept();
}






/*!
 *
 * @param index  
 */
QString SetupAnalyticDialog::processBool(quint16 index)
{
   QCheckBox* edit = qobject_cast<QCheckBox*>(_edits.at(index));
   _manager->set(index,edit->isChecked());
   return edit->isChecked() ? "TRUE" : "FALSE";
}






/*!
 *
 * @param index  
 */
QString SetupAnalyticDialog::processInteger(quint16 index)
{
   QSpinBox* edit = qobject_cast<QSpinBox*>(_edits.at(index));
   _manager->set(index,edit->value());
   return QString::number(edit->value());
}






/*!
 *
 * @param index  
 */
QString SetupAnalyticDialog::processDouble(quint16 index)
{
   QLineEdit* edit = qobject_cast<QLineEdit*>(_edits.at(index));
   _manager->set(index,edit->text().toDouble());
   return edit->text();
}






/*!
 *
 * @param index  
 */
QString SetupAnalyticDialog::processString(quint16 index)
{
   QLineEdit* edit = qobject_cast<QLineEdit*>(_edits.at(index));
   _manager->set(index,edit->text());
   return QString("\"").append(edit->text().replace('"',"\\\"")).append("\"");
}






/*!
 *
 * @param index  
 */
QString SetupAnalyticDialog::processSelection(quint16 index)
{
   QComboBox* edit = qobject_cast<QComboBox*>(_edits.at(index));
   _manager->set(index,edit->currentText());
   return QString("\"").append(edit->currentText().replace('"',"\\\"")).append("\"");
}






/*!
 */
QLayout* SetupAnalyticDialog::createForm()
{
   QFormLayout* ret {new QFormLayout};
   for (int i = 0; i < _manager->size() ;++i)
   {
      QLabel* label = new QLabel(_manager->data(i,EAbstractAnalytic::Input::Title).toString());
      label->setWhatsThis(_manager->data(i,EAbstractAnalytic::Input::WhatsThis).toString());
      QWidget* field;
      switch (_manager->type(i))
      {
      case EAbstractAnalytic::Input::Type::Boolean:
         field = createBool(i);
         break;
      case EAbstractAnalytic::Input::Type::Integer:
         field = createInteger(i);
         break;
      case EAbstractAnalytic::Input::Type::Double:
         field = createDouble(i);
         break;
      case EAbstractAnalytic::Input::Type::String:
         field = createString(i);
         break;
      case EAbstractAnalytic::Input::Type::Selection:
         field = createSelection(i);
         break;
      case EAbstractAnalytic::Input::Type::FileIn:
      case EAbstractAnalytic::Input::Type::FileOut:
      case EAbstractAnalytic::Input::Type::DataIn:
      case EAbstractAnalytic::Input::Type::DataOut:
         field = createFile(i);
         break;
      }
      ret->addRow(label,field);
   }
   return ret;
}






/*!
 */
QLayout* SetupAnalyticDialog::createButtons()
{
   QPushButton* execute {new QPushButton(tr("&Execute"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(execute,&QPushButton::clicked,this,&SetupAnalyticDialog::executeTriggered);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(execute);
   ret->addStretch();
   ret->addWidget(cancel);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createBool(quint16 index)
{
   QCheckBox* ret = new QCheckBox;
   ret->setChecked(_manager->data(index,EAbstractAnalytic::Input::Default).toBool());
   _edits.append(ret);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createInteger(quint16 index)
{
   QSpinBox* ret = new QSpinBox;
   ret->setMinimum(_manager->data(index,EAbstractAnalytic::Input::Minimum).toInt());
   ret->setMaximum(_manager->data(index,EAbstractAnalytic::Input::Maximum).toInt());
   ret->setValue(_manager->data(index,EAbstractAnalytic::Input::Default).toInt());
   _edits.append(ret);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createDouble(quint16 index)
{
   QLineEdit* ret = new QLineEdit;
   ret->setText(QString::number(_manager->data(index,EAbstractAnalytic::Input::Default).toDouble()));
   double bottom {_manager->data(index,EAbstractAnalytic::Input::Minimum).toDouble()};
   double top {_manager->data(index,EAbstractAnalytic::Input::Maximum).toDouble()};
   int decimals {_manager->data(index,EAbstractAnalytic::Input::Decimals).toInt()};
   ret->setValidator(new QDoubleValidator(bottom,top,decimals,this));
   _edits.append(ret);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createString(quint16 index)
{
   QLineEdit* ret = new QLineEdit;
   ret->setText(_manager->data(index,EAbstractAnalytic::Input::Default).toString());
   _edits.append(ret);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createSelection(quint16 index)
{
   QComboBox* ret = new QComboBox;
   const QStringList options
   {
      _manager->data(index,EAbstractAnalytic::Input::SelectionValues).toStringList()
   };
   for (auto option: options)
   {
      ret->addItem(option);
   }
   ret->setCurrentIndex(ret->findText(_manager->data(index,EAbstractAnalytic::Input::Default).toString()));
   _edits.append(ret);
   return ret;
}






/*!
 *
 * @param index  
 */
QWidget* SetupAnalyticDialog::createFile(quint16 index)
{
   QPushButton* browse {new QPushButton(tr("Browse"))};
   switch (_manager->type(index))
   {
   case EAbstractAnalytic::Input::Type::FileIn:
   case EAbstractAnalytic::Input::Type::FileOut:
      connect(browse,&QPushButton::clicked,[this,index]{ findFile(index); });
      break;
   case EAbstractAnalytic::Input::Type::DataIn:
   case EAbstractAnalytic::Input::Type::DataOut:
      connect(browse,&QPushButton::clicked,[this,index]{ findDataObject(index); });
      break;
   default:
      break;
   }
   QLineEdit* edit = new QLineEdit;
   edit->setReadOnly(true);
   _edits.append(edit);
   QHBoxLayout* layout {new QHBoxLayout};
   layout->addWidget(edit);
   layout->addWidget(browse);
   QWidget* ret {new QWidget};
   ret->setLayout(layout);
   return ret;
}
