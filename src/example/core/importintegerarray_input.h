#ifndef IMPORTINTEGERARRAY_INPUT_H
#define IMPORTINTEGERARRAY_INPUT_H
#include "importintegerarray.h"
//



/*!
 * This implements the abstract input of the import integer array analytic. 
 */
class ImportIntegerArray::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   /*!
    * Defines all input arguments for this analytic. 
    */
   enum Argument
   {
      /*!
       * Defines the input file argument. 
       */
      InputFile
      /*!
       * Defines the output integer array argument. 
       */
      ,OutputData
      /*!
       * Defines the total number of all arguments. 
       */
      ,Total
   };
   explicit Input(ImportIntegerArray* parent);
   virtual int size() const override final;
   virtual EAbstractAnalytic::Input::Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   QVariant inputFileData(Role role) const;
   QVariant outputDataData(Role role) const;
   /*!
    * Pointer to the base analytic for this object. 
    */
   ImportIntegerArray* _base;
};



#endif
