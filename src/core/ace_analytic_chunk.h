#ifndef ACE_ANALYTIC_CHUNK_H
#define ACE_ANALYTIC_CHUNK_H
#include "ace_analytic_manager.h"
#include "ace_analytic_iobase.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class Chunk : public Manager, public IOBase
      {
         Q_OBJECT
      public:
         Chunk(quint16 type, int index, int size);
         virtual ~Chunk() override final;
         virtual bool isFinished() const override final;
      protected:
         virtual QFile* addOutputFile(const QString& path) override final;
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system) override final;
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
         void process();
      private:
         void setupFile();
         void setupIndexes();
         void setupOpenCL();
         void setupSerial();
         /*!
          */
         int _index;
         /*!
          */
         int _size;
         /*!
          */
         Run* _runner {nullptr};
         /*!
          */
         QString _fileName;
         /*!
          */
         QFile* _file;
         /*!
          */
         QDataStream* _stream {nullptr};
         /*!
          */
         int _end;
         /*!
          */
         int _nextWork;
         /*!
          */
         int _nextResult;
      };
   }
}



#endif
