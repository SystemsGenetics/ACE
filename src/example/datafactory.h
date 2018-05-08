#ifndef DATAFACTORY_H
#define DATAFACTORY_H
#include <core/core.h>
//



/*!
 * This implements the ACE data factory for producing new data objects and giving 
 * basic information about all available data types. 
 */
class DataFactory : public EAbstractDataFactory
{
public:
   /*!
    * Defines all available data types this program implements along with the total 
    * size. 
    */
   enum Type
   {
      /*!
       * Defines the integer array data type. 
       */
      IntegerArrayType
      /*!
       * Defines the total number of data types that exist. 
       */
      ,Total
   };
   virtual quint16 size() const override final;
   virtual QString name(quint16 type) const override final;
   virtual QString fileExtension(quint16 type) const override final;
   virtual std::unique_ptr<EAbstractData> make(quint16 type) const override final;
};



#endif
