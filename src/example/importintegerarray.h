#ifndef IMPORTINTEGERARRAY_H
#define IMPORTINTEGERARRAY_H
#include <core/core.h>
#include "global.h"
//



/*!
 */
class ImportIntegerArray : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   virtual ~ImportIntegerArray() override final;
   virtual int size() const override final;
   virtual void process(int index, const EAbstractAnalytic::Block* results) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual void initialize();
private:
   /*!
    */
   constexpr static const int _incrementSize {1024};
   /*!
    */
   QFile* _in {nullptr};
   /*!
    */
   QTextStream* _stream {nullptr};
   /*!
    */
   IntegerArray* _out {nullptr};
};



#endif
