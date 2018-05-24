#ifndef ACE_DATAWINDOW_H
#define ACE_DATAWINDOW_H
#include <memory>
#include <QMainWindow>
#include <core/ace.h>
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
      ~DataWindow();
      void setWindowTitle(const QString& title);
   private slots:
      void metadataTriggered();
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
      QAction* _metadataAction;
      /*!
       */
      QAction* _closeAction;
   };
}



#endif