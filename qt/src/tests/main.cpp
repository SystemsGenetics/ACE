#include "dummydatafactory.h"
#include "testexception.h"
#include "testdatastream.h"
#include "testdataobject.h"
#include "testdatamanager.h"

#ifdef DUMMY_QTEST_APPLESS_MAIN_NEEDED_FOR_QTCREATOR_TO_RECOGNIZE_THIS_AS_QTTESTS
QTEST_APPLESS_MAIN(TestException)
#endif



int main(int argc, char* argv[])
{
   std::unique_ptr<EAbstractDataFactory> factory(new DummyDataFactory);
   EAbstractDataFactory::setInstance(std::move(factory));
   int status {0};
   auto ASSERT_TEST = [&status,argc,argv](QObject* object)
   {
      status |= QTest::qExec(object,argc,argv);
      delete object;
   };
   ASSERT_TEST(new TestException);
   ASSERT_TEST(new TestDataStream);
   ASSERT_TEST(new TestDataObject);
   ASSERT_TEST(new TestDataManager);
   return status;
}
