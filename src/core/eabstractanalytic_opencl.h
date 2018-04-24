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
   explicit OpenCL(std::unique_ptr<::OpenCL::Context>&& context, EAbstractAnalytic* parent);
   /*!
    */
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Block> makeBlock() const = 0;
protected:
   ::OpenCL::Context& context();
private:
   /*!
    */
   ::OpenCL::Context* _context;
};



#endif
