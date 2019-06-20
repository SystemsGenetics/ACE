#ifndef EXPORTDATAFRAME_INPUT_H
#define EXPORTDATAFRAME_INPUT_H
#include "exportdataframe.h"
//



/*!
 * This class implements the abstract input of the export dataframe analytic.
 */
class ExportDataFrame::Input : public EAbstractAnalyticInput
{
   Q_OBJECT
public:
   /*!
    * Defines all input arguments for this analytic.
    */
   enum Argument
   {
      /*!
       * Defines the input dataframe argument.
       */
      InputData
      /*!
       * Defines the output text file argument.
       */
      ,OutputFile
      /*!
       * Defines the NAN token argument.
       */
      ,NANToken
      /*!
       * Defines the total number of all arguments.
       */
      ,Total
   };
   explicit Input(ExportDataFrame* parent);
   virtual int size() const override final;
   virtual EAbstractAnalyticInput::Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   /*!
    * Pointer to the base analytic for this object.
    */
   ExportDataFrame* _base;
};



#endif
