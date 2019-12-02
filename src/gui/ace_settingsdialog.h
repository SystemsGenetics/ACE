#ifndef ACE_SETTINGSDIALOG_H
#define ACE_SETTINGSDIALOG_H
#include <QDialog>



class QComboBox;
class QSpinBox;
class QLineEdit;
class QCheckBox;
//



namespace Ace
{
   /*!
    * This is the settings dialog. This provides all persistent global settings for 
    * ACE to the user to view and edit. 
    */
   class SettingsDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit SettingsDialog();
   private slots:
      void okClicked();
      void applyClicked();
      void currentOpenCLPlatformChanged(int index);
   private:
      QLayout* setupWarning();
      QLayout* createForm();
      QLayout* createButtons();
      QLayout* createCUDA();
      QLayout* createOpenCL();
      /*!
       * The combo box for this dialog used to select the CUDA device.
       */
      QComboBox* _cudaDeviceCombo;
      /*!
       * The combo box for this dialog used to select the OpenCL platform. 
       */
      QComboBox* _openclPlatformCombo;
      /*!
       * The combo box for this dialog used to select the OpenCL device. 
       */
      QComboBox* _openclDeviceCombo;
      /*!
       * The spin box for this dialog used to edit the thread size setting. 
       */
      QSpinBox* _threadEdit;
      /*!
       * The spin box for this dialog used to edit the buffer size setting. 
       */
      QSpinBox* _bufferEdit;
      /*!
       * The line edit for this dialog used to edit the chunk working directory setting. 
       */
      QLineEdit* _chunkDirEdit;
      /*!
       * The line edit for this dialog used to edit the chunk prefix setting. 
       */
      QLineEdit* _chunkPrefixEdit;
      /*!
       * The line edit for this dialog used to edit the chunk extension setting. 
       */
      QLineEdit* _chunkExtensionEdit;
      QCheckBox* _loggingEdit;
      QSpinBox* _loggingPortEdit;
   };
}



#endif
