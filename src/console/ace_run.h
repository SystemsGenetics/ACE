#ifndef ACE_RUN_H
#define ACE_RUN_H
#include <QTextStream>
#include <../core/ace_analytic.h>
#include "ace_options.h"
#include "ace_command.h"
//



namespace Ace
{
   /*!
    * This handles all run commands for the console program of ACE. These commands 
    * include run, chunk run, and merge. If MPI is being used it is auto detected by 
    * this class. The main responsibility of this class is to initialize an analytic 
    * manager and start its process of running the analytic. This class handles all 
    * input from the user, feeding it to the analytic manager to setup the analytic. 
    * This class is event driven and expects the qt event system to be running in 
    * order to function properly. When the analytic manager signals it is finished 
    * this class deletes itself and as a result deletes the manager. 
    */
   class Run : public QObject
   {
      Q_OBJECT
   public:
      Run(const Command& command, const Options& options);
   private slots:
      void progressed(int percentComplete);
      void done();
      void finished();
   private:
      void setupIndexes();
      void setupChunk();
      void setupMerge();
      quint16 getType();
      void setupManager(quint16 type);
      void addArguments();
      void addInteger(int index, const QString& key);
      void addDouble(int index, const QString& key);
      void addSelection(int index, const QString& key);
      /*!
       * A qt text stream associated with standard output and used as such by this 
       * object. 
       */
      QTextStream _stream;
      /*!
       * The options derived from the command line arguments of this program. 
       */
      Options _options;
      /*!
       * The command arguments derived from the command line arguments of this program. 
       */
      Command _command;
      /*!
       * Pointer to this object's analytic manager used for managing the execution of 
       * this object's analytic run. 
       */
      Analytic::AbstractManager* _manager;
      /*!
       * The chunk index for this analytic run. The default value means this is not a 
       * chunk or merge run. 
       */
      int _index {0};
      /*!
       * The chunk size for this analytic run. The default value means this is not a 
       * chunk or merge run. 
       */
      int _size {1};
   };
}



#endif
