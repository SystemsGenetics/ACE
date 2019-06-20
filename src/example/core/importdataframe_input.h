#ifndef IMPORTDATAFRAME_INPUT_H
#define IMPORTDATAFRAME_INPUT_H
#include "importdataframe.h"
//



/*!
 * This implements the abstract input of the import dataframe analytic.
 */
class ImportDataFrame::Input : public EAbstractAnalyticInput
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
       * Defines the output dataframe argument.
       */
      ,OutputData
      /*!
       * Defines the NAN token argument.
       */
      ,NANToken
      /*!
       * Defines the total number of all arguments.
       */
      ,Total
   };
   explicit Input(ImportDataFrame* parent);
   virtual int size() const override final;
   virtual Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   /*!
    * Pointer to the base analytic for this object.
    */
   ImportDataFrame* _base;
};



#endif
