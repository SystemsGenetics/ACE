#include "ace_settingsdialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <core/ace_settings.h>
#include <core/opencl_platform.h>
#include <core/opencl_device.h>



using namespace Ace;
//






/*!
 */
SettingsDialog::SettingsDialog()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());
   setLayout(layout);
}






/*!
 */
void SettingsDialog::okClicked()
{
   applyClicked();
   accept();
}






/*!
 */
void SettingsDialog::applyClicked()
{
   Ace::Settings& settings {Ace::Settings::instance()};
   if ( _platformCombo->currentIndex() == OpenCL::Platform::size() )
   {
      settings.setOpenCLPlatform(-1);
      settings.setOpenCLDevice(0);
   }
   else
   {
      settings.setOpenCLPlatform(_platformCombo->currentIndex());
      settings.setOpenCLDevice(_deviceCombo->currentIndex());
   }
   settings.setThreadSize(_threadEdit->value());
   settings.setBufferSize(_bufferEdit->value());
   settings.setChunkDir(_chunkDirEdit->text());
   settings.setChunkPrefix(_chunkPrefixEdit->text());
   settings.setChunkExtension(_chunkExtensionEdit->text());
}






/*!
 *
 * @param index  
 */
void SettingsDialog::currentPlatformChanged(int index)
{
   _deviceCombo->clear();
   if ( index >= 0 && index < OpenCL::Platform::size() )
   {
      for (int i = 0; i < OpenCL::Platform::get(index)->deviceSize() ;++i)
      {
         _platformCombo->addItem(OpenCL::Platform::get(index)->device(i)->name());
      }
   }
}






/*!
 */
QLayout* SettingsDialog::createForm()
{
   Ace::Settings& settings {Ace::Settings::instance()};
   _threadEdit = new QSpinBox;
   _threadEdit->setMinimum(1);
   _threadEdit->setMaximum(INT_MAX);
   _threadEdit->setValue(settings.threadSize());
   _bufferEdit = new QSpinBox;
   _bufferEdit->setMinimum(1);
   _bufferEdit->setMaximum(INT_MAX);
   _bufferEdit->setValue(settings.bufferSize());

   _chunkDirEdit = new QLineEdit;
   _chunkDirEdit->setText(settings.chunkDir());
   _chunkPrefixEdit = new QLineEdit;
   _chunkPrefixEdit->setText(settings.chunkPrefix());
   _chunkExtensionEdit = new QLineEdit;
   _chunkExtensionEdit->setText(settings.chunkExtension());

   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("OpenCL Device:")),createOpenCL());
   ret->addRow(new QLabel(tr("Thread Size:")),_threadEdit);
   ret->addRow(new QLabel(tr("Buffer Size:")),_bufferEdit);
   ret->addRow(new QLabel(tr("Chunk Working Directory:")),_chunkDirEdit);
   ret->addRow(new QLabel(tr("Chunk Prefix:")),_chunkPrefixEdit);
   ret->addRow(new QLabel(tr("Chunk Extension:")),_chunkExtensionEdit);
   return ret;
}






/*!
 */
QLayout* SettingsDialog::createButtons()
{
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* apply {new QPushButton(tr("Apply"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&SettingsDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&SettingsDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);

   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);
   return ret;
}






/*!
 */
QLayout* SettingsDialog::createOpenCL()
{
   Ace::Settings& settings {Ace::Settings::instance()};
   _platformCombo = new QComboBox;
   for (int i = 0; i < OpenCL::Platform::size() ;++i)
   {
      _platformCombo->addItem(OpenCL::Platform::get(i)->name());
   }
   _platformCombo->addItem(tr("none"));
   _deviceCombo = new QComboBox;
   if ( settings.openCLDevicePointer() )
   {
      _platformCombo->setCurrentIndex(settings.openCLPlatform());
      currentPlatformChanged(settings.openCLPlatform());
      _deviceCombo->setCurrentIndex(settings.openCLDevice());
   }
   else
   {
      _platformCombo->setCurrentIndex(OpenCL::Platform::size());
   }

   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_platformCombo);
   ret->addWidget(_deviceCombo);
   return ret;
}
