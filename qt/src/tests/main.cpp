#include "testexception.h"

#ifdef DUMMY_QTEST_APPLESS_MAIN_NEEDED_FOR_QTCREATOR_TO_RECOGNIZE_THIS_AS_QTTESTS
QTEST_APPLESS_MAIN(TestException)
#endif



int main(int argc, char* argv[])
{
   int status {0};
   auto ASSERT_TEST = [&status,argc,argv](QObject* object)
   {
      status |= QTest::qExec(object,argc,argv);
      delete object;
   };
   ASSERT_TEST(new TestException);
   return status;
}
