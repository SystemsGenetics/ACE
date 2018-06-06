#ifndef ACE_ANALYTIC_ABSTRACTMANAGER_H
#define ACE_ANALYTIC_ABSTRACTMANAGER_H
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
       * analytic type. This class provides methods for all input for a new analytic run 
       * and starts the process of running it, along with cleaning up all open files and 
       * data objects once the analytic has finished running. An implementation of this 
       * class is responsible for running the analytic once this class starts it and 
       * signaling when it is finished. This class creates the correct type of 
       * implementation class depending on the input given and status of things like MPI. 
       * This is the root analytic class that should be used to interface with the 
       * analytic run system outside of the core library. 
       */
      class AbstractManager : public QObject
      {
         Q_OBJECT
      public:
         static std::unique_ptr<Ace::Analytic::AbstractManager> makeManager(quint16 type, int index, int size);
         int size() const;
         EAbstractAnalytic::Input::Type type(int index) const;
         QVariant data(int index, EAbstractAnalytic::Input::Role role) const;
         void set(int index, const QVariant& value);
      signals:
         /*!
          * Signals the running analytic has made progress to the given percentage. 
          *
          * @param percentComplete The percent complete out of 100 for this analytic. 
          */
         void progressed(int percentComplete);
         /*!
          * Signals the analytic is done. 
          */
         void done();
         /*!
          * Signals the analytic and this manager has completely finished execution and is 
          * ready to be destroyed. 
          */
         void finished();
      public slots:
         void initialize();
         void terminationRequested();
         void finish();
      protected:
         explicit AbstractManager(quint16 type);
         virtual QFile* addOutputFile(const QString& path);
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system);
         std::unique_ptr<EAbstractAnalytic::Block> makeWork(int index);
         void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result, int expectedIndex);
         EAbstractAnalytic* analytic();
         const EAbstractAnalytic* analytic() const;
      protected slots:
         virtual void start();
      private:
         void setupInput();
         void inputBasic();
         void inputFiles();
         QFile* addInputFile(const QString& path);
         void inputData();
         EMetadata inputDataIn();
         Ace::DataObject* addInputData(const QString& path);
         EMetadata buildMeta(const QList<Ace::DataObject*>& inputs);
         EMetadata buildMetaVersion();
         EMetadata buildMetaInput(const QList<Ace::DataObject*>& inputs);
         EMetadata buildMetaCommand();
         void inputDataOut(const EMetadata& system);
         /*!
          * The analytic type this manager uses. 
          */
         quint16 _type;
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
          * Temporary list of input values for this manager's analytic that is used to hold 
          * those values until setting them is finished and then added to the analytic. 
          */
         QVector<QVariant> _inputs;
         /*!
          * Pointer list of new output data objects this manager saves so they can be 
          * finished once this manager's analytic is finished. 
          */
         QList<Ace::DataObject*> _outputData;
         /*!
          * The percent of blocks this manager has completed processing. 
          */
         int _percentComplete {0};
      };
   }
}



#endif
