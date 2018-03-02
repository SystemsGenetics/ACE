#include <memory>

#include "abstractanalytic.h"
#include "datamanager.h"
#include "exception.h"
#include "datareference.h"
#include "opencldevice.h"
#include "ace_qmpi.h"



QMutex EAbstractAnalytic::_mutex;
using namespace std;






EAbstractAnalytic::~EAbstractAnalytic()
{
   // cleanup all open data objects and files
   qDeleteAll(_dataIn);
   qDeleteAll(_dataOut);
   qDeleteAll(_files);
}






void EAbstractAnalytic::run()
{
   try
   {
      // call prepare run for analytic
      prepareRun();

      // check to see if analytic can run OpenCL and there is a device to use
      if ( getCapabilities()&Capabilities::OpenCL
           && EOpenCLDevice::getInstance().getStatus() == EOpenCLDevice::Ok )
      {
         // initialize block info
         int blockSize {getBlockSize()};
         int done {0};
         bool blocks[blockSize];
         for (int i = 0; i < blockSize ;++i)
         {
            blocks[i] = true;
         }

         // begin block while loop
         while ( done < blockSize )
         {
            for (int i = 0; i < blockSize ;++i)
            {
               // if interruption is requested exit
               if ( isInterruptionRequested() )
               {
                  return;
               }

               // make sure block is still alive
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
      }

      // else just run serial if possible
      else if ( getCapabilities()&Capabilities::Serial )
      {
         runSerial();
      }

      // else analytic cannot run and throw failure
      else
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Failed Analytic Execution."));
         e.setDetails(tr("Could not execute analytic because it lacks any applicable capability."));
         throw e;
      }

      // call finish analytic run
      finishRun();

      // emit finished signal
      emit finished();
   }
   catch (EException e)
   {
      // If exception occured report it to the main thread
      emit exceptionThrown(e.getFile(),e.getLine(),e.getFunction(),e.getTitle(),e.getDetails());
   }
}






void EAbstractAnalytic::mpiRun(/*Ace::MPI& mpi*/)
{
   // MUST complete new MPI class before I can do this
}






void EAbstractAnalytic::stop()
{
   // kill the thread if it is still running
   if ( isRunning() )
   {
      requestInterruption();
      wait();
   }
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
   (*data)->open();

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






void EAbstractAnalytic::startMPI()
{
   Ace::QMPI& mpi {Ace::QMPI::initialize()};
   connect(&mpi,&Ace::QMPI::dataReceived,this,&EAbstractAnalytic::receiveData);
   connect(this,&EAbstractAnalytic::sendData,&mpi,&Ace::QMPI::sendData);
   if ( mpi.isMaster() )
   {
      prepareRun();
      processMPI();
   }
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
      (*data)->open();

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






void EAbstractAnalytic::receiveData(const QByteArray& data, int fromRank)
{
   Q_UNUSED(fromRank)
   Ace::QMPI& mpi {Ace::QMPI::initialize()};
   if ( mpi.isMaster() )
   {
      _mpiBlocks << data;
      --_mpiOut;
      processMPI();
   }
   else
   {
      emit sendData(0,processMPIBlock(data));
   }
}






void EAbstractAnalytic::processMPI()
{
   Ace::QMPI& mpi {Ace::QMPI::initialize()};
   for (auto block = _mpiBlocks.begin(); block != _mpiBlocks.end() ;)
   {
      if ( readMPIBlock(*block) ) block = _mpiBlocks.erase(block);
      else ++block;
   }
   while ( _mpiOut < (mpi.size()*8) )
   {
      QByteArray data {buildMPIBlock()};
      if ( !data.isEmpty() )
      {
         emit sendData(_nextOut++,data);
         ++_mpiOut;
         if ( _nextOut == mpi.size() ) _nextOut = 1;
      }
      else if ( _mpiBlocks.isEmpty() && _mpiOut == 0 )
      {
         finishRun();
         emit finished();
         break;
      }
      else
      {
         QTimer::singleShot(1000, this, SLOT(processMPI()));
         break;
      }
   }
}






void EAbstractAnalytic::prepareRun()
{
   // call initialize function of analytic
   bool preAllocate {initialize()};

   // go through all new data objects and call their prepare function
   for (auto dataOut : qAsConst(_dataOut))
   {
      dataOut->get()->data().prepare(preAllocate);
   }
}






void EAbstractAnalytic::finishRun()
{
   // call finish function of analytic
   finish();

   // create metadata history for each output data file
   EMetadata inputs(EMetadata::Object);
   for (auto i = _dataIn.constBegin(); i != _dataIn.constEnd() ;++i)
   {
      EMetadata* file = new EMetadata((**i)->getMeta());
      QFileInfo fileInfo((**i)->getPath());
      QString path = fileInfo.fileName();
      while ( inputs.toObject()->contains(path) )
      {
         path.prepend('_');
      }
      file->setParent(&inputs);
      inputs.toObject()->insert(path,file);
   }

   // create command metadata that created new data objects
   EMetadata command(EMetadata::String);
   *command.toString() = _command;

   // iterate through all output data objects, removing each one from reference list
   for (auto i = _dataOut.constBegin(); i != _dataOut.constEnd() ;++i)
   {
      // call finish function, add metadata history and command, and delete reference
      (**i)->data().finish();
      EMetadata::Map* object = (**i)->getMeta().toObject();
      EMetadata* newInputs = new EMetadata(inputs);
      EMetadata* newCommand = new EMetadata(command);
      if ( object->contains("inputs") )
      {
         delete object->take("inputs");
      }
      if ( object->contains("command") )
      {
         delete object->take("command");
      }
      if ( newInputs->toObject()->size() > 0 )
      {
         object->insert("inputs",newInputs);
      }
      object->insert("command",newCommand);
      newInputs->setParent(&((**i)->getMeta()));
      newCommand->setParent(&((**i)->getMeta()));
      (**i)->writeMeta();
      delete *i;
   }
   _dataOut.clear();
}
