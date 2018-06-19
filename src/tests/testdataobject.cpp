#include "testdataobject.h"
#include "ace_dataobject.h"






void TestDataObject::initTestCase()
{
   // make sure data object file is empty
   _path = QDir::tempPath() + "/ACE_test_object.obj";
   QFile file(_path);
   file.remove();

   // initialize new data object
   _object = new Ace::DataObject(_path);
}






void TestDataObject::testIsNew()
{
   Q_ASSERT( _object->isNew() );
}






void TestDataObject::testClear()
{
   // make sure clear works
   _object->clear(0);
   Q_ASSERT( !_object->isNew() );
   Q_ASSERT( *_object );
}






void TestDataObject::testAllocate()
{
   // make sure allocate works
   _object->allocate(555);
   Q_ASSERT( _object->size() == 555 );
}






void TestDataObject::testSeek()
{
   // make sure seek works
   Q_ASSERT( _object->seek(55) );
   Q_ASSERT( !_object->seek(555) );
}






void TestDataObject::testBadClear()
{
   // attempt to make a new data object with invalid type
   bool caught {false};
   try
   {
      _object->clear(1);
   }
   catch (EException)
   {
      caught = true;
   }

   // make sure it failed
   Q_ASSERT( caught );
   Q_ASSERT( *_object );
}






void TestDataObject::testBadOpen()
{
   // attempt to open a file that user has no access to
   bool caught {false};
   try
   {
      new Ace::DataObject("/ShouldNotExist.sjd9f78sdhf98");
   }
   catch (EException)
   {
      caught = true;
   }

   // make sure it failed
   Q_ASSERT( caught );
}






void TestDataObject::cleanupTestCase()
{
   // remove temporary file
   QFile file(_path);
   file.remove();
}
