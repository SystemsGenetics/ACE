#include "eabstractanalytic.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "edebug.h"






/*!
 * Extracts the index from a byte array created from a to bytes method call of
 * this class. This is used to extract special codes used in MPI.
 *
 *
 * @return Index or special code of block byte array.
 */
int EAbstractAnalytic::Block::extractIndex(const QByteArray& data)
{
   // Add the debug header.
   EDEBUG_FUNC(data);

   // Read the index from the byte array, making sure it worked.
   int ret;
   QDataStream stream(data);
   stream >> ret;
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }

   // Return the extracted index.
   return ret;
}






/*!
 * Constructs a new block with the given index.
 *
 * @param index The index for this new block.
 */
EAbstractAnalytic::Block::Block(int index)
   :
   _index(index)
{
   EDEBUG_FUNC(this,index);
}






/*!
 * Returns the index of this block.
 *
 * @return Index of this block.
 */
int EAbstractAnalytic::Block::index() const
{
   EDEBUG_FUNC(this);
   return _index;
}






/*!
 * Returns a byte array that contains all this blocks information. This includes
 * any implementation's data.
 *
 * @return Byte array containing all data of this block.
 */
QByteArray EAbstractAnalytic::Block::toBytes() const
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create a new byte array and a data stream for writing to it.
   QByteArray ret;
   QDataStream stream(&ret,QIODevice::WriteOnly);

   // Write this block's index and and then this block's data by calling the write
   // interface.
   stream << _index;
   write(stream);

   // Make sure all writing to the byte array was successful.
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }

   // Return the byte array containing this block's index and data.
   return ret;
}






/*!
 * Read in new data from the given byte array for this block, overwriting any
 * previous data, including the data of this block's implementation.
 *
 * @param data The byte array that is read in and should contain data for an
 *             index and data of a block.
 */
void EAbstractAnalytic::Block::fromBytes(const QByteArray& data)
{
   // Add the debug header.
   EDEBUG_FUNC(this,data);

   // Create a data stream for reading the given byte array.
   QDataStream stream(data);

   // Read in this block's index and then its data by calling the read interface,
   // making sure all reading was successful.
   stream >> _index;
   read(stream);
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }
}






/*!
 * This interface writes out its data to the given qt data stream. The default
 * implementation does nothing.
 *
 * @param stream The data stream that is used to write out this block's
 *               implementation data.
 */
void EAbstractAnalytic::Block::write(QDataStream& stream) const
{
   EDEBUG_FUNC(this,&stream);
   Q_UNUSED(stream)
}






/*!
 * This interface reads in new data from the given byte array, overwriting any
 * data it previously contained. The default implementation does nothing.
 *
 * @param stream The data stream that is used to read in this block's
 *               implementation data.
 */
void EAbstractAnalytic::Block::read(QDataStream& stream)
{
   EDEBUG_FUNC(this,&stream);
   Q_UNUSED(stream)
}






/*!
 * Constructs a new input object with the given analytic as its parent.
 *
 * @param parent The parent analytic object for this new input object.
 */
EAbstractAnalytic::Input::Input(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}






/*!
 * Constructs a new serial object with the given analytic as its parent.
 *
 * @param parent The parent analytic object for this new serial object.
 */
EAbstractAnalytic::Serial::Serial(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}






/*!
 * Returns the total number of threads ACE uses to run OpenCL workers in
 * parallel.
 *
 * @return Total number of OpenCL worker threads.
 */
int EAbstractAnalytic::OpenCL::Worker::threadSize()
{
   EDEBUG_FUNC();
   return Ace::Settings::instance().threadSize();
}






/*!
 * Constructs a new abstract OpenCL object with the given parent.
 *
 * @param parent The parent analytic object for this new OpenCL object.
 */
EAbstractAnalytic::OpenCL::OpenCL(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}






/*!
 * Returns the total number of threads ACE uses to run CUDA workers in parallel.
 *
 * @return Total number of CUDA worker threads.
 */
int EAbstractAnalytic::CUDA::Worker::threadSize()
{
   EDEBUG_FUNC();
   return Ace::Settings::instance().threadSize();
}






/*!
 * Constructs a new abstract CUDA object with the given parent.
 *
 * @param parent The parent analytic object for this new CUDA object.
 */
EAbstractAnalytic::CUDA::CUDA(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}






/*!
 * This interface creates and returns a work block for this analytic with the
 * given index. The default implementation throws an exception because this
 * should never be called if it is not implemented by an analytic.
 *
 * @param index Index used to make the block of work.
 *
 * @return Pointer new work block with the given index.
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork(int index) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // Throw an exception.
   Q_UNUSED(index)
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface creates an empty and uninitialized work block. This is used by
 * MPI and chunk runs for reading in work blocks from byte arrays. The default
 * implementation throws an exception because this should never be called if it
 * is not implemented by an analytic.
 *
 * @return Pointer uninitialized work block.
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork() const
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Throw an exception.
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface creates an empty and uninitialized result block. This is used
 * by MPI and chunk runs for reading in work blocks from byte arrays. The
 * default implementation throws an exception because this should never be
 * called if it is not implemented by an analytic.
 *
 * @return Pointer uninitialized result block.
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeResult() const
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Throw an exception.
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface makes a new serial object and returns its pointer. If this
 * analytic type does not support serial then a null pointer is returned. The
 * default implementation returns a null pointer.
 *
 * @return Pointer to new serial object or null if no serial support.
 */
EAbstractAnalytic::Serial* EAbstractAnalytic::makeSerial()
{
   EDEBUG_FUNC(this);
   return nullptr;
}






/*!
 * This interface makes a new OpenCL object and returns its pointer. If this
 * analytic type does not support OpenCL then a null pointer is returned. The
 * default implementation returns a null pointer.
 *
 * @return Pointer to new OpenCL object or null if this analytic does not
 *         support OpenCL.
 */
EAbstractAnalytic::OpenCL* EAbstractAnalytic::makeOpenCL()
{
   EDEBUG_FUNC(this);
   return nullptr;
}






/*!
 * This interface makes a new CUDA object and returns its pointer. If this
 * analytic type does not support CUDA then a null pointer is returned. The
 * default implementation returns a null pointer.
 *
 * @return Pointer to new CUDA object or null if this analytic does not support
 *         CUDA.
 */
EAbstractAnalytic::CUDA* EAbstractAnalytic::makeCUDA()
{
   EDEBUG_FUNC(this);
   return nullptr;
}






/*!
 * This interface initializes this analytic. This is called only once before any
 * other interface is called for this analytic. The default implementation does
 * nothing.
 */
void EAbstractAnalytic::initialize()
{
   EDEBUG_FUNC(this);
}






/*!
 * This interface initializes any outputs for this analytic. This is called only
 * once before any other interface is called for this analytic. This is only
 * called for analytic objects that will process output result blocks. The
 * default implementation does nothing.
 */
void EAbstractAnalytic::initializeOutputs()
{
   EDEBUG_FUNC(this);
}






/*!
 * This interface is called to finish this analytic. This is called at the very
 * end after every other interface call. The default implementation does
 * nothing.
 */
void EAbstractAnalytic::finish()
{
   EDEBUG_FUNC(this);
}






/*!
 * Tests if this is the master node in an MPI run. If this is not an MPI run
 * then this will also return true.
 *
 * @return True if this is the master node of an MPI run or this is not running
 *         in MPI mode, otherwise false is returned.
 */
bool EAbstractAnalytic::isMaster()
{
   EDEBUG_FUNC();
   return Ace::QMPI::instance().isMaster();
}

