#ifndef EABSTRACTANALYTIC_OPENCL_H
#define EABSTRACTANALYTIC_OPENCL_H
#include "eabstractanalytic.h"
#include "opencl.h"
//



/*!
 * This represents the base OpenCL class for an analytic. This class is responsible 
 * for initializing and creating all OpenCL resources that all work threads will 
 * use. The most common task that should be expected from an implementation of this 
 * class is creating an OpenCL program and compiling the kernels used for its 
 * analytic type. 
 */
class EAbstractAnalytic::OpenCL : public QObject
{
   Q_OBJECT
public:
   class Worker;
   explicit OpenCL(EAbstractAnalytic* parent);
   /*!
    * This interface creates and returns a new OpenCL worker for the implementation's 
    * analytic type. 
    *
    * @return Pointer to a new OpenCL worker object. 
    */
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() const = 0;
   /*!
    * This interface initializes all OpenCL resources used by this object's 
    * implementation. 
    *
    * @param context  
    */
   virtual void initialize(::OpenCL::Context* context) = 0;
};



#endif
