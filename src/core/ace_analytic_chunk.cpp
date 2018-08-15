#include "ace_analytic_chunk.h"
#include <QTimer>
#include "ace_analytic_serialrun.h"
#include "ace_analytic_openclrun.h"
#include "ace_settings.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Implements the interface that tests if this abstract input is finished and 
 * received all result blocks for its analytic. This implementation is special in 
 * that it only processes a chunk of its analytic work blocks therefore it is 
 * finished once it has received all of its portion of result blocks. 
 *
 * @return True if this abstract input is finished or false otherwise. 
 */
bool Chunk::isFinished() const
{
   return _nextResult >= _end;
}






/*!
 * Constructs a new chunk manager with the given analytic type, chunk index, and 
 * chunk size. 
 *
 * @param type The analytic type that is used. 
 *
 * @param index The chunk index for this process. 
 *
 * @param size The chunk size which is the total number of chunks the analytic is 
 *             split into. 
 */
Chunk::Chunk(quint16 type, int index, int size):
   AbstractManager(type),
   _index(index),
   _size(size)
{}






/*!
 * Deletes the qt data stream used for saved data to the chunk file. 
 */
Chunk::~Chunk()
{
   delete _stream;
}






/*!
 * Implements the interface that opens a new file set to write only and truncate 
 * with the given path. This implementation does nothing and returns a null pointer 
 * because a chunk manager saves result blocks to a special binary file. 
 *
 * @param path Unused path to file. 
 */
QFile* Chunk::addOutputFile(const QString& path)
{
   Q_UNUSED(path)
   return nullptr;
}






/*!
 * This interface opens a new data object with the given path, erasing any data the 
 * file may have contained and returning a pointer to the new data object. This 
 * implementation does nothing and returns a null pointer because a chunk manager 
 * saved result blocks to a special binary file. 
 *
 * @param path Unused path to data object file. 
 *
 * @param type Unused data object type. 
 *
 * @param system Unused system metadata for new data objects. 
 */
Ace::DataObject* Chunk::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   Q_UNUSED(path)
   Q_UNUSED(type)
   Q_UNUSED(system)
   return nullptr;
}






/*!
 * Implements the interface that saves the given result block to its underlying 
 * analytic and assumes the order of indexes given is not sorted and random. This 
 * implementation simply saves the raw result block to a temporary binary file. 
 *
 * @param result The result block that is saved to a temporary binary file. 
 */
void Chunk::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   // save the given result block to the temporary binary file deleting it and 
   // increment the next result. If any write error to the file occurs then throw an 
   // exception. 
   QByteArray data {result->toBytes()};
   result.reset();
   *_stream << data.size();
   _stream->device()->write(data);
   ++_nextResult;
   if ( _stream->status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Write Error"));
      e.setDetails(tr("Failed writing to temporary chunk file %1: %2")
                   .arg(_fileName)
                   .arg(_file->errorString()));
      throw e;
   }
}






/*!
 * Implements the interface that is called once to begin the analytic run for this 
 * manager after all argument input has been set. This implementation initializes 
 * the chunk file, indexes, and schedules the process slot to be called. 
 */
void Chunk::start()
{
   // Setup OpenCL, setup serial if OpenCL fails, and connect this abstract run's 
   // finished signal with this manager's finish slot. 
   if ( !setupOpenCL() )
   {
      setupSerial();
   }
   connect(_runner,&AbstractRun::finished,this,&AbstractManager::finish);

   // Setup the chunk file, setup the chunk indexes, and schedule the process slot to 
   // be called. 
   setupFile();
   setupIndexes();
   QTimer::singleShot(0,this,&Chunk::process);
}






/*!
 * Processes the work blocks this chunk manager is responsible for saving, adding 
 * them to this manager's abstract run object for processing. 
 */
