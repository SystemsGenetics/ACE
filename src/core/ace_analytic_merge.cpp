#include "ace_analytic_merge.h"
#include <QFile>
#include <QDataStream>
#include "ace_settings.h"
#include "eabstractanalyticblock.h"
#include "edebug.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Implements the interface that tests if this abstract input is finished and 
 * received all result blocks for its analytic. 
 *
 * @return True if this abstract input is finished or false otherwise. 
 */
bool Merge::isFinished() const
{
   EDEBUG_FUNC(this)

   return _nextResult >= analytic()->size();
}






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
{
   EDEBUG_FUNC(this,type,size)
}






/*!
 * Implements the interface that returns the next expected result block index to 
 * maintain order of result blocks. 
 *
 * @return The next expected result block index to maintain order. 
 */
int Merge::index() const
{
   EDEBUG_FUNC(this)

   return _nextResult;
}






/*!
 * Implements the interface that is called to save the given result block to the 
 * underlying analytic and it can be assumed that the index order is maintained 
 * from least to greatest. 
 *
 * @param result The result block that is processed by this manager's abstract 
 *               analytic. 
 */
void Merge::writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result)
{
   EDEBUG_FUNC(this,result.get())

   // Write the given result block to this manager's analytic and increment the next 
   // result index. If this abstract input is finished with all results then emit the 
   // done signal and call this manager's finish slot. 
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
 */
void Merge::start()
{
   EDEBUG_FUNC(this);

   analytic()->initializeOutputs();

   // Calculate the chunk size for this merge and schedule this object's process slot 
   // to be called. 
   _chunkSize  = analytic()->size()/_size + (analytic()->size()%_size ? 1 : 0);
   QTimer::singleShot(0,this,&Merge::process);
}






/*!
 * Called to process all temporary chunk files, merging them all together by 
 * sorting all result blocks and processing them with this manager's analytic. 
 */
void Merge::process()
{
   EDEBUG_FUNC(this)

   // Iterate through all temporary chunk files, reading in each one. 
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
 */
void Merge::readChunk(int index)
{
   EDEBUG_FUNC(this,index)

   // Determine the range of indexes this chunk file should contain. 
   int nextWork {index*_chunkSize};
   int end {(index + 1)*_chunkSize};
   if ( end > analytic()->size() )
   {
      end = analytic()->size();
   }

   // Determine the file path for the chunk file with the given index and open it as 
   // read only. If opening fails then throw an exception, else go to the next step. 
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

   // Create a qt data stream with the open file and read in the total number of 
   // expected result blocks. 
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
 */
void Merge::readBlock(QDataStream& stream)
{
   EDEBUG_FUNC(this,&stream)

   // Read in the next result block in byte array format from the data stream. If any 
   // read error occurs then throw an exception, else go to the next step. 
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

   // Create a blank result block from this manager's analytic. If creation of the 
   // result block fails then throw an exception, else go to the next step. 
   unique_ptr<EAbstractAnalyticBlock> result {analytic()->makeResult()};
   if ( !result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null result block pointer."));
      throw e;
   }

   // Load the blank result block with the byte array data and save it to this 
   // abstract input for sorting and processing. 
   result->fromBytes(data);
   saveResult(std::move(result));
}
