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
#include "../core/ace_analytic_abstractmanager.h"
#include "../core/ace_settings.h"
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "ace_mainwindow.h"



using namespace Ace;
//






/*!
 * Constructs a new setup analytic dialog with the given analytic manager and its 
 * analytic type. The type must be given so this new object can lookup the command 
 * line name of the analytic it is running. 
 *
 * @param manager The analytic manager whose input will be displayed to the user in 
 *                this new dialog and set to the form values if executed. 
 */
SetupAnalyticDialog::SetupAnalyticDialog(Analytic::AbstractManager* manager):
   _manager(manager)
{
   // Create a new vertical layout _layout_, adding the form layout and then the 
   // buttons layout. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());

   // Set this object's layout to _layout_ and set its window title using the 
   // analytic name. 
   setLayout(layout);
   setWindowTitle(tr("Execute %1").arg(EAbstractAnalyticFactory::instance().name(_manager->analyticType())));
}






/*!
 * Called when the browse button for a file in or out argument is clicked. This 
 * opens a qt file dialog to find the file, getting the file filters from the 
 * argument index. If the user selects a correct file then the text edit value is 
 * set to its relative path. 
 *
 * @param index The index of the edit widget and analytic argument that is being 
 *              called to change its relative path value. 
 */
void SetupAnalyticDialog::findFile(int index)
{
   // Create a new qt file dialog _dialog_, setting it to accepting an open or save 
   // depending on if the analytic argument is file in or out. 
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

   // Set the name filters for _dialog_ to the file filters for the given analytic 
   // argument and then execute _dialog_ in modal. If execution returns failure then 
   // return. 
   dialog.setNameFilters(_manager->data(index,EAbstractAnalytic::Input::FileFilters).toStringList());
   if ( !dialog.exec() )
   {
      return;
   }

   // Get the first path from the selected files in _dialog_, then alter it to its 
   // relative path in relation to the current directory, and then set that relative 
   // path to the value of the line edit widget for this argument. 
   QStringList files = dialog.selectedFiles();
   QString path {QDir(QDir::currentPath()).relativeFilePath(files.at(0))};
   qobject_cast<QLineEdit*>(_edits.at(index))->setText(path);
}






/*!
 * Called when the browse button for a data in or out argument is clicked. This 
 * opens a qt file dialog to find the data object file, getting the data type from 
 * the argument index. If the user selects a correct data object file then the text 
 * edit value is set to its relative path. 
 *
 * @param index The index of the edit widget and analytic argument that is being 
 *              called to change its relative path value. 
 */
void SetupAnalyticDialog::findDataObject(int index)
{
   // Create a new qt file dialog _dialog_, setting it to accepting an open or save 
   // depending on if the analytic argument is data in or out. 
   QFileDialog dialog(nullptr,tr("Select Data Object File"));
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

   // Get the data object type for this analytic argument. If the type returned if 
   // out of range then throw an exception. 
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

   // Set the name filters for _dialog_ to the file filters for the given analytic 
   // argument's data object type and then execute _dialog_ in modal. If execution 
   // returns failure then return. 
   dialog.setNameFilter(tr("%1 data object (*.%2)")
                        .arg(factory.name(type))
                        .arg(factory.fileExtension(type)));
   if ( !dialog.exec() )
   {
      return;
   }

   // Get the first path from the selected files in _dialog_, then alter it to its 
   // relative path in relation to the current directory, and then set that relative 
   // path to the value of the line edit widget for this argument. 
   QStringList files = dialog.selectedFiles();
   QString path {QDir(QDir::currentPath()).relativeFilePath(files.at(0))};
   qobject_cast<QLineEdit*>(_edits.at(index))->setText(path);
}






/*!
 * Called when the execute button is clicked. This applies all arguments to this 
 * object's analytic manager, then adding the command line version to the main 
 * window, and then closing on success. 
 */
void SetupAnalyticDialog::executeClicked()
{
   // Create a new string _command_ that is the beginning of the command line version 
   // of this analytic execution. 
   QString command {EAbstractAnalyticFactory::instance().commandName(_manager->analyticType())};

   // Iterate through all arguments for this object's analytic manager, setting each 
   // one with the value of its edit widget along with appending it to the string 
   // _command_ for its command line version option. 
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

   // Add the command line version of this analytic run to the main window and close 
   // with dialog with success. 
   MainWindow::instance().addCommand(command);
   accept();
   close();
}






/*!
 * Called when the cancel button is clicked. This rejects the dialog and closes it. 
 */
void SetupAnalyticDialog::cancelClicked()
{
   // .
   reject();
   close();
}






/*!
 * Processes a boolean edit widget with the given index, setting the edit widget 
 * value to the corresponding analytic manager argument and returning the command 
 * line option version. 
 *
 * @param index The index of the analytic manager argument that is set with its 
 *              corresponding edit widget. 
 *
 * @return Command line version for setting the analytic argument with the given 
 *         index. 
 */
