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
   Manager(type),
   _index(index),
   _size(size)
{
   setupOpenCL();
   setupSerial();
   connect(_runner,&Run::finished,this,&Manager::finish);
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
   return _nextResult >= _end;
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
 */
void Chunk::start()
{
   setupFile();
   setupIndexes();
   QTimer::singleShot(0,this,&Chunk::process);
}






/*!
 */
void Chunk::process()
{
   while ( _nextWork < _end )
   {
      _runner->addWork(makeWork(_nextWork++));
   }
}






/*!
 */
void Chunk::setupFile()
{
   Settings& settings {Settings::instance()};
   _fileName = settings.chunkDir().append("/")
                                  .append(settings.chunkPrefix())
                                  .append(QString::number(_index))
                                  .append(".")
                                  .append(settings.chunkExtension());
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
   _fileName = QFileInfo(_fileName).fileName();
   _stream = new QDataStream(_file);
}






/*!
 */
void Chunk::setupIndexes()
{
   int chunkSize {analytic()->size()/_size + ( _size%analytic()->size() ? 1 : 0)};
   _nextWork = _nextResult = _index*chunkSize;
   _end = (_index + 1)*chunkSize;
   _end = _end > analytic()->size() ? analytic()->size() : _end;
}






/*!
 */
void Chunk::setupOpenCL()
{
   int index {_index};
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
