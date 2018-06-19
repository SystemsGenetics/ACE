#include "testdatamanager.h"
#include "ace_datamanager.h"
#include "datareference.h"
#include "dummydata.h"



using namespace Ace;






void TestDataManager::initTestCase()
{
   // make sure temporary file does not exist
   _path = QDir::tempPath() + "/ACE_test_object.obj";
   QFile file(_path);
   file.remove();
}






void TestDataManager::testOpen()
{
   // open two references to the same file
   DataManager& manager {DataManager::getInstance()};
   auto ref1 = manager.open(_path);
   auto ref2 = manager.open(_path);

   // make sure both references point to the same data object
   (*ref1)->clear(0);
   (*ref1)->writeMeta();
   dynamic_cast<DummyData*>(&((*ref1)->data()))->touch();
   Q_ASSERT( dynamic_cast<DummyData*>(&((*ref2)->data()))->getTouch() );
   Q_ASSERT( (*ref1).operator ->() == (*ref2).operator ->() );
   Q_ASSERT( !(*ref1)->isNew() );
   Q_ASSERT( !(*ref2)->isNew() );

   // cleanup references
   delete ref1;
   delete ref2;
}






void TestDataManager::testDestruction()
{
   // open a single reference
   DataManager& manager {DataManager::getInstance()};
   auto ref1 = manager.open(_path);

   // clear out the data object
   (*ref1)->clear(0);
   (*ref1)->writeMeta();

   // touch the abstract data within in then close the reference
   dynamic_cast<DummyData*>(&((*ref1)->data()))->touch();
   delete ref1;

   // open a new reference and make sure the abstract data is a new untouched object
   auto ref2 = manager.open(_path);
   Q_ASSERT( !dynamic_cast<DummyData*>(&((*ref2)->data()))->getTouch() );

   // cleaup second reference
   delete ref2;
}






void TestDataManager::cleanupTestCase()
{
   // remove temporary file
   QFile file(_path);
   file.remove();
}
