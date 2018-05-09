#ifndef IMPORTINTEGERARRAY_H
#define IMPORTINTEGERARRAY_H
#include <core/core.h>
#include "global.h"
//



/*!
 * This implements an import integer array analytic. This reads in a text file 
 * assumed to have a single line of integers separated by spaces and outputs a new 
 * integer array data object containing those integers. This is a simple analytic 
 * which means it does not produce blocks and simply uses the process interface for 
 * incrementally processing its task. 
 */
class ImportIntegerArray : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   virtual ~ImportIntegerArray() override final;
   virtual int size() const override final;
   virtual void process(const EAbstractAnalytic::Block* result) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual void initialize();
private:
   /*!
    * The number of bytes that are read in from the input text file per call to the 
    * process interface. 
    */
   constexpr static const int _incrementSize {16};
   /*!
    * Pointer to the input text file. 
    */
   QFile* _in {nullptr};
   /*!
    * Pointer to the output integer array. 
    */
   IntegerArray* _out {nullptr};
   /*!
    * Pointer to the qt text stream attached to the input text file and used by this 
    * object to read in new integer values. 
    */
   QTextStream* _stream {nullptr};
};



#endif
