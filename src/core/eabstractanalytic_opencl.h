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
   class Block;
   explicit OpenCL(EAbstractAnalytic* parent);
   /*!
    */
   virtual int stepSize() const = 0;
   /*!
    */
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Block> makeBlock() const = 0;
   /*!
    *
    * @param context  
    */
   virtual void initialize(::OpenCL::Context* context) = 0;
};



#endif
