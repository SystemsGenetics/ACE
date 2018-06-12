#ifndef ACE_DATAWINDOW_H
#define ACE_DATAWINDOW_H
#include <memory>
#include <QMainWindow>
#include <../core/ace.h>
//



namespace Ace
{
   /*!
    */
   class DataWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      DataWindow(std::unique_ptr<DataObject>&& data, QWidget* parent = nullptr);
      void setWindowTitle(const QString& title);
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
       */
      static const char* _stateKey;
      /*!
       */
      static const char* _geometryKey;
      /*!
       */
      DataObject* _data;
      /*!
       */
      QString _title;
      /*!
       */
      QAction* _systemMetaAction;
      /*!
       */
      QAction* _userMetaAction;
      /*!
       */
      QAction* _closeAction;
   };
}



#endif
