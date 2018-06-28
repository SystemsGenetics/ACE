#include "ace_settingsdialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include "../core/ace_settings.h"
#include "../core/opencl_platform.h"
#include "../core/opencl_device.h"



using namespace Ace;
//






/*!
 * Constructs a new settings dialog. 
 */
SettingsDialog::SettingsDialog()
{
   // Create a new layout, adding the form and buttons of this dialog.
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());

   // Set the layout for this dialog.
   setLayout(layout);
}






/*!
 * Called when the OK button is clicked, applying changes and closing the dialog. 
 */
void SettingsDialog::okClicked()
{
   // Apply any changes and close the dialog.
   applyClicked();
   close();
}






/*!
 * Called when the apply button is clicked, applying any changes made to ACE 
 * settings. 
 */
void SettingsDialog::applyClicked()
{
   // Get a reference of ACE global settings.
   Ace::Settings& settings {Ace::Settings::instance()};

   // Check if the selected OpenCL platform is the special "none".
   if ( _platformCombo->currentIndex() == OpenCL::Platform::size() )
   {
      // Set the ACE OpenCL device to none.
      settings.setOpenCLPlatform(-1);
      settings.setOpenCLDevice(0);
   }

   // Else a valid OpenCL device is selected.
   else
   {
      // Set the ACE OpenCL device to the user selection.
      settings.setOpenCLPlatform(_platformCombo->currentIndex());
      settings.setOpenCLDevice(_deviceCombo->currentIndex());
   }

   // Set the thread and buffer size settings.
   settings.setThreadSize(_threadEdit->value());
   settings.setBufferSize(_bufferEdit->value());

   // Set all chunk settings.
   settings.setChunkDir(_chunkDirEdit->text());
   settings.setChunkPrefix(_chunkPrefixEdit->text());
   settings.setChunkExtension(_chunkExtensionEdit->text());
}






/*!
 * Called when this object's OpenCL platform combo box has changed its selection to 
 * the given index. This updates the list of devices to select. 
 *
 * @param index The new index this object's OpenCL platform combo box has changed 
 *              to. 
 */
void SettingsDialog::currentPlatformChanged(int index)
{
   // Clear any previous devices.
   _deviceCombo->clear();

   // If the index is out of range then return.
   if ( index < 0 || index >= OpenCL::Platform::size() )
   {
      return;
   }

   // Build the list of available devices for the OpenCL platform with the given index.
   for (int i = 0; i < OpenCL::Platform::get(index)->deviceSize() ;++i)
   {
      _deviceCombo->addItem(OpenCL::Platform::get(index)->device(i)->name());
   }
}






/*!
 * Creates and initializes the form for this new settings dialog, returning the 
 * form layout. 
 *
 * @return The form layout containing all edit widgets for this dialog. 
 */
QLayout* SettingsDialog::createForm()
{
   // Get a reference of ACE global settings.
   Ace::Settings& settings {Ace::Settings::instance()};

   // Create and initialize the thread size spin box widget.
   _threadEdit = new QSpinBox;
   _threadEdit->setMinimum(1);
   _threadEdit->setMaximum(INT_MAX);
   _threadEdit->setValue(settings.threadSize());

   // Create and initialize the buffer size spin box widget.
   _bufferEdit = new QSpinBox;
   _bufferEdit->setMinimum(1);
   _bufferEdit->setMaximum(INT_MAX);
   _bufferEdit->setValue(settings.bufferSize());

   // Create and initialize all chunk settings with line edit widgets.
   _chunkDirEdit = new QLineEdit;
   _chunkDirEdit->setText(settings.chunkDir());
   _chunkPrefixEdit = new QLineEdit;
   _chunkPrefixEdit->setText(settings.chunkPrefix());
   _chunkExtensionEdit = new QLineEdit;
   _chunkExtensionEdit->setText(settings.chunkExtension());

   // Create a new form layout and add all setting edit widgets to it with corresponding
   // labels.
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("OpenCL Device:")),createOpenCL());
   ret->addRow(new QLabel(tr("Thread Size:")),_threadEdit);
   ret->addRow(new QLabel(tr("Buffer Size:")),_bufferEdit);
   ret->addRow(new QLabel(tr("Chunk Working Directory:")),_chunkDirEdit);
   ret->addRow(new QLabel(tr("Chunk Prefix:")),_chunkPrefixEdit);
   ret->addRow(new QLabel(tr("Chunk Extension:")),_chunkExtensionEdit);

   // Return the form layout.
   return ret;
}






/*!
 * Creates and initializes the OK, apply, and cancel buttons for this new settings 
 * dialog, returning a layout containing them. 
 *
 * @return The layout containing all the buttons for this dialog. 
 */
QLayout* SettingsDialog::createButtons()
{
   // Create all buttons, connecting their clicked signals to the appropriate slots.
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* apply {new QPushButton(tr("Apply"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&SettingsDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&SettingsDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::close);

   // Create a new horizontal layout, adding the created buttons with a stretch between the
   // OK/apply buttons and the cancel button.
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return the layout.
   return ret;
}






/*!
 * Creates and initializes the OpenCL combo boxes for this new settings dialog, 
 * returning the layout containing them. The layout consists of two combo boxes, 
 * one for the platform and the other for the device. 
 *
 * @return The layout containing the OpenCL selection combo boxes. 
 */
QLayout* SettingsDialog::createOpenCL()
{
   // Get a reference of ACE global settings.
   Ace::Settings& settings {Ace::Settings::instance()};

   // Create the platform combo box and add all available platforms to it including the
   // special none selection.
   _platformCombo = new QComboBox;
   for (int i = 0; i < OpenCL::Platform::size() ;++i)
   {
      _platformCombo->addItem(OpenCL::Platform::get(i)->name());
   }
   _platformCombo->addItem(tr("none"));

   // Create the device combo box.
   _deviceCombo = new QComboBox;

   // Check if an OpenCL device is selected in ACE settings.
   if ( settings.openCLDevicePointer() )
   {
      // Set the platform combo index, update the device combo box, and then set the device
      // combo index.
      _platformCombo->setCurrentIndex(settings.openCLPlatform());
      currentPlatformChanged(settings.openCLPlatform());
      _deviceCombo->setCurrentIndex(settings.openCLDevice());
   }

   // Else there is no OpenCL device in ACE settings to set the platform to none.
   else
   {
      _platformCombo->setCurrentIndex(OpenCL::Platform::size());
   }

   // Connect the current index changed signal for the platform combo box.
   connect(_platformCombo
           ,QOverload<int>::of(&QComboBox::currentIndexChanged)
           ,this
           ,&SettingsDialog::currentPlatformChanged);

   // Create a new layout, add both combo boxes, and return it.
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_platformCombo);
   ret->addWidget(_deviceCombo);
   return ret;
}
