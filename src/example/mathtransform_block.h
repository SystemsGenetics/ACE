#ifndef MATHTRANSFORM_BLOCK_H
#define MATHTRANSFORM_BLOCK_H
#include "mathtransform.h"
//



/*!
 */
class MathTransform::Block : public EAbstractAnalytic::Block
{
   Q_OBJECT
public:
   /*!
    */
   explicit Block() = default;
   explicit Block(int index);
   explicit Block(int index, qint32 number);
   /*!
    */
   qint32 _number;
protected:
   virtual void write(QDataStream& stream) const override final;
   virtual void read(QDataStream& stream) override final;
};



#endif
