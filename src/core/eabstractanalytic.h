#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <memory>
#include <QObject>
#include "global.h"



/*!
 * This represents an analytic process for the ACE system. Because of the
 * complexity of an analytic its tasks are divided into multiple sub classes.
 * This class is responsible for making all other sub classes, making blocks of
 * work, reading blocks of results, initialization, and finishing the analytic.
 * An implementation of this class must do all of these things. The work of an
 * analytic is designed to be split up into individual blocks of work. This
 * class creates these blocks of work and then reads in the corresponding blocks
 * containing the results of the work. To accommodate very simple analytic types
 * an implementation of this class can choose to not produce work blocks which
 * causes the process interface to be called with empty generic result blocks. A
 * simple analytic only supports single process serial mode for operation and
 * does not override the serial or OpenCL methods. Simple analytic types also do
 * not need to implement any interfaces for making blocks since they are not
 * used.
 */
class EAbstractAnalytic : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface returns the total number of blocks this analytic must process
    * as steps or blocks of work. If this analytic produces no work blocks then
    * this means the number of times the process interface is called.
    *
    * @return Total number of blocks or steps that this analytic must work on.
    */
   virtual int size() const = 0;
   /*!
    * This interface processes the given result block. The order of indexes called
    * will always be in order starting at 0. If this analytic does not produce work
    * blocks then the given block is a generic and empty block which only contains
    * the index.
    *
    * @param result Pointer to result block that is processed.
    */
   virtual void process(const EAbstractAnalyticBlock* result) = 0;
   /*!
    * This interface makes a new input object and returns its pointer. The returned
    * input object must have this abstract analytic set as its parent.
    *
    * @return Pointer to new input object.
    */
   virtual EAbstractAnalyticInput* makeInput() = 0;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeWork(int index) const;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeWork() const;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeResult() const;
   virtual EAbstractAnalyticSerial* makeSerial();
   virtual EAbstractAnalyticOpenCL* makeOpenCL();
   virtual EAbstractAnalyticCUDA* makeCUDA();
   virtual void initialize();
   virtual void initializeOutputs();
   virtual void finish();
protected:
   static bool isMaster();
};

#endif