QString SetupAnalyticDialog::processBool(quint16 index)
{
   // Set the value of the analytic argument with the given index to the value of the 
   // corresponding qt check box edit widget and return the command line version. 
   QCheckBox* edit = qobject_cast<QCheckBox*>(_edits.at(index));
   _manager->set(index,edit->isChecked());
   return edit->isChecked() ? "TRUE" : "FALSE";
}






/*!
 * Processes a integer edit widget with the given index, setting the edit widget 
 * value to the corresponding analytic manager argument and returning the command 
 * line option version. 
 *
 * @param index The index of the analytic manager argument that is set with its 
 *              corresponding edit widget. 
 *
 * @return Command line version for setting the analytic argument with the given 
 *         index. 
 */
QString SetupAnalyticDialog::processInteger(quint16 index)
{
   // Set the value of the analytic argument with the given index to the value of the 
   // corresponding qt check box edit widget and return the command line version. 
   QSpinBox* edit = qobject_cast<QSpinBox*>(_edits.at(index));
   _manager->set(index,edit->value());
   return QString::number(edit->value());
}






/*!
 * Processes a double edit widget with the given index, setting the edit widget 
 * value to the corresponding analytic manager argument and returning the command 
 * line option version. 
 *
 * @param index The index of the analytic manager argument that is set with its 
 *              corresponding edit widget. 
 *
 * @return Command line version for setting the analytic argument with the given 
 *         index. 
 */
QString SetupAnalyticDialog::processDouble(quint16 index)
{
   // Set the value of the analytic argument with the given index to the value of the 
   // corresponding qt check box edit widget and return the command line version. 
   QLineEdit* edit = qobject_cast<QLineEdit*>(_edits.at(index));
   _manager->set(index,edit->text().toDouble());
   return edit->text();
}






/*!
 * Processes a string edit widget with the given index, setting the edit widget 
 * value to the corresponding analytic manager argument and returning the command 
 * line option version. 
 *
 * @param index The index of the analytic manager argument that is set with its 
 *              corresponding edit widget. 
 *
 * @return Command line version for setting the analytic argument with the given 
 *         index. 
 */
QString SetupAnalyticDialog::processString(quint16 index)
{
   // Set the value of the analytic argument with the given index to the value of the 
   // corresponding qt check box edit widget and return the command line version. 
   QLineEdit* edit = qobject_cast<QLineEdit*>(_edits.at(index));
   _manager->set(index,edit->text());
   return QString("\"").append(edit->text().replace('"',"\\\"")).append("\"");
}






/*!
 * Processes selection edit widget with the given index, setting the edit widget 
 * value to the corresponding analytic manager argument and returning the command 
 * line option version. 
 *
 * @param index The index of the analytic manager argument that is set with its 
 *              corresponding edit widget. 
 *
 * @return Command line version for setting the analytic argument with the given 
 *         index. 
 */
QString SetupAnalyticDialog::processSelection(quint16 index)
{
   // Set the value of the analytic argument with the given index to the value of the 
   // corresponding qt check box edit widget and return the command line version. 
   QComboBox* edit = qobject_cast<QComboBox*>(_edits.at(index));
   _manager->set(index,edit->currentText());
   return QString("\"").append(edit->currentText().replace('"',"\\\"")).append("\"");
}






/*!
 * Creates all GUI edit widgets contained in a qt form layout for all arguments of 
 * this new object's analytic manager, returning the layout. 
 *
 * @return Qt form layout containing all edit widgets for this new object's 
 *         analytic manager. 
 */
