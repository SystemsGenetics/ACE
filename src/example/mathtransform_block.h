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
   explicit Block(qint32 number);
   virtual QByteArray toBytes() const override final;
   virtual void fromBytes(const QByteArray& data) override final;
   /*!
    */
   qint32 _number;
};



#endif
