#ifndef IMPORTINTEGERARRAY_INPUT_H
#define IMPORTINTEGERARRAY_INPUT_H
#include "importintegerarray.h"
//



/*!
 */
class ImportIntegerArray::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   /*!
    */
   enum Argument
   {
      /*!
       */
      InputFile
      /*!
       */
      ,OutputData
      /*!
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
    */
   ImportIntegerArray* _base;
};



#endif
