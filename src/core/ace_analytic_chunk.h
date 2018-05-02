#ifndef ACE_ANALYTIC_CHUNK_H
#define ACE_ANALYTIC_CHUNK_H
#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_abstractinput.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is the chunk run manager. This manager is used to execute only a portion of 
       * the blocks for an analytic, temporarily saving them in a binary file. The 
       * function of this class is to determine which part of the work blocks it must 
       * process into result blocks based off the index and size it is given. From there 
       * it processes the chunk of blocks and saved them into a temporary binary file. 
       * The location and file name of the chunk is determined by global settings and the 
       * index of this chunk manager. 
       */
      class Chunk : public AbstractManager, public AbstractInput
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
          * The chunk index for this chunk manager. 
          */
         int _index;
         /*!
          * The chunk size for this chunk run. 
          */
         int _size;
         /*!
          * Pointer to the abstract run object used to process work blocks. 
          */
         AbstractRun* _runner {nullptr};
         /*!
          * The file name of the temporary binary file this chunk manager stores all its 
          * result blocks. 
          */
         QString _fileName;
         /*!
          * Pointer to the qt file of this chunk manager's temporary binary file. 
          */
         QFile* _file;
         /*!
          * Pointer to the qt data stream associated with this chunk manager's temporary 
          * binary file. 
          */
         QDataStream* _stream {nullptr};
         /*!
          * The work block index after the last index this chunk manager must process. 
          */
         int _end;
         /*!
          * The next work block index this chunk manager must process. 
          */
         int _nextWork;
         /*!
          * A complicated integer used to keep track of how many result blocks have been 
          * saved to this chunk manager's binary file. It does not keep track of indexes 
          * because this manager does not sort its result blocks. 
          */
         int _nextResult;
      };
   }
}



#endif
