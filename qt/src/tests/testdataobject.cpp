#include "testdataobject.h"
#include "dataobject.h"






void TestDataObject::initTestCase()
{
   _path = QDir::tempPath() + "/ACE_test_object.obj";
   QFile file(_path);
   file.remove();
   _object = new Ace::DataObject(_path);
}






void TestDataObject::testIsNew()
{
   Q_ASSERT( _object->isNew() );
}






void TestDataObject::testClear()
{
   _object->clear(0);
   Q_ASSERT( !_object->isNew() );
   Q_ASSERT( *_object );
}






void TestDataObject::testAllocate()
{
   _object->allocate(555);
   Q_ASSERT( _object->size() == 555 );
}






void TestDataObject::testSeek()
{
   Q_ASSERT( _object->seek(55) );
   Q_ASSERT( !_object->seek(555) );
}






void TestDataObject::testBadClear()
{
   bool caught {false};
   try
   {
      _object->clear(1);
   }
   catch (EException)
   {
      caught = true;
   }
   Q_ASSERT( caught );
   Q_ASSERT( *_object );
}






void TestDataObject::testBadOpen()
{
   bool caught {false};
   try
   {
      new Ace::DataObject("/ShouldNotExist.sjd9f78sdhf98");
   }
   catch (EException)
   {
      caught = true;
   }
   Q_ASSERT( caught );
}






void TestDataObject::cleanupTestCase()
{
   QFile file(_path);
   file.remove();
}
