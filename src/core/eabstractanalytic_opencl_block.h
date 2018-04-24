#ifndef EABSTRACTANALYTIC_OPENCL_BLOCK_H
#define EABSTRACTANALYTIC_OPENCL_BLOCK_H
#include "eabstractanalytic_opencl.h"
//



/*!
 */
class EAbstractAnalytic::OpenCL::Block : public QObject
{
   Q_OBJECT
protected:
   /*!
    *
    * @param block  
    */
   virtual void start(const EAbstractAnalytic::Block* block) = 0;
   /*!
    *
    * @param step  
    */
   virtual void execute(int step) = 0;
   /*!
    */
   virtual std::unique_ptr<EAbstractAnalytic::Block> finish() = 0;
};



#endif
