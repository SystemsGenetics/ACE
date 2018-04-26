#ifndef ACE_ANALYTIC_MANAGER_H
#define ACE_ANALYTIC_MANAGER_H
#include <memory>
#include <QObject>
#include <QVector>
#include <QVariant>
#include "eabstractanalytic_input.h"
#include "ace.h"



class QFile;
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This represents a session manager for a single analytic run for a specific 
       * analytic type. This class takes care of processing all input for a new analytic 
       * run and starts the process of running it, along with cleaning up all open files 
       * and data objects once the analytic has finished running. An implementation of 
       * this class is responsible for running the analytic once this class starts it and 
       * signaling when it is finished. This class creates the correct type of 
       * implementation class depending on the input given and status of things like MPI. 
       * This class also manages its own deletion, calling on the qt system to delete it 
       * once it finishes executing its analytic or termination is requested. 
       */
      class Manager : public QObject
      {
         Q_OBJECT
      public:
         static std::unique_ptr<Ace::Analytic::Manager> makeManager(quint16 type, int index, int size);
         Manager(quint16 type);
         int size() const;
         EAbstractAnalytic::Input::Type type(int index) const;
         QVariant data(int index, EAbstractAnalytic::Input::Role role) const;
         void set(int index, const QVariant& value);
         void initialize();
      signals:
         /*!
          * Signals the running analytic has made progress to the given percentage. 
          *
          * @param percentComplete The percent complete out of 100 for this analytic. 
          */
         void progressed(int percentComplete);
         /*!
          * Signals the analytic and this manager has finished execution. 
          */
         void finished();
      public slots:
         void terminationRequested();
         void finish();
      protected slots:
         /*!
          * This interface is called once to begin the analytic run for this manager after 
          * all argument input has been set. 
          */
         virtual void start() = 0;
      protected:
         virtual QFile* addOutputFile(const QString& path);
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system);
         EAbstractAnalytic* analytic();
         const EAbstractAnalytic* analytic() const;
      private:
         void inputBasic();
         void inputFiles();
         QFile* addInputFile(const QString& path);
         void inputData();
         EMetadata inputDataIn();
         Ace::DataObject* addInputData(const QString& path);
         EMetadata buildMeta(const QList<Ace::DataObject*>& inputs);
         EMetadata buildMetaInput(const QList<Ace::DataObject*>& inputs);
         EMetadata buildMetaCommand();
         void inputDataOut(const EMetadata& system);
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
         /*!
          */
         QVector<QVariant> _inputs;
         /*!
          */
         QList<Ace::DataObject*> _outputData;
      };
   }
}



#endif
