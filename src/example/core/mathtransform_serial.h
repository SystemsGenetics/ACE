#ifndef MATHTRANSFORM_SERIAL_H
#define MATHTRANSFORM_SERIAL_H
#include "mathtransform.h"
//



/*!
 * This implements the serial working class for its parent math transform analytic. 
 */
class MathTransform::Serial : public EAbstractAnalytic::Serial
{
   Q_OBJECT
public:
   explicit Serial(MathTransform* parent);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   /*!
    * Pointer to this serial object's parent math transform analytic. 
    */
   MathTransform* _base;
};



#endif
