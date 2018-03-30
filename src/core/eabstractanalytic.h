#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QObject>
//



/*!
 * This represents an analytic process for the ACE system. Because of the 
 * complexity of an analytic its tasks are divided into multiple sub classes. This 
 * class is responsible for making all other sub classes, making blocks of work, 
 * reading blocks of results, initialization, and finishing the analytic. An 
 * implementation of this class must do all of these things. The work of an 
 * analytic is designed to be split up into individual blocks of work. This class 
 * creates these blocks of work and then reads in the corresponding blocks 
 * containing the results of the work. 
 */
class EAbstractAnalytic : public QObject
{
   Q_OBJECT
public:
   class Input;
   class Serial;
   class OpenCL;
   /*!
    * This interface returns the total number of blocks this analytic must process as 
    * blocks of work. 
    *
    * @return Total number of blocks that this analytic must work on. 
    */
   virtual int size() const = 0;
   /*!
    * This interface makes a new block of work from the given block index. The order 
    * of indexes called will always be in order starting at 0. 
    *
    * @param index Index used to make the block of work. 
    *
    * @return Block of work encoded into a byte array. 
    */
   virtual QByteArray makeBlock(int index) const = 0;
   /*!
    * This interface reads in a block of results made from a block of work with the 
    * corresponding index. The order of indexes called will always be in order 
    * starting at 0. 
    *
    * @param index  
    *
    * @param results  
    */
   virtual void readBlock(int index, const QByteArray& results) = 0;
   /*!
    * This interface makes a new input object and returns its pointer. 
    *
    * @return Pointer to new input object. 
    */
   virtual EAbstractAnalytic::Input* makeInput() = 0;
   /*!
    * This interface makes a new serial object and returns its pointer. 
    *
    * @return Pointer to new serial object. 
    */
   virtual EAbstractAnalytic::Serial* makeSerial() const = 0;
   virtual EAbstractAnalytic::OpenCL* makeOpenCL() const;
   virtual void initialize();
   virtual void finish();
};



#endif
