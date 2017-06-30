#include "testexception.h"
#include "exception.h"






void TestException::testSettersGetters()
{
   // make exception object and set all values
   EException e;
   e.setFunction("test function");
   e.setFile("test file");
   e.setTitle("test title");
   e.setDetails("test details");
   e.setType(667);
   e.setLevel(EException::Notice);

   // use getters to make sure all values set correctly
   Q_ASSERT( e.getFunction() == QString("test function") );
   Q_ASSERT( e.getFile() == QString("test file") );
   Q_ASSERT( e.getTitle() == QString("test title") );
   Q_ASSERT( e.getDetails() == QString("test details") );
   Q_ASSERT( e.getType() == 667 );
   Q_ASSERT( e.getLevel() == EException::Notice );

   // change level back to critical and make sure it changed in getter
   e.setLevel(EException::Critical);
   Q_ASSERT( e.getLevel() == EException::Critical );
}
