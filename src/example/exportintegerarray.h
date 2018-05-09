#ifndef EXPORTINTEGERARRAY_H
#define EXPORTINTEGERARRAY_H
#include <core/core.h>
#include "global.h"
//



/*!
 * This implements an export integer array analytic. This reads in an integer array 
 * data object and outputs all integers it contained to a text file separated by 
 * spaces. This is a simple analytic which means it does not produce blocks and 
 * simply uses the process interface for incrementally processing its task. 
 */
class ExportIntegerArray : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   virtual ~ExportIntegerArray() override final;
   virtual int size() const override final;
   virtual void process(const EAbstractAnalytic::Block* result) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual void initialize();
private:
   /*!
    * Pointer to the input integer array. 
    */
   IntegerArray* _in {nullptr};
   /*!
    * Pointer to the output text file. 
    */
   QFile* _out {nullptr};
   /*!
    * Pointer to the qt text stream attached to the output text file and used by this 
    * object o write out integer values. 
    */
   QTextStream* _stream {nullptr};
};



#endif