QLayout* SetupAnalyticDialog::createForm()
{
   // Create a new form layout _ret_. 
   QFormLayout* ret {new QFormLayout};

   // Iterate through all arguments for this object's analytic manager, adding its 
   // corresponding edit widget to this object's list of edit widgets and the layout 
   // _ret_ and setting its default value. 
   for (int i = 0; i < _manager->size() ;++i)
   {
      QLabel* label = new QLabel(_manager->data(i,EAbstractAnalytic::Input::Title).toString());
      label->setWhatsThis(_manager->data(i,EAbstractAnalytic::Input::WhatsThis).toString());
      QWidget* field {nullptr};
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

   // Return _ret_. 
   return ret;
}






/*!
 * Creates and initializes the execute and cancel buttons for this new dialog, 
 * returning a layout of the buttons. 
 *
 * @return Layout contains the buttons for this new dialog. 
 */
QLayout* SetupAnalyticDialog::createButtons()
{
   // Create the execute and cancel buttons for this new dialog, connecting their 
   // clicked signals. 
   QPushButton* execute {new QPushButton(tr("&Execute"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(execute,&QPushButton::clicked,this,&SetupAnalyticDialog::executeClicked);
   connect(cancel,&QPushButton::clicked,this,&SetupAnalyticDialog::cancelClicked);

   // Create a new horizontal layout _ret_, adding the execute button then a stretch 
   // boundary then the cancel button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(execute);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return _ret_. 
   return ret;
}






/*!
 * Creates a new qt check box edit widget for the analytic manager argument with 
 * the given index, appending it to the list of edit widgets. The given index must 
 * match where the edit widget is added to the list of edit widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to the newly created edit widget. 
 */
QWidget* SetupAnalyticDialog::createBool(quint16 index)
{
   // Create a new qt check box _ret_, settings its state to the default value of the 
   // analytic manager argument with the given index. 
   QCheckBox* ret = new QCheckBox;
   ret->setChecked(_manager->data(index,EAbstractAnalytic::Input::Default).toBool());

   // Append _ret_ to this object's list of edit widgets and return it. 
   _edits.append(ret);
   return ret;
}






/*!
 * Creates a new qt spin box edit widget for the analytic manager argument with the 
 * given index, appending it to the list of edit widgets. The given index must 
 * match where the edit widget is added to the list of edit widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to the newly created edit widget. 
 */
QWidget* SetupAnalyticDialog::createInteger(quint16 index)
{
   // Create a new qt spin box _ret_, setting its minimum, maximum, and default 
   // values from interrogating the analytic manager argument with the given index. 
   QSpinBox* ret = new QSpinBox;
   ret->setMinimum(_manager->data(index,EAbstractAnalytic::Input::Minimum).toInt());
   ret->setMaximum(_manager->data(index,EAbstractAnalytic::Input::Maximum).toInt());
   ret->setValue(_manager->data(index,EAbstractAnalytic::Input::Default).toInt());

   // Append _ret_ to this object's list of edit widgets and return it. 
   _edits.append(ret);
   return ret;
}






/*!
 * Creates a new qt line edit widget with a double validator for the analytic 
 * manager argument with the given index, appending it to the list of edit widgets. 
 * The given index must match where the edit widget is added to the list of edit 
 * widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to the newly created edit widget. 
 */
QWidget* SetupAnalyticDialog::createDouble(quint16 index)
{
   // Create a new qt line edit _ret_ with a qt double validator, setting its 
   // default, bottom, top, and decimals values from interrogating the analytic 
   // manager argument with the given index. 
   QLineEdit* ret = new QLineEdit;
   ret->setText(QString::number(_manager->data(index,EAbstractAnalytic::Input::Default).toDouble()));
   double bottom {_manager->data(index,EAbstractAnalytic::Input::Minimum).toDouble()};
   double top {_manager->data(index,EAbstractAnalytic::Input::Maximum).toDouble()};
   int decimals {1000};
   QVariant value {_manager->data(index,EAbstractAnalytic::Input::Decimals)};
   if ( !value.isNull() )
   {
      decimals = value.toInt();
   }
   ret->setValidator(new QDoubleValidator(bottom,top,decimals,this));

   // Append _ret_ to this object's list of edit widgets and return it. 
   _edits.append(ret);
   return ret;
}






/*!
 * Creates a new qt line edit widget for the analytic manager argument with the 
 * given index, appending it to the list of edit widgets. The given index must 
 * match where the edit widget is added to the list of edit widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to the newly created edit widget. 
 */
QWidget* SetupAnalyticDialog::createString(quint16 index)
{
   // Create a new qt line edit _ret_, setting its default value from interrogating 
   // the analytic manager argument with the given index. 
   QLineEdit* ret = new QLineEdit;
   ret->setText(_manager->data(index,EAbstractAnalytic::Input::Default).toString());

   // Append _ret_ to this object's list of edit widgets and return it. 
   _edits.append(ret);
   return ret;
}






/*!
 * Creates a new qt combo box edit widget for the analytic manager argument with 
 * the given index, appending it to the list of edit widgets. The given index must 
 * match where the edit widget is added to the list of edit widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to the newly created edit widget. 
 */
QWidget* SetupAnalyticDialog::createSelection(quint16 index)
{
   // Create a new qt combo box _ret_, appending all options and setting its default 
   // value from interrogating the analytic manager argument with the given index. 
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

   // Append _ret_ to this object's list of edit widgets and return it. 
   _edits.append(ret);
   return ret;
}






/*!
 * Creates a new file selection edit widget for the analytic manager argument with 
 * the given index, appending it to the list of edit widgets. The given index must 
 * match where the edit widget is added to the list of edit widgets. The file 
 * selection edit widget is a grouping of base qt edit widgets, namely a read only 
 * line edit and browse button. The line edit is added to the list of edit widgets. 
 *
 * @param index Analytic manager argument index which a new edit widget is created 
 *              for. 
 *
 * @return Pointer to a newly created widget containing the group of GUI elements 
 *         that make up the entire file selection edit widget. 
 */
QWidget* SetupAnalyticDialog::createFile(quint16 index)
{
   // Create a new qt push button _browse_, connecting its clicked signal to the 
   // correct slot based off it being a file or data object. 
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

   // Create a new line edit widget _edit_, setting it to read only and appending it 
   // to this object's list of edit widgets. 
   QLineEdit* edit = new QLineEdit;
   edit->setReadOnly(true);
   _edits.append(edit);

   // Create a new horizontal layout _layout_, adding _edit_ then _browse_ widgets. 
   QHBoxLayout* layout {new QHBoxLayout};
   layout->addWidget(edit);
   layout->addWidget(browse);

   // Create a new qt widget, setting its layout to _layout_ and then returning its 
   // pointer. 
   QWidget* ret {new QWidget};
   ret->setLayout(layout);
   return ret;
}