void Chunk::process()
{
   // While this manager still has work block indexes to process make the next work 
   // block and add it to this manager's abstract run object. 
   while ( _nextWork < _end )
   {
      _runner->addWork(makeWork(_nextWork++));
   }
}






/*!
 * Opens this chunk manager's temporary binary file as write only and truncated. 
 * This also creates this manager's qt data stream used for file output. If opening 
 * fails then an exception is thrown. 
 */
void Chunk::setupFile()
{
   // Determine this chunk's file name from the global settings object. 
   Settings& settings {Settings::instance()};
   _fileName = settings.chunkDir().append("/")
                                  .append(settings.chunkPrefix())
                                  .append(QString::number(_index))
                                  .append(".")
                                  .append(settings.chunkExtension());

   // Create a new qt file and open this chunk's file as write only and truncate. If 
   // opening fails then throw an exception, else go to the next step. 
   _file = new QFile(_fileName,this);
   if ( !_file->open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Open Error"));
      e.setDetails(tr("Failed opening temporary chunk file %1: %2")
                   .arg(_fileName)
                   .arg(_file->errorString()));
      throw e;
   }

   // Save the file name of this chunk's binary file and create a new qt data stream 
   // from the open file. 
   _fileName = QFileInfo(_fileName).fileName();
   _stream = new QDataStream(_file);
}






/*!
 * Determines the starting work block index and ending work block index for this 
 * chunk manager. 
 */
void Chunk::setupIndexes()
{
   // Determine the chunk size, or how many blocks each chunk is responsible for, and 
   // then determine the first next work index and the end index for this chunk 
   // manager. 
   int chunkSize {analytic()->size()/_size + ( analytic()->size()%_size ? 1 : 0)};
   _nextWork = _nextResult = _index*chunkSize;
   _end = (_index + 1)*chunkSize;
   _end = _end > analytic()->size() ? analytic()->size() : _end;
}






/*!
 * Attempts to initialize an OpenCL run object for block processing for this 
 * manager. If successful sets this manager's abstract run pointer. 
 */
bool Chunk::setupOpenCL()
{
   // If the singleton settings object does not contain a valid OpenCL device pointer 
   // then do nothing and exit, else go to the next step. 
   bool ret {false};
   int index {_index};
   if ( Settings::instance().openCLDevicePointer() )
   {

      // Attempt to find an OpenCL index that is the nth device based off this slave 
      // node's process ranking minus one to account for the master node. All devices 
      // from all platforms are considered. If there is not enough devices then no 
      // device is found. 
      OpenCL::Device* device {nullptr};
      for (int p = 0; ( p < OpenCL::Platform::size() ) && !device ;++p)
      {
         for (int d = 0; d < OpenCL::Platform::get(p)->deviceSize() ;++d)
         {
            if ( index-- == 0 )
            {
               device = OpenCL::Platform::get(p)->device(d);
               break;
            }
         }
      }

      // If an OpenCL device was found and this manager's analytic creates a valid 
      // abstract OpenCL object then create a new OpenCL run object and set it to this 
      // object's run pointer. 
      if ( device )
      {
         if ( EAbstractAnalytic::OpenCL* opencl = analytic()->makeOpenCL() )
         {
            _runner = new OpenCLRun(opencl,device,this,this);
            ret = true;
         }
      }
   }
   return ret;
}






/*!
 * Initializes this object's abstract run object as a serial run object if it has 
 * not already been set. If this manager's analytic fails creating a valid abstract 
 * serial object then an exception is thrown. 
 */
void Chunk::setupSerial()
{
   // If this object already has an abstract run object then do nothing and exit, 
   // else go to the next step. 
   if ( !_runner )
   {
      // If this manager's analytic creates a valid abstract serial object then create a 
      // new serial run object and set it to this object's run pointer, else throw an 
      // exception. 
      if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
      {
         _runner = new SerialRun(serial,this,this);
      }
      else
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Cannot run simple analytic in chunk mode."));
         throw e;
      }
   }
}
