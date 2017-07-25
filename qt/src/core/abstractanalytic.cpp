#include <memory>

#include "abstractanalytic.h"
#include "datamanager.h"
#include "exception.h"
#include "datareference.h"



QMutex EAbstractAnalytic::_mutex;
using namespace std;






EAbstractAnalytic::~EAbstractAnalytic()
{
   for (auto i = _newData.constBegin(); i != _newData.constEnd() ;++i)
   {
      (*i)->finish();
   }
   qDeleteAll(_data);
   qDeleteAll(_files);
   qDeleteAll(_extraDatas);
}






void EAbstractAnalytic::run()
{
   // call initialize function of analytic
   bool preAllocate {initialize()};
   for (auto i = _newData.constBegin(); i != _newData.constEnd() ;++i)
   {
      (*i)->prepare(preAllocate);
   }

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






void EAbstractAnalytic::addFileIn(int argument, const QString &path)
{
   unique_ptr<QFile> file(new QFile(path));
   if ( !file->open(QIODevice::ReadOnly) )
   {
      ;//ERROR
   }
   setArgument(argument,file.get());
   _files.append(file.release());
}






void EAbstractAnalytic::addFileOut(int argument, const QString &path)
{
   unique_ptr<QFile> file(new QFile(path));
   if ( !file->open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      ;//ERROR
   }
   setArgument(argument,file.get());
   _files.append(file.release());
}






void EAbstractAnalytic::addDataIn(int argument, const QString &path, quint16 type)
{
   unique_ptr<Ace::DataReference> data {Ace::DataManager::getInstance().open(path)};
   if ( (*data)->isNew() )
   {
      ;//ERROR!
   }
   if ( (*data)->getType() != type )
   {
      ;//ERROR!
   }
   setArgument(argument,&((*data)->data()));
   _data.append(data.release());
}






void EAbstractAnalytic::addDataOut(int argument, const QString &path, quint16 type)
{
   unique_ptr<Ace::DataReference> data {Ace::DataManager::getInstance().open(path)};
   (*data)->clear(type);
   _newData.append(&((*data)->data()));
   setArgument(argument,&((*data)->data()));
   _data.append(data.release());
}






EAbstractData* EAbstractAnalytic::getExtraData(const QString &path, bool clear, quint16 type)
{
   // lock mutex
   _mutex.lock();

   // get data reference to new data object requested
   unique_ptr<Ace::DataReference> data;
   try
   {
      data.reset(Ace::DataManager::getInstance().open(path));
      if ( clear )
      {
         (*data)->clear(type);
      }
   }
   catch (...)
   {
      // if anything goes wrong unlock mutex and rethrow
      _mutex.unlock();
      throw;
   }
   if ( clear )
   {
      _newData.append(&((*data)->data()));
   }

   // add data reference to list of extra data objects
   _extraDatas.push_back(data.release());

   // unlock mutex and return pointer to new data
   _mutex.unlock();
   return &(*_extraDatas.back())->data();
}
