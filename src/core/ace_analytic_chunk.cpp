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
 *
 * @param type  
 *
 * @param index  
 *
 * @param size  
 */
Chunk::Chunk(quint16 type, int index, int size):
   Manager(type)
{
   setupFile(index);
   setupIndexes(index,size);
   setupOpenCL(index);
   setupSerial();
}






/*!
 */
Chunk::~Chunk()
{
   delete _stream;
}






/*!
 */
bool Chunk::isFinished() const
{
   return _nextResult >= _last;
}






/*!
 *
 * @param path  
 */
QFile* Chunk::addOutputFile(const QString& path)
{
   Q_UNUSED(path)
   return nullptr;
}






/*!
 *
 * @param path  
 *
 * @param type  
 *
 * @param system  
 */
Ace::DataObject* Chunk::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   Q_UNUSED(path)
   Q_UNUSED(type)
   Q_UNUSED(system)
   return nullptr;
}






/*!
 *
 * @param result  
 */
void Chunk::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   QByteArray data {result->toBytes()};
   *_stream << data.size();
   _stream->writeBytes(data.data(),data.size());
   ++_nextResult;
   if ( isFinished() )
   {
      *_stream << (int)-1;
   }
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
 */
void Chunk::start()
{
   QTimer::singleShot(0,this,&Chunk::process);
}






/*!
 */
void Chunk::process()
{
   while ( _nextWork < _last )
   {
      _runner->addWork(makeWork(_nextWork++));
   }
}






/*!
 *
 * @param index  
 */
void Chunk::setupFile(int index)
{
   Settings& settings {Settings::instance()};
   _fileName = settings.chunkDir().append("/")
                                  .append(settings.chunkPrefix())
                                  .append(QString::number(index))
                                  .append(".")
                                  .append(settings.chunkExtension());
   _file = new QFile(_fileName,this);
   if ( !_file->open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Open Error"));
      e.setDetails(tr("Failed opening to temporary chunk file %1: %2")
                   .arg(_fileName)
                   .arg(_file->errorString()));
      throw e;
   }
   _fileName = QFileInfo(_fileName).fileName();
   _stream = new QDataStream(_file);
}






/*!
 *
 * @param index  
 *
 * @param size  
 */
void Chunk::setupIndexes(int index, int size)
{
   int blockSize {analytic()->size()/size};
   if ( analytic()->size() )
   {
      ++blockSize;
   }
   _nextWork = index*blockSize;
   _last = (index + 1)*blockSize;
   if ( _last > analytic()->size() )
   {
      _last = analytic()->size();
   }
}






/*!
 *
 * @param index  
 */
void Chunk::setupOpenCL(int index)
{
   if ( Settings::instance().openCLDevicePointer() )
   {
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
      if ( device )
      {
         _runner = new OpenCLRun(analytic()->makeOpenCL(),device,this,this);
      }
   }
}






/*!
 */
void Chunk::setupSerial()
{
   if ( !_runner )
   {
      if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
      {
         _runner = new SerialRun(serial,this,this);
      }
      else
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Cannot run simple analytic in MPI mode."));
         throw e;
      }
   }
}
