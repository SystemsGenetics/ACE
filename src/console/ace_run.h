#ifndef ACE_RUN_H
#define ACE_RUN_H
#include <QTextStream>
#include <core/ace_analytic.h>
#include "ace_options.h"
#include "ace_command.h"
//



namespace Ace
{
   /*!
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
      void addInteger(int index, const QString& name);
      void addDouble(int index, const QString& name);
      void addSelection(int index, const QString& name);
      /*!
       */
      QTextStream _stream;
      /*!
       */
      Options _options;
      /*!
       */
      Command _command;
      /*!
       */
      Analytic::Manager* _manager;
      /*!
       */
      int _index;
      /*!
       */
      int _size;
   };
}



#endif
