#ifndef EABSTRACTANALYTIC_OPENCL_BLOCK_H
#define EABSTRACTANALYTIC_OPENCL_BLOCK_H
#include "eabstractanalytic_opencl.h"
//



/*!
 */
class EAbstractAnalytic::OpenCL::Block : public QObject
{
   Q_OBJECT
public:
   /*!
    *
    * @param block  
    */
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) = 0;
};



#endif
