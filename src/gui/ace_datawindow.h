#ifndef ACE_DATAWINDOW_H
#define ACE_DATAWINDOW_H
#include <memory>
#include <QMainWindow>
#include <../core/ace.h>
//



namespace Ace
{
   /*!
    * This is the data window that displays a data object. This window displays the 
    * table model of the data object it contains. It also provides a menu for opening 
    * a metadata dialog for the system or user metadata of its data object. 
    */
   class DataWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      DataWindow(std::unique_ptr<DataObject>&& data, QWidget* parent = nullptr);
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private slots:
      void systemMetaTriggered();
      void userMetaTriggered();
   private:
      void createActions();
      void createMenu();
      void restoreSettings();
      /*!
       * The key used to save the state of this data window in qt settings. 
       */
      static const char* _stateKey;
      /*!
       * The key used to save the geometry of this data window in qt settings. 
       */
      static const char* _geometryKey;
      /*!
       * Pointer to the data object this window contains. 
       */
      DataObject* _data;
      /*!
       * The system metadata action for this window. 
       */
      QAction* _systemMetaAction;
      /*!
       * The user metadata action for this window. 
       */
      QAction* _userMetaAction;
      /*!
       * The close action for this window. 
       */
      QAction* _closeAction;
   };
}



#endif
