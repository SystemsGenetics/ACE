#include "../src/getopts.h"
#include "unit.h"
namespace getopts
{



using namespace AccelCompEng;



const char* commStr1 = "one -test=1";
const char* commStr2 = "-first one --second=1 TWO ---third=1.12 three ----fourth=ok -----fifth";
const char* commStr3 = "no options";
const char* commStr4 = "--no --comms";
const char* invalidComm = "hello ---test=1=1";



void construct1()
{
   GetOpts t(commStr1);
   if (t.com_size()!=1)
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   bool caught {false};
   try
   {
      GetOpts t(invalidComm);
   }
   catch (GetOpts::InvalidSyntax)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void orig1()
{
   GetOpts t(commStr1);
   if (t.orig()!=std::string(commStr1))
   {
      throw UTests::Fail();
   }
}



void com_size1()
{
   GetOpts t(commStr2);
   if (t.com_size()!=3)
   {
      throw UTests::Fail();
   }
}



void com_empty1()
{
   GetOpts t(commStr4);
   if (!t.com_empty())
   {
      throw UTests::Fail();
   }
}



void com_get1()
{
   GetOpts t(commStr1);
   if (t.com_get({"wrong"})!=0)
   {
      throw UTests::Fail();
   }
}



void com_get2()
{
   GetOpts t(commStr1);
   if (t.com_get({"one"})!=1)
   {
      throw UTests::Fail();
   }
}



void com_front1()
{
   GetOpts t(commStr1);
   if (t.com_front()!=std::string("one"))
   {
      throw UTests::Fail();
   }
}



void com_pop1()
{
   GetOpts t(commStr2);
   t.com_pop();
   if (t.com_get({"TWO"})!=1)
   {
      throw UTests::Fail();
   }
}



void size1()
{
   GetOpts t(commStr2);
   if (t.size()!=5)
   {
      throw UTests::Fail();
   }
}



void empty1()
{
   GetOpts t(commStr3);
   if (!t.empty())
   {
      throw UTests::Fail();
   }
}



void has_opt1()
{
   GetOpts t(commStr2);
   if (!t.has_opt("third"))
   {
      throw UTests::Fail();
   }
}



void has_opt2()
{
   GetOpts t(commStr2);
   t.has_opt("third",true);
   if (t.size()!=4)
   {
      throw UTests::Fail();
   }
}



void iterate1()
{
   GetOpts t(commStr2);
   int count {0};
   for (auto i = t.begin();i!=t.end();++i)
   {
      count++;
   }
   if (count!=5)
   {
      throw UTests::Fail();
   }
}



void erase1()
{
   GetOpts t(commStr1);
   t.erase(t.begin());
   if (t.begin()!=t.end())
   {
      throw UTests::Fail();
   }
}



namespace iterator
{



void key1()
{
   GetOpts t(commStr2);
   auto i = t.begin();
   if (i.key()!=std::string("first"))
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.key()!=std::string("second"))
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.key()!=std::string("third"))
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.key()!=std::string("fourth"))
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.key()!=std::string("fifth"))
   {
      throw UTests::Fail();
   }
}



void value1()
{
   GetOpts t(commStr2);
   auto i = t.begin();
   bool caught {false};
   try
   {
      i.value<int>();
   }
   catch (GetOpts::InvalidType)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void value2()
{
   GetOpts t(commStr2);
   auto i = t.begin();
   ++i;
   if (i.value<int>()!=1)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.value<float>()!=1.12f)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.value<std::string>()!=std::string("ok"))
   {
      throw UTests::Fail();
   }
}



void is_key1()
{
   GetOpts t(commStr1);
   auto i = t.begin();
   if (!i.is_key("test"))
   {
      throw UTests::Fail();
   }
}



void val_empty1()
{
   GetOpts t(commStr2);
   auto i = t.begin();
   if (!i.val_empty())
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.val_empty())
   {
      throw UTests::Fail();
   }
}



void operator1()
{
   GetOpts t(commStr1);
   auto x = t.begin();
   auto y = t.begin();
   if (!(x==y))
   {
      throw UTests::Fail();
   }
}



}
}
using namespace getopts;



void add_getopts(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("GetOpts",nullptr,nullptr));
   run->add_test("construct1",construct1);
   run->add_test("construct2",construct2);
   run->add_test("orig1",orig1);
   run->add_test("com_size1",com_size1);
   run->add_test("com_empty1",com_empty1);
   run->add_test("com_get1",com_get1);
   run->add_test("com_get2",com_get2);
   run->add_test("com_front1",com_front1);
   run->add_test("com_pop1",com_pop1);
   run->add_test("size1",size1);
   run->add_test("empty1",empty1);
   run->add_test("has_opt1",has_opt1);
   run->add_test("has_opt2",has_opt2);
   run->add_test("iterate1",iterate1);
   run->add_test("erase1",erase1);
   tests.attach(run);
   run.reset(new UTests::Run("GetOpts::Iterator",nullptr,nullptr));
   run->add_test("key1",iterator::key1);
   run->add_test("value1",iterator::value1);
   run->add_test("value2",iterator::value2);
   run->add_test("is_key1",iterator::is_key1);
   run->add_test("val_empty1",iterator::val_empty1);
   run->add_test("operator1",iterator::operator1);
   tests.attach(run);
}
