#include "unit.h"
namespace datamap
{



using namespace AccelCompEng;



AceTestFactory factory;



void in()
{
   LinuxTerm::stty_raw();
}



void out1()
{
   system("rm -fr *.tmp");
   LinuxTerm::stty_cooked();
}



void out2()
{
   system("rm -fr *.tmp");
}



void construct1()
{
   bool caught {false};
   try
   {
      DataMap t(factory);
      DataMap tt(factory);
   }
   catch (DataMap::InvalidUse)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void open1()
{
   DataMap t(factory);
   Data* n = t.open("test.tmp","FakeData",true);
   if (n!=t.current())
   {
      throw UTests::Fail();
   }
}



void open2()
{
   DataMap t(factory);
   bool caught {false};
   try
   {
      t.open("test2.tmp","UnknownData");
   }
   catch (DataMap::InvalidType)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void open3()
{
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   bool caught {false};
   try
   {
      t.open("test.tmp","FakeData");
   }
   catch (DataMap::AlreadyExists)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void close1()
{
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   t.close("test.tmp");
   if (t.find("test.tmp")!=nullptr)
   {
      throw UTests::Fail();
   }
}



void close2()
{
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   if (!t.close("test.tmp"))
   {
      throw UTests::Fail();
   }
}



void unselect1()
{
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   if (!t.unselect())
   {
      throw UTests::Fail();
   }
}



void unselect2()
{
   DataMap t(factory);
   if (t.unselect())
   {
      throw UTests::Fail();
   }
}



void load1()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   bool caught {false};
   try
   {
      t.load(got,term);
   }
   catch (DataMap::NoSelect)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void load2()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   t.select("test.tmp");
   t.load(got,term);
   FakeData* tmp = dynamic_cast<FakeData*>(t.find("test.tmp"));
   if (!(tmp->touched))
   {
      throw UTests::Fail();
   }
}



void load3()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   t.load(got,term);
   bool caught {false};
   try
   {
      t.load(got,term);
   }
   catch (int x)
   {
      if (x==0)
      {
         caught = true;
      }
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void dump1()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   bool caught {false};
   try
   {
      t.dump(got,term);
   }
   catch (DataMap::NoSelect)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void dump2()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   t.select("test.tmp");
   t.dump(got,term);
   FakeData* tmp = dynamic_cast<FakeData*>(t.find("test.tmp"));
   if (!(tmp->touched))
   {
      throw UTests::Fail();
   }
}



void dump3()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   t.dump(got,term);
   bool caught {false};
   try
   {
      t.dump(got,term);
   }
   catch (int x)
   {
      if (x==1)
      {
         caught = true;
      }
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void query1()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   bool caught {false};
   try
   {
      t.query(got,term);
   }
   catch (DataMap::NoSelect)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void query2()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData");
   t.select("test.tmp");
   t.query(got,term);
   FakeData* tmp = dynamic_cast<FakeData*>(t.find("test.tmp"));
   if (!(tmp->touched))
   {
      throw UTests::Fail();
   }
}



void query3()
{
   LinuxTerm term;
   GetOpts got("one");
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   t.query(got,term);
   bool caught {false};
   try
   {
      t.query(got,term);
   }
   catch (int x)
   {
      if (x==2)
      {
         caught = true;
      }
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void iterate1()
{
   DataMap t(factory);
   t.open("test1.tmp","FakeData");
   t.open("test2.tmp","FakeData");
   t.open("test3.tmp","FakeData");
   int count {0};
   for (auto i = t.begin();i!=t.end();++i)
   {
      ++count;
   }
   if (count!=3)
   {
      throw UTests::Fail();
   }
}



void selected1()
{
   DataMap t(factory);
   if (t.selected()!=t.end())
   {
      throw UTests::Fail();
   }
}



void selected2()
{
   DataMap t(factory);
   t.open("test.tmp","FakeData",true);
   if (t.selected()!=t.begin())
   {
      throw UTests::Fail();
   }
}



namespace iterator
{



void file1()
{
   std::string file("test.tmp");
   DataMap t(factory);
   t.open(file,"FakeData");
   auto i = t.begin();
   if (i.file()!=file)
   {
      throw UTests::Fail();
   }
}



void type1()
{
   std::string type("FakeData");
   DataMap t(factory);
   t.open("test.tmp",type);
   auto i = t.begin();
   if (i.type()!=type)
   {
      throw UTests::Fail();
   }
}



}
}
using namespace datamap;



void add_datamap(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("Datamap",in,out1));
   run->add_test("construct1",construct1);
   run->add_test("open1",open1);
   run->add_test("open2",open2);
   run->add_test("open3",open3);
   run->add_test("close1",close1);
   run->add_test("close2",close2);
   run->add_test("unselect1",unselect1);
   run->add_test("unselect2",unselect2);
   run->add_test("load1",load1);
   run->add_test("load2",load2);
   run->add_test("load3",load3);
   run->add_test("dump1",dump1);
   run->add_test("dump2",dump2);
   run->add_test("dump3",dump3);
   run->add_test("query1",query1);
   run->add_test("query2",query2);
   run->add_test("query3",query3);
   run->add_test("iterate1",iterate1);
   run->add_test("selected1",selected1);
   run->add_test("selected2",selected2);
   tests.attach(run);
   run.reset(new UTests::Run("Datamap::Iterator",nullptr,out2));
   run->add_test("file1",iterator::file1);
   run->add_test("type1",iterator::type1);
   tests.attach(run);
}
