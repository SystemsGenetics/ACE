#ifndef EABSTRACTANALYTIC_OPENCL_WORKER_H
#define EABSTRACTANALYTIC_OPENCL_WORKER_H
#include "eabstractanalytic_opencl.h"
//



/*!
 * This represents a single OpenCL worker for an analytic type that processes work 
 * blocks and returns result blocks using OpenCL acceleration. Multiple instances 
 * of this class are used in parallel on separate threads so thread safety must 
 * always be considered for any code used by the the methods of any implementation 
 * of this class. 
 */
class EAbstractAnalytic::OpenCL::Worker : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface reads in the given work block, executes the algorithms necessary 
    * to produce its results using OpenCL acceleration, and saves those results in a 
    * new results block whose pointer is returned. 
    *
    * @param block  
    *
    * @return Pointer to results block produced from the given work block. 
    */
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) = 0;
protected:
   static int threadSize();
};



#endif
