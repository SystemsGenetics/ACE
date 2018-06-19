#include "testexception.h"
#include "eexception.h"






void TestException::testSettersGetters()
{
   // make exception object and set all values
   EException e;
   e.setFunction("test function");
   e.setFile("test file");
   e.setTitle("test title");
   e.setDetails("test details");

   // use getters to make sure all values set correctly
   Q_ASSERT( e.getFunction() == QString("test function") );
   Q_ASSERT( e.getFile() == QString("test file") );
   Q_ASSERT( e.getTitle() == QString("test title") );
   Q_ASSERT( e.getDetails() == QString("test details") );
}
