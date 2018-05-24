#include "ace_analytic_merge.h"
#include <QFile>
#include <QDataStream>
#include "ace_settings.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new merge manager with the given analytic type and the given chunk 
 * size. 
 *
 * @param type The analytic type that is used by this manager. 
 *
 * @param size The chunk size of the chunk runs this manager will merge to finish 
 *             the analytic. 
 */
Merge::Merge(quint16 type, int size):
   AbstractManager(type),
   _size(size)
{}






/*!
 * Implements the interface that tests if this abstract input is finished and 
 * received all result blocks for its analytic. 
 *
 * @return True if this abstract input is finished or false otherwise. 
 */
bool Merge::isFinished() const
{
   return _nextResult >= analytic()->size();
}






/*!
 * Implements the interface that returns the next expected result block index to 
 * maintain order of result blocks. 
 *
 * @return The next expected result block index to maintain order. 
 */
int Merge::index() const
{
   return _nextResult;
}






/*!
 * Implements the interface that is called to save the given result block to the 
 * underlying analytic and it can be assumed that the index order is maintained 
 * from least to greatest. 
 *
 * @param result The result block that is processed by this manager's abstract 
 *               analytic. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Write the given result block to this manager's analytic and increment the 
 *    next result index. If this abstract input is finished with all results then 
 *    emit the done signal and call this manager's finish slot. 
 */
void Merge::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   AbstractManager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
      finish();
   }
}






/*!
 * Implements the interface that is called once to begin the analytic run for this 
 * manager after all argument input has been set. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Calculate the chunk size for this merge and schedule this object's process 
 *    slot to be called. 
 */
void Merge::start()
{
   _chunkSize  = analytic()->size()/_size + (analytic()->size()%_size ? 1 : 0);
   QTimer::singleShot(0,this,&Merge::process);
}






/*!
 * Called to process all temporary chunk files, merging them all together by 
 * sorting all result blocks and processing them with this manager's analytic. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all temporary chunk files, reading in each one. 
 */
void Merge::process()
{
   for (int i = 0; i < _size ;++i)
   {
      readChunk(i);
   }
}






/*!
 * Reads in all result blocks from a chunk file produced by the given chunk index, 
 * processing them in order with this manager's analytic. If an error occurs with 
 * opening the file, reading , or the chunk file did not contain the expected 
 * number of result blocks then an exception is thrown. 
 *
 * @param index The chunk index whose temporary binary file of result blocks is 
 *              read in. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Determine the range of indexes this chunk file should contain. 
 *
 * 2. Determine the file path for the chunk file with the given index and open it 
 *    as read only. If opening fails then throw an exception, else go to the next 
 *    step. 
 *
 * 3. Create a qt data stream with the open file and read in the total number of 
 *    expected result blocks. 
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
   {
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
 * Reads in a single result block from the given qt data stream, saving it to this 
 * abstract input's hopper for sorting and processing. 
 *
 * @param stream  
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the next result block in byte array format from the data stream. If 
 *    any read error occurs then throw an exception, else go to the next step. 
 *
 * 2. Create a blank result block from this manager's analytic. If creation of the 
 *    result block fails then throw an exception, else go to the next step. 
 *
 * 3. Load the blank result block with the byte array data and save it to this 
 *    abstract input for sorting and processing. 
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
