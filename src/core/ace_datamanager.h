#ifndef ACE_DATAMANAGER_H
#define ACE_DATAMANAGER_H
#include <QObject>
#include "ace.h"
//



namespace Ace
{
   /*!
    * This is a singleton class which handles all events for data objects being 
    * opened. This is used to make sure data objects do not remain open if they are 
    * being overwritten as a new data object from a running analytic. 
    */
   class DataManager : public QObject
   {
      Q_OBJECT
   public:
      static Ace::DataManager& instance();
   signals:
      /*!
       * Signals the file with the given path is being overwritten as a new data object 
       * with the given pointer. 
       *
       * @param canonicalPath Canonical path to data object being overwritten as a new 
       *                      data object. 
       *
       * @param object Pointer to the new data object overwriting the given file path. 
       */
      void dataOverwritten(const QString& canonicalPath, Ace::DataObject* object);
   public slots:
      void newDataOpened(const QString& canonicalPath, Ace::DataObject* object);
   private:
      /*!
       * Constructs a new data manager object. 
       */
      explicit DataManager() = default;
      /*!
       * Pointer to global instance the data manager object. 
       */
      static Ace::DataManager* _instance;
   };
}



#endif
