#ifndef ACE_ANALYTIC_MANAGER_H
#define ACE_ANALYTIC_MANAGER_H
#include <memory>
#include <QObject>
#include "global.h"



class QFile;
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This represents a session manager for a single analytic run for a specific 
       * analytic type. This class takes care of processing all input for a new analytic 
       * run and starts the process of running it. An implementation of this class is 
       * responsible for running the analytic once this class starts it. This class 
       * creates the correct type of implementation class depending on the input given 
       * and status of things like MPI. 
       */
      class Manager : public QObject
      {
         Q_OBJECT
      public:
         static std::unique_ptr<Ace::Analytic::Manager> makeManager(quint16 type, int index, int size);
         Manager(int type);
         int size() const;
         EAbstractAnalytic::Input::Type type(int index) const;
         QVariant data(int index, EAbstractAnalytic::Input::Role role) const;
         void set(int index, const QVariant& value);
         void setInputFile(const QString& path);
         virtual void setOutputFile(const QString& path);
         void setInputData(const QString& path);
         virtual void setOutputData(const QString& path);
         void finish();
      signals:
         /*!
          * Signals the running analytic has made progress to the given percentage. 
          *
          * @param percentComplete The percent complete out of 100 for this analytic. 
          */
         void progressed(int percentComplete);
      public slots:
         void terminationRequested();
      protected:
         /*!
          * This interface is called once to begin the analytic run for this manager after 
          * all argument input has been set. 
          */
         virtual void run() = 0;
         EAbstractAnalytic* analytic();
         const EAbstractAnalytic* analytic() const;
      private:
         /*!
          * Pointer to this manager's abstract analytic based off the analytic type this 
          * manager was given. 
          */
         EAbstractAnalytic* _analytic;
         /*!
          * Pointer to the abstract analytic input object for this manager's abstract 
          * analytic. Once the finish method is called the input object is deleted and this 
          * is set to null. 
          */
         EAbstractAnalytic::Input* _input;
      };
   }
}



#endif
