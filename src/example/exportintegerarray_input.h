#ifndef EXPORTINTEGERARRAY_INPUT_H
#define EXPORTINTEGERARRAY_INPUT_H
#include "exportintegerarray.h"
//



/*!
 */
class ExportIntegerArray::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   /*!
    */
   enum Argument
   {
      /*!
       */
      InputData
      /*!
       */
      ,OutputFile
      /*!
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
    */
   ExportIntegerArray* _base;
};



#endif
