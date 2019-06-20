#ifndef ACE_ANALYTIC_MERGE_H
#define ACE_ANALYTIC_MERGE_H
#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_abstractinput.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all classes related to running an analytic. This required its
    * own name space because of the immense complexity required for ACE to provide
    * an abstract interface for running analytic types in heterogeneous
    * environments.
    */
   namespace Analytic
   {
      /*!
       * This is the merge manager. This manager is used to take all the chunk files
       * produced by all the chunk run processes and merge them together finishing the
       * execution of the underlying analytic by having it process all result blocks
       * stored in the temporary binary chunk files.
       */
      class Merge : public AbstractManager, public AbstractInput
      {
         Q_OBJECT
      public:
         virtual bool isFinished() const override final;
      public:
         explicit Merge(quint16 type, int size);
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result) override final;
      protected slots:
         virtual void start() override final;
         void process();
      private:
         void readChunk(int index);
         void readBlock(QDataStream& stream);
      private:
         /*!
          * The chunk size used for this chunk run which this merge manager uses to
          * determine the input files and chunk size.
          */
         int _size;
         /*!
          * The chunk size for this chunk run that is being merged by this manager. This
          * size is the number of result blocks each chunk file should contain excluding
          * the last chunk index file.
          */
         int _chunkSize;
         /*!
          * The next result block index to be processed to maintain order.
          */
         int _nextResult {0};
      };
   }
}

#endif
