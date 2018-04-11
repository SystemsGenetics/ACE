#ifndef ANALYTICFACTORY_H
#define ANALYTICFACTORY_H
#include <core/core.h>
//



/*!
 */
class AnalyticFactory : public EAbstractAnalyticFactory
{
public:
   /*!
    */
   enum Type
   {
      /*!
       */
      ImportIntegerArrayType
      /*!
       */
      ,MathTransformType
      /*!
       */
      ,ExportIntegerArrayType
      /*!
       */
      ,Total
   };
   virtual quint16 size() const override final;
   virtual QString name(quint16 type) const override final;
   virtual QString commandName(quint16 type) const override final;
   virtual std::unique_ptr<EAbstractAnalytic> make(quint16 type) const override final;
};



#endif
