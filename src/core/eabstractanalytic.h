#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <memory>
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
   class Block;
   class Input;
   class Serial;
   class OpenCL;
   /*!
    * This interface returns the total number of blocks this analytic must process as 
    * steps or blocks of work. If this analytic produces no work blocks then this 
    * means the number of times the read block interface is called. 
    *
    * @return Total number of blocks or steps that this analytic must work on. 
    */
   virtual int size() const = 0;
   /*!
    * This interface processes the given index with a possible block of results if 
    * this analytic produces work blocks. The order of indexes called will always be 
    * in order starting at 0. If null is given that means this analytic does not 
    * produce work blocks. 
    *
    * @param index Index of the given block that is read in. 
    *
    * @param results Pointer to the block of results that is read in. 
    */
   virtual void process(int index, const EAbstractAnalytic::Block* results) = 0;
   /*!
    * This interface makes a new input object and returns its pointer. The returned 
    * input object must have this abstract analytic set as its parent. 
    *
    * @return Pointer to new input object. 
    */
   virtual EAbstractAnalytic::Input* makeInput() = 0;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeBlock(int index) const;
   virtual EAbstractAnalytic::Serial* makeSerial();
   virtual EAbstractAnalytic::OpenCL* makeOpenCL();
   virtual void initialize();
   virtual void finish();
};



#endif
