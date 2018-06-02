#ifndef ACE_SETTINGSDIALOG_H
#define ACE_SETTINGSDIALOG_H
#include <QDialog>



class QComboBox;
class QSpinBox;
class QLineEdit;
//



namespace Ace
{
   /*!
    */
   class SettingsDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit SettingsDialog();
   private slots:
      void okClicked();
      void applyClicked();
      void currentPlatformChanged(int index);
   private:
      QLayout* createForm();
      QLayout* createButtons();
      QLayout* createOpenCL();
      /*!
       */
      QComboBox* _platformCombo;
      /*!
       */
      QComboBox* _deviceCombo;
      /*!
       */
      QSpinBox* _threadEdit;
      /*!
       */
      QSpinBox* _bufferEdit;
      /*!
       */
      QLineEdit* _chunkDirEdit;
      /*!
       */
      QLineEdit* _chunkPrefixEdit;
      /*!
       */
      QLineEdit* _chunkExtensionEdit;
   };
}



#endif
