#ifndef DUMMYDATAFACTORY_H
#define DUMMYDATAFACTORY_H
#include "eabstractdatafactory.h"



class DummyDataFactory : public EAbstractDataFactory
{
public:
   virtual quint16 getCount() override final { return 1; }
   virtual QString getName(quint16 /*type*/) override final { return QString("Dummy"); }
   virtual QString getFileExtension(quint16 /*type*/) override final
      { return QString("dmy"); }
   virtual std::unique_ptr<EAbstractData> make(quint16 type) override final;
};



#endif
