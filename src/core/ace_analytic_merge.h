#ifndef ACE_ANALYTIC_MERGE_H
#define ACE_ANALYTIC_MERGE_H
#include "ace_analytic_manager.h"
#include "ace_analytic_iobase.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class Merge : public Manager, public IOBase
      {
         Q_OBJECT
      public:
         Merge(quint16 type, int size);
         virtual bool isFinished() const override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
         void process();
      private:
         void readChunk(int index);
         void readBlock(QDataStream& stream);
         /*!
          */
         int _size;
         /*!
          */
         int _chunkSize;
         /*!
          */
         int _nextResult {0};
      };
   }
}



#endif
