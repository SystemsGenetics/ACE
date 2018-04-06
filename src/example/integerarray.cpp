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
   return static_cast<qint64>(_numbers.size())*4+4;
}






/*!
 * Implements the interface that reads in the data of an existing data object that 
 * was just opened. 
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
 * Implements the interface that is called to initialize the data object's data to 
 * a null state. 
 */
void IntegerArray::writeNewData()
{
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
 * Implements the interface is called to prepare this data object's data to get new 
 * data from an analytic and optionally allocate all required space on the 
 * underlying file. 
 *
 * @param preAllocate True if this interface should allocate all space required for 
 *                    the incoming data or false otherwise. 
 */
void IntegerArray::prepare(bool preAllocate)
{
   if ( preAllocate )
   {
      seek(0);
      allocate(4 + 4*_futureSize);
   }
}






/*!
 * Implements the interface that finalizes this data object's data after the 
 * analytic that created it has finished giving it new data. 
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
