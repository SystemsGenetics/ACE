#include <memory>

#include "abstractanalytic.h"
#include "datamanager.h"
#include "exception.h"
#include "datareference.h"



QMutex EAbstractAnalytic::_mutex;
using namespace std;






EAbstractAnalytic::~EAbstractAnalytic()
{
   // go through any remaining new data objects and call their finish function
   for (auto i = _dataOut.constBegin(); i != _dataOut.constEnd() ;++i)
   {
      (**i)->data().finish();
      (**i)->writeMeta();
   }

   // cleanup all open data objects and files
   qDeleteAll(_dataIn);
   qDeleteAll(_dataOut);
   qDeleteAll(_files);
}






void EAbstractAnalytic::run()
{
   // call initialize function of analytic
   bool preAllocate {initialize()};

   // go through all new data objects and call their prepare function
   for (auto i = _dataOut.constBegin(); i != _dataOut.constEnd() ;++i)
   {
      (**i)->data().prepare(preAllocate);
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

   // create metadata history for each output data file
   EMetadata history(EMetadata::Object);
   for (auto i = _dataIn.constBegin(); i != _dataIn.constEnd() ;++i)
   {
      EMetadata* file = new EMetadata((**i)->getMeta());
      QString path = (**i)->getPath();
      while ( history.toObject()->contains(path) )
      {
         path.prepend('_');
      }
      history.toObject()->insert(path,file);
   }

   // create command metadata that created new data objects
   EMetadata command(EMetadata::String);
   *command.toString() = _command;

   // iterate through all output data objects, removing each one from reference list
   Ace::DataReference* data;
   while ( ( data = _dataOut.takeAt(0) ) != nullptr )
   {
      // call finish function, add metadata history and command, and delete reference
      (*data)->data().finish();
      EMetadata::Map* object = (*data)->getMeta().toObject();
      object->remove("history");
      object->remove("command");
      object->insert("history",new EMetadata(history));
      object->insert("command",new EMetadata(command));
      (*data)->writeMeta();
      delete data;
   }

   // emit finished signal
   emit finished();
}






void EAbstractAnalytic::addFileIn(int argument, const QString &path)
{
   // open file for read only and make sure it worked
   unique_ptr<QFile> file(new QFile(path));
   if ( !file->open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Abstract Analytic"));
      e.setDetails(tr("Failed opening file %1 for read only.").arg(path));
      throw e;
   }

   // call set argument function and append to list of files
   setArgument(argument,file.get());
   _files.append(file.release());
}






void EAbstractAnalytic::addFileOut(int argument, const QString &path)
{
   // open file for write only and truncate, making sure it worked
   unique_ptr<QFile> file(new QFile(path));
   if ( !file->open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Abstract Analytic"));
      e.setDetails(tr("Failed opening file %1 for write only and truncate.").arg(path));
      throw e;
   }

   // call set argument function and append to list of files
   setArgument(argument,file.get());
   _files.append(file.release());
}






void EAbstractAnalytic::addDataIn(int argument, const QString &path, quint16 type)
{
   // open data object from manager
   unique_ptr<Ace::DataReference> data {Ace::DataManager::getInstance().open(path)};

   // make sure it is not new
   if ( (*data)->isNew() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Abstract Analytic"));
      e.setDetails(tr("Failed opening data object %1 for input; it is a new object.").arg(path));
      throw e;
   }

   // make sure it is the expected type
   if ( (*data)->getType() != type )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Abstract Analytic"));
      e.setDetails(tr("Failed opening data object %1 for input; expected type %2 when it is type"
                      " %3.").arg(path).arg(type).arg((*data)->getType()));
      throw e;
   }

   // call set argument function and add to list of data objects
   setArgument(argument,&((*data)->data()));
   _dataIn.append(data.release());
}






void EAbstractAnalytic::addDataOut(int argument, const QString &path, quint16 type)
{
   // open data object and clear it of any data
   unique_ptr<Ace::DataReference> data {Ace::DataManager::getInstance().open(path)};
   (*data)->clear(type);

   // call set argument function and add to list of data objects
   setArgument(argument,&((*data)->data()));
   _dataOut.append(data.release());
}






EAbstractData* EAbstractAnalytic::getDataIn(const QString &path, quint16 type)
{
   // lock mutex
   _mutex.lock();

   // get data reference to new data object requested
   unique_ptr<Ace::DataReference> data;
   try
   {
      // open data object
      data.reset(Ace::DataManager::getInstance().open(path));

      // make sure it is not new
      if ( (*data)->isNew() )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Abstract Analytic"));
         e.setDetails(tr("Failed opening data object %1 for input; it is a new object.").arg(path));
         throw e;
      }

      // make sure it is the expected type
      if ( (*data)->getType() != type )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Abstract Analytic"));
         e.setDetails(tr("Failed opening data object %1 for input; expected type %2 when it is type"
                         " %3.").arg(path).arg(type).arg((*data)->getType()));
         throw e;
      }
   }
   catch (...)
   {
      // if anything goes wrong unlock mutex and rethrow
      _mutex.unlock();
      throw;
   }

   // add data reference to list of extra data objects
   _dataIn.push_back(data.release());

   // unlock mutex and return pointer to new data
   _mutex.unlock();
   return &(*_dataIn.back())->data();
}






EAbstractData* EAbstractAnalytic::getDataOut(const QString &path, quint16 type)
{
   // lock mutex
   _mutex.lock();

   // get data reference to new data object requested
   unique_ptr<Ace::DataReference> data;
   try
   {
      // open data object and clear it
      data.reset(Ace::DataManager::getInstance().open(path));
      (*data)->clear(type);
   }
   catch (...)
   {
      // if anything goes wrong unlock mutex and rethrow
      _mutex.unlock();
      throw;
   }

   // add data reference to list of extra data objects
   _dataOut.push_back(data.release());

   // unlock mutex and return pointer to new data
   _mutex.unlock();
   return &(*_dataOut.back())->data();
}
