#ifndef INTEGERARRAY_H
#define INTEGERARRAY_H
#include <core/core.h>
//



/*!
 * This implements an integer array data object. This is a very simple example data 
 * object which simply holds an array of integer values. Because this is just an 
 * example the list of numbers is simply exposed publicly for analytic objects to 
 * use directly. 
 */
class IntegerArray : public EAbstractData
{
   Q_OBJECT
public:
   virtual qint64 dataEnd() const override final;
   virtual void readData() override final;
   virtual void writeNewData() override final;
   virtual QAbstractTableModel* model() override final;
   virtual void finish() override final;
   /*!
    * The list of integer values this data object contains. Because this is an example 
    * class all of its data can easily be stored in system memory. This however may 
    * not be the case in a real data object in which case it should be accessed 
    * through the data stream object on demand. 
    */
   QList<int> _numbers;
};



#endif
