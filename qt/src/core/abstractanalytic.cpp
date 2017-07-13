#include "abstractanalytic.h"
#include "datamanager.h"
#include "exception.h"
#include "datareference.h"



QMutex EAbstractAnalytic::_mutex;
using namespace std;






void EAbstractAnalytic::run()
{
   // call initialize function of analytic
   initialize();

   // initialize block info
   int blockSize {getBlockSize()};
   int done {0};
   bool blocks[blockSize] {true};

   // begin block while loop
   while ( done < blockSize )
   {
      for (int i = 0; i < blockSize ;++i)
      {
         if ( blocks[i] )
         {
            // if block is still alive run it
            if ( !runBlock(i) )
            {
               // block is done, remove it from active list
               blocks[i] = false;
               ++done;
            }
         }
      }
   }

   // call finish function of analytic
   finish();
}






EAbstractData* EAbstractAnalytic::getExtraData(const QString &path)
{
   // lock mutex
   _mutex.lock();

   // get data reference to new data object requested
   unique_ptr<Ace::DataReference> data;
   try
   {
      data.reset(Ace::DataManager::getInstance().open(path));
   }
   catch (...)
   {
      // if anything goes wrong unlock mutex and rethrow
      _mutex.unlock();
      throw;
   }

   // add data reference to list of extra data objects
   _extraDatas.push_back(data.release());

   // unlock mutex and return pointer to new data
   _mutex.unlock();
   return &(*_extraDatas.back())->data();
}
