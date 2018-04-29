#include "ace_analytic_merge.h"
#include <QFile>
#include <QDataStream>
#include "ace_settings.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param type  
 *
 * @param size  
 */
Merge::Merge(quint16 type, int size):
   Manager(type),
   _size(size)
{}






/*!
 */
bool Merge::isFinished() const
{
   return _nextResult >= analytic()->size();
}






/*!
 */
int Merge::index() const
{
   return _nextResult;
}






/*!
 *
 * @param result  
 */
void Merge::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   Manager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
      finish();
   }
}






/*!
 */
void Merge::start()
{
   _chunkSize  = analytic()->size()/_size + (_size%analytic()->size() ? 1 : 0);
   QTimer::singleShot(0,this,&Merge::process);
}






/*!
 */
void Merge::process()
{
   for (int i = 0; i < _size ;++i)
   {
      readChunk(i);
   }
}






/*!
 *
 * @param index  
 */
void Merge::readChunk(int index)
{
   int nextWork {index*_chunkSize};
   int end {(index + 1)*_chunkSize};
   if ( end > analytic()->size() )
   {
      end = analytic()->size();
   }
   Settings& settings {Settings::instance()};
   QString path
   {//
      settings.chunkDir().append("/")
                         .append(settings.chunkPrefix())
                         .append(QString::number(index))
                         .append(".")
                         .append(settings.chunkExtension())
   };
   QFile file(path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Open Error"));
      e.setDetails(tr("Failed opening temporary chunk file %1: %2")
                   .arg(path)
                   .arg(file.errorString()));
      throw e;
   }
   QDataStream stream(&file);
   while ( nextWork++ < end )
   {
      readBlock(stream);
   }
}






/*!
 *
 * @param stream  
 */
void Merge::readBlock(QDataStream& stream)
{
   int size;
   stream >> size;
   QByteArray data {stream.device()->read(size)};
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading temporary chunk file."));
      throw e;
   }
   unique_ptr<EAbstractAnalytic::Block> result {analytic()->makeResult()};
   if ( !result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null result block pointer."));
      throw e;
   }
   result->fromBytes(data);
   saveResult(std::move(result));
}
