#include "ace_settingsdialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include "../core/ace_settings.h"
#include "../core/cuda_device.h"
#include "../core/opencl_platform.h"
#include "../core/opencl_device.h"



using namespace Ace;
//






/*!
 * Constructs a new settings dialog. 
 */
SettingsDialog::SettingsDialog()
{
   // .
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupWarning());
   layout->addSpacing(16);
   layout->addLayout(createForm());
   layout->addSpacing(16);
   layout->addLayout(createButtons());

   // .
   setLayout(layout);
}






/*!
 * Called when the OK button is clicked, applying changes and closing the dialog. 
 */
void SettingsDialog::okClicked()
{
   // .
   applyClicked();
   close();
}






/*!
 * Called when the apply button is clicked, applying any changes made to ACE 
 * settings. 
 */
void SettingsDialog::applyClicked()
{
   // .
   Ace::Settings& settings {Ace::Settings::instance()};

   // .
   if ( _cudaDeviceCombo->currentIndex() == CUDA::Device::size() )
   {
      // .
      settings.setCUDADevice(-1);
   }

   // .
   else
   {
      // .
      settings.setCUDADevice(_cudaDeviceCombo->currentIndex());
   }

   // .
   if ( _openclPlatformCombo->currentIndex() == OpenCL::Platform::size() )
   {
      // .
      settings.setOpenCLPlatform(-1);
      settings.setOpenCLDevice(0);
   }

   // .
   else
   {
      // .
      settings.setOpenCLPlatform(_openclPlatformCombo->currentIndex());
      settings.setOpenCLDevice(_openclDeviceCombo->currentIndex());
   }

   // .
   settings.setThreadSize(_threadEdit->value());
   settings.setBufferSize(_bufferEdit->value());

   // .
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
void SettingsDialog::currentOpenCLPlatformChanged(int index)
{
   // .
   _openclDeviceCombo->clear();

   // .
   if ( index < 0 || index >= OpenCL::Platform::size() )
   {
      return;
   }

   // .
   for (int i = 0; i < OpenCL::Platform::get(index)->deviceSize() ;++i)
   {
      _openclDeviceCombo->addItem(OpenCL::Platform::get(index)->device(i)->name());
   }
}






/*!
 */
QLayout* SettingsDialog::setupWarning()
{
   QHBoxLayout* ret {new QHBoxLayout};
   QLabel* label {new QLabel};
   label->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(64));
   label->setAlignment(Qt::AlignRight|Qt::AlignTop);
   ret->addWidget(label);
   QLabel* warning {new QLabel(tr("<h3>WARNING</h3><p>All settings in this dialog are PERSISTENT and carry over to any other use of this application in both GUI or command line mode!"))};
   warning->setWordWrap(true);
   ret->addWidget(warning);
   return ret;
}






/*!
 * Creates and initializes the form for this new settings dialog, returning the 
 * form layout. 
 *
 * @return The form layout containing all edit widgets for this dialog. 
 */
QLayout* SettingsDialog::createForm()
{
   // .
   Ace::Settings& settings {Ace::Settings::instance()};

   // .
   _threadEdit = new QSpinBox;
   _threadEdit->setMinimum(1);
   _threadEdit->setMaximum(INT_MAX);
   _threadEdit->setValue(settings.threadSize());

   // .
   _bufferEdit = new QSpinBox;
   _bufferEdit->setMinimum(1);
   _bufferEdit->setMaximum(INT_MAX);
   _bufferEdit->setValue(settings.bufferSize());

   // .
   _chunkDirEdit = new QLineEdit;
   _chunkDirEdit->setText(settings.chunkDir());
   _chunkPrefixEdit = new QLineEdit;
   _chunkPrefixEdit->setText(settings.chunkPrefix());
   _chunkExtensionEdit = new QLineEdit;
   _chunkExtensionEdit->setText(settings.chunkExtension());

   // .
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("CUDA Device:")),createCUDA());
   ret->addRow(new QLabel(tr("OpenCL Device:")),createOpenCL());
   ret->addRow(new QLabel(tr("Thread Size:")),_threadEdit);
   ret->addRow(new QLabel(tr("Buffer Size:")),_bufferEdit);
   ret->addRow(new QLabel(tr("Chunk Working Directory:")),_chunkDirEdit);
   ret->addRow(new QLabel(tr("Chunk Prefix:")),_chunkPrefixEdit);
   ret->addRow(new QLabel(tr("Chunk Extension:")),_chunkExtensionEdit);

   // .
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
   // .
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* apply {new QPushButton(tr("Apply"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&SettingsDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&SettingsDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::close);

   // .
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // .
   return ret;
}






/*!
 * Creates and initializes the CUDA combo box for this new settings dialog,
 * returning the layout containing them. The layout consists of one combo box
 * for the device.
 *
 * @return The layout containing the CUDA selection combo box.
 */
QLayout* SettingsDialog::createCUDA()
{
   // .
   Ace::Settings& settings {Ace::Settings::instance()};

   // .
   _cudaDeviceCombo = new QComboBox;
   for (int i = 0; i < CUDA::Device::size() ;++i)
   {
      _cudaDeviceCombo->addItem(CUDA::Device::get(i)->name());
   }
   _cudaDeviceCombo->addItem(tr("none"));

   // .
   if ( settings.cudaDevicePointer() )
   {
      // .
      _cudaDeviceCombo->setCurrentIndex(settings.cudaDevice());
   }

   // .
   else
   {
      // .
      _cudaDeviceCombo->setCurrentIndex(CUDA::Device::size());
   }

   // .
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_cudaDeviceCombo);
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
   // .
   Ace::Settings& settings {Ace::Settings::instance()};

   // .
   _openclPlatformCombo = new QComboBox;
   for (int i = 0; i < OpenCL::Platform::size() ;++i)
   {
      _openclPlatformCombo->addItem(OpenCL::Platform::get(i)->name());
   }
   _openclPlatformCombo->addItem(tr("none"));

   // .
   _openclDeviceCombo = new QComboBox;

   // .
   if ( settings.openCLDevicePointer() )
   {
      // .
      _openclPlatformCombo->setCurrentIndex(settings.openCLPlatform());
      currentOpenCLPlatformChanged(settings.openCLPlatform());
      _openclDeviceCombo->setCurrentIndex(settings.openCLDevice());
   }

   // .
   else
   {
      _openclPlatformCombo->setCurrentIndex(OpenCL::Platform::size());
   }

   // .
   connect(_openclPlatformCombo
           ,QOverload<int>::of(&QComboBox::currentIndexChanged)
           ,this
           ,&SettingsDialog::currentOpenCLPlatformChanged);

   // .
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_openclPlatformCombo);
   ret->addWidget(_openclDeviceCombo);
   return ret;
}
