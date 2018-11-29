#ifndef ANALYTICFACTORY_H
#define ANALYTICFACTORY_H
#include <core/core.h>
//



/*!
 * This implements the ACE analytic factory for producing new analytic objects and 
 * giving basic information about all available analytic types. 
 */
class AnalyticFactory : public EAbstractAnalyticFactory
{
public:
   /*!
    * Defines all available analytic types this program implements along with the 
    * total size. 
    */
   enum Type
   {
      /*!
       * Defines the import dataframe analytic type. 
       */
      ImportDataFrameType
      /*!
       * Defines the export dataframe analytic type. 
       */
      ,ExportDataFrameType
      /*!
       * Defines the math transform analytic type. 
       */
      ,MathTransformType
      /*!
       * Defines the total number of analytic types that exist. 
       */
      ,Total
   };
   virtual quint16 size() const override final;
   virtual QString name(quint16 type) const override final;
   virtual QString commandName(quint16 type) const override final;
   virtual std::unique_ptr<EAbstractAnalytic> make(quint16 type) const override final;
};



#endif
