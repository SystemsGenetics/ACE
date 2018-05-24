#include "integerarray.h"
//






/*!
 * Implements the interface that returns the index within this data object after 
 * the end of where the data is stored. 
 *
 * @return Index of the first byte in the data object after the data section ends. 
 */
qint64 IntegerArray::dataEnd() const
{
   return static_cast<qint64>(_numbers.size())*4 + 4;
}






/*!
 * Implements the interface that reads in the data of an existing data object that 
 * was just opened. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Seek to the beginning of the data and read in the amount of integers it 
 *    contains. 
 *
 * 2. Read in all integer values the data contains, adding them to this object's 
 *    list of integer values. 
 */
void IntegerArray::readData()
{
   seek(0);
   quint32 amount {0};
   stream() >> amount;
   while ( amount )
   {
      qint32 value;
      stream() >> value;
      _numbers << value;
      --amount;
   }
}






/*!
 * Implements the interface that is called to initialize this data object's data to 
 * a null state. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Seek to the beginning of data and write out the amount of integers to be 0. 
 */
void IntegerArray::writeNewData()
{
   EMetadata ok {EMetadata::Object};
   ok.toObject().insert("FOO",EMetadata(QString("BAR")));
   setMeta(ok);
   seek(0);
   stream() << (quint32)0;
}






/*!
 * Implements the interface that returns a qt table model that represents the data 
 * of this data object as a table. 
 *
 * @return Pointer to model that represents the data of this data object. 
 */
QAbstractTableModel* IntegerArray::model()
{
   return nullptr;
}






/*!
 * Implements the interface that finalizes this data object's data after the 
 * analytic that created it has finished giving it new data. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Seek to the beginning of the data and write out the amount of integers it 
 *    will contain. 
 *
 * 2. Iterate through this object's list of integers, writing out each one to the 
 *    data. 
 */
void IntegerArray::finish()
{
   seek(0);
   stream() << static_cast<quint32>(_numbers.size());
   for (auto number: qAsConst(_numbers))
   {
      stream() << static_cast<qint32>(number);
   }
}
