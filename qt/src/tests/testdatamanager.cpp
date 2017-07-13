#include "testdatamanager.h"
#include "datamanager.h"
#include "datareference.h"
#include "dummydata.h"



using namespace Ace;






void TestDataManager::initTestCase()
{
   _path = QDir::tempPath() + "/ACE_test_object.obj";
   QFile file(_path);
   file.remove();
}






void TestDataManager::testOpen()
{
   DataManager& manager {DataManager::getInstance()};
   auto ref1 = manager.open(_path);
   auto ref2 = manager.open(_path);
   (*ref1)->clear(0);
   (*ref1)->writeMeta();
   dynamic_cast<DummyData*>(&((*ref1)->data()))->touch();
   Q_ASSERT( dynamic_cast<DummyData*>(&((*ref2)->data()))->getTouch() );
   Q_ASSERT( (*ref1).operator ->() == (*ref2).operator ->() );
   Q_ASSERT( !(*ref1)->isNew() );
   Q_ASSERT( !(*ref2)->isNew() );
   delete ref1;
   delete ref2;
}






void TestDataManager::testDestruction()
{
   DataManager& manager {DataManager::getInstance()};
   auto ref1 = manager.open(_path);
   (*ref1)->clear(0);
   (*ref1)->writeMeta();
   dynamic_cast<DummyData*>(&((*ref1)->data()))->touch();
   delete ref1;
   auto ref2 = manager.open(_path);
   Q_ASSERT( !dynamic_cast<DummyData*>(&((*ref2)->data()))->getTouch() );
   delete ref2;
}






void TestDataManager::cleanupTestCase()
{
   QFile file(_path);
   file.remove();
}
