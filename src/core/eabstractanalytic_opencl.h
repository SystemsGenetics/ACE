#ifndef EABSTRACTANALYTIC_OPENCL_H
#define EABSTRACTANALYTIC_OPENCL_H
#include "eabstractanalytic.h"
#include "opencl.h"
//



/*!
 */
class EAbstractAnalytic::OpenCL : public QObject
{
   Q_OBJECT
public:
   class Worker;
   explicit OpenCL(EAbstractAnalytic* parent);
   /*!
    */
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() const = 0;
   /*!
    *
    * @param context  
    */
   virtual void initialize(::OpenCL::Context* context) = 0;
};



#endif
