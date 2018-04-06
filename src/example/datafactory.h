#ifndef DATAFACTORY_H
#define DATAFACTORY_H
#include <core/AceCore.h>
#endif
//



/*!
 */
class DataFactory : public EAbstractDataFactory
{
public:
   /*!
    */
   enum Type
   {
      /*!
       */
      IntegerArrayType
      /*!
       */
      ,Total
   };
   virtual quint16 size() const override final;
   virtual QString name(quint16 type) const override final;
   virtual QString fileExtension(quint16 type) const override final;
   virtual std::unique_ptr<EAbstractData> make(quint16 type) const override final;
};



#endif
