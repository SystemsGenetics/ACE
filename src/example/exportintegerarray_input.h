#ifndef EXPORTINTEGERARRAY_INPUT_H
#define EXPORTINTEGERARRAY_INPUT_H
#include "exportintegerarray.h"
//



/*!
 * This implements the abstract input of the export integer array analytic. 
 */
class ExportIntegerArray::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   /*!
    * Defines all input arguments for this analytic. 
    */
   enum Argument
   {
      /*!
       * Defines the input integer array argument. 
       */
      InputData
      /*!
       * Defines the output text file argument. 
       */
      ,OutputFile
      /*!
       * Defines the total number of all arguments. 
       */
      ,Total
   };
   explicit Input(ExportIntegerArray* parent);
   virtual int size() const override final;
   virtual EAbstractAnalytic::Input::Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   QVariant inputDataData(Role role) const;
   QVariant outputFileData(Role role) const;
   /*!
    * Pointer to the base analytic for this object. 
    */
   ExportIntegerArray* _base;
};



#endif
