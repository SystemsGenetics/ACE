#ifndef EABSTRACTANALYTIC_CUDA_H
#define EABSTRACTANALYTIC_CUDA_H
#include "eabstractanalytic.h"
#include "cuda_common.h"
//



/*!
 * This represents the base CUDA class for an analytic. This class is responsible
 * for initializing and creating all CUDA resources that all work threads will
 * use. The most common task that should be expected from an implementation of this
 * class is creating a CUDA program and compiling the kernels used for its
 * analytic type.
 */
class EAbstractAnalytic::CUDA : public QObject
{
   Q_OBJECT
public:
   class Worker;
public:
   /*!
    * This interface creates and returns a new CUDA worker for the implementation's
    * analytic type.
    *
    * @return Pointer to a new CUDA worker object.
    */
   virtual std::unique_ptr<EAbstractAnalytic::CUDA::Worker> makeWorker() = 0;
   /*!
    * This interface initializes all CUDA resources used by this object's
    * implementation.
    */
   virtual void initialize() = 0;
public:
   explicit CUDA(EAbstractAnalytic* parent);
};



#endif
