#include "../src/fstring.h"
#include "unit.h"
#include <string.h>
using namespace AccelCompEng;




namespace fstring
{



struct TestStringHead : public NVMemory::Node
{
   TestStringHead(): Node(sizeof(Header)) { init_data<Header>(); }
   using Node::get;
   struct __attribute__ ((__packed__)) Header
   {
      int16_t _size {0};
      uint8_t _strip {0};
   };
   void null_data() override final {}
   void flip_endian() override final { flip(0,2); }
};
struct TestString : public NVMemory::Node
{
   TestString(int len): Node(len) { init_data<char>(len); }
   using Node::get;
   void null_data() override final {}
   void flip_endian() override final {}
};
constexpr int stripVal {170};
const char* testStr = "hello world! A really long string.";
const char* altTestStr = "hello world! Another very long string that is different.";
const char* tmpFile = "strfile.tmp";
int64_t badAddr;



void in()
{
   int len = strlen(testStr)+1;
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   TestStringHead strhead;
   strhead.mem(mem);
   strhead.allocate();
   strhead.get<TestStringHead::Header>()._size = len;
   strhead.get<TestStringHead::Header>()._strip = stripVal;
   strhead.write();
   TestString str(len);
   str.mem(mem);
   str.allocate();
   memcpy(&(str.get<char>()),testStr,len);
   str.write();
   len = strlen(altTestStr)+1;
   strhead.addr(fnullptr);
   strhead.allocate();
   strhead.get<TestStringHead::Header>()._size = len;
   strhead.get<TestStringHead::Header>()._strip = stripVal;
   strhead.write();
   TestString str2(len);
   str2.mem(mem);
   str2.allocate();
   memcpy(&(str2.get<char>()),altTestStr,len);
   str2.write();
   strhead.addr(fnullptr);
   strhead.allocate();
   badAddr = strhead.addr();
   strhead.get<TestStringHead::Header>()._size = 1;
   strhead.get<TestStringHead::Header>()._strip = stripVal+1;
   strhead.write();
}



void out()
{
   system("rm -fr *.tmp");
}



void construct1()
{
   FString str;
   auto mem = str.mem();
   if (mem.get())
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   if (str.mem()!=mem)
   {
      throw UTests::Fail();
   }
}



void construct3()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem,fheadptr);
   if (str.mem()!=mem||str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void load1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.load(fheadptr);
   if (str.mem()!=mem||str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void load2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr)+1);
   str.load(fheadptr);
   if (str.mem()!=mem||str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void load3()
{
   FString str;
   bool caught {false};
   try
   {
      str.load(fheadptr);
   }
   catch (FString::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void load4()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   bool caught {false};
   try
   {
      str.load(badAddr);
   }
   catch (FString::InvalidPtr)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void load5()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr));
   bool caught {false};
   try
   {
      str.load(fheadptr);
   }
   catch (FString::BufferOverrun)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void reset1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem,fheadptr);
   str.reset();
   str.write(testStr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void str1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem,fheadptr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void str2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   bool caught {false};
   try
   {
      str.str();
   }
   catch (FString::NullStr)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void str3()
{
   FString str;
   bool caught {false};
   try
   {
      str.str();
   }
   catch (FString::NullStr)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   int64_t tmpAddr = str.write(testStr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
   FString str2(mem,tmpAddr);
   if (str2.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void write2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer((strlen(testStr)*2)+2);
   int64_t tmpAddr = str.write(testStr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
   FString str2(mem,tmpAddr);
   if (str2.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void write3()
{
   FString str;
   bool caught {false};
   try
   {
      str.write(testStr);
   }
   catch (FString::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write4()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem,fheadptr);
   bool caught {false};
   try
   {
      str.write(testStr);
   }
   catch (FString::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write5()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr));
   bool caught {false};
   try
   {
      str.write(testStr);
   }
   catch (FString::BufferOverrun)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void static_buffer1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr)+1);
   str.load(fheadptr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void static_buffer2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr));
   bool caught {false};
   try
   {
      str.load(fheadptr);
   }
   catch (FString::BufferOverrun)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void static_buffer3()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   bool caught {false};
   try
   {
      str.static_buffer(0);
   }
   catch (FString::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void clear_buffer1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   str.static_buffer(strlen(testStr));
   str.clear_buffer();
   str.load(fheadptr);
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void bump1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem,fheadptr);
   str.bump();
   if (str.str()!=std::string(altTestStr))
   {
      throw UTests::Fail();
   }
}



void bump2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(tmpFile)};
   FString str(mem);
   int64_t startAddr = str.write(testStr);
   str.reset();
   str.write(altTestStr);
   str.reset();
   str.write(testStr);
   str.reset();
   str.write(altTestStr);
   str.load(startAddr);
   str.bump();
   if (str.str()!=std::string(altTestStr))
   {
      throw UTests::Fail();
   }
   str.bump();
   if (str.str()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
   str.bump();
   if (str.str()!=std::string(altTestStr))
   {
      throw UTests::Fail();
   }
}



}
using namespace fstring;


void add_fstring(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("FString",in,out));
   run->add_test("construct1",construct1);
   run->add_test("construct2",construct2);
   run->add_test("construct3",construct3);
   run->add_test("load1",load1);
   run->add_test("load2",load2);
   run->add_test("load3",load3);
   run->add_test("load4",load4);
   run->add_test("load5",load5);
   run->add_test("reset1",reset1);
   run->add_test("str1",str1);
   run->add_test("str2",str2);
   run->add_test("str3",str3);
   run->add_test("write1",write1);
   run->add_test("write2",write2);
   run->add_test("write3",write3);
   run->add_test("write4",write4);
   run->add_test("write5",write4);
   run->add_test("static_buffer1",static_buffer1);
   run->add_test("static_buffer2",static_buffer2);
   run->add_test("static_buffer3",static_buffer3);
   run->add_test("clear_buffer1",clear_buffer1);
   run->add_test("bump1",bump1);
   run->add_test("bump2",bump2);
   tests.attach(run);
}


/*
namespace unit
{
   namespace fstring
   {
      constexpr int stripeVal {170};
      constexpr const char* testStr = "hello world! A really long string.";
      struct TestString : FileMem::Static<38>
      {
         TestString()
         {
            stripe() = stripeVal;
            sSize() = 35;
            memcpy(c_str(),testStr,35);
         }
         uint8_t& stripe() { get<uint8_t>(0); }
         uint16_t& sSize() { get<uint16_t>(1); }
         char* c_str() { &get<char>(3); }
      };
      constexpr auto tmpFile = "strfile.tmp";
   }
}



bool unit::fstring::main()
{
   bool ret = false;
   header("FString");
   try
   {
      ret = construct()&&
            move()&&
            addr()&&
            operat_fp()&&
            operat_set()&&
            final();
   }
   catch (...)
   {
      system("rm -f *.tmp");
      end();
      throw;
   }
   system("rm -f *.tmp");
   end();
   return ret;
}



bool unit::fstring::construct()
{
   std::string empty;
   std::string hello(testStr);
   bool cont = true;
   {
      start();
      FileMem tf(tmpFile);
      FString t(&tf);
      bool test = *t==empty;
      cont = cont&&finish(test,"construct1");
   }
   if (cont)
   {
      start();
      {
         FileMem tf(tmpFile);
         tf.clear();
         TestString tmp;
         tf.allot(tmp);
         tf.sync(tmp,FileSync::write);
      }
      FileMem tf(tmpFile);
      FString t(&tf,tf.head());
      bool test = *t==hello;
      cont = cont&&finish(test,"construct2");
   }
   if (cont)
   {
      start();
      {
         FileMem tf(tmpFile);
         tf.clear();
         TestString tmp;
         tmp.stripe() = stripeVal+1;
         tf.allot(tmp);
         tf.sync(tmp,FileSync::write);
      }
      bool test = false;
      try
      {
         FileMem tf(tmpFile);
         FString t(&tf,tf.head());
      }
      catch (FString::InvalidPtr)
      {
         test = true;
      }
      cont = cont&&finish(test,"construct3");
   }
   if (cont)
   {
      start();
      bool test = false;
      try
      {
         FString t(nullptr);
      }
      catch(FString::InvalidPtr)
      {
         test = true;
      }
      cont = cont&&finish(test,"construct4");
   }
   return cont;
}



bool unit::fstring::move()
{
   std::string hello(testStr);
   bool cont = true;
   {
      start();
      {
         FileMem tf(tmpFile);
         tf.clear();
         TestString tmp;
         tf.allot(tmp);
         tf.sync(tmp,FileSync::write);
      }
      FileMem tf(tmpFile);
      FString tmp(&tf,tf.head());
      FString t(std::move(tmp));
      bool test = *t==hello;
      cont = cont&&finish(test,"move1");
   }
   if (cont)
   {
      start();
      FileMem tf(tmpFile);
      FString tmp(&tf,tf.head());
      FString t(&tf);
      t = std::move(tmp);
      bool test = *t==hello;
      cont = cont&&finish(test,"move2");
   }
   return cont;
}



bool unit::fstring::addr()
{
   start();
   {
      FileMem tf(tmpFile);
      tf.clear();
      TestString tmp;
      tf.allot(tmp);
      tf.sync(tmp,FileSync::write);
   }
   FileMem tf(tmpFile);
   FString t(&tf);
   t.addr(tf.head());
   bool test = t.addr()==tf.head();
   return finish(test,"addr1");
}



bool unit::fstring::operat_fp()
{
   start();
   FileMem tf(tmpFile);
   FString t(&tf);
   bool test = t->empty();
   return finish(test,"operat_fp1");
}



bool unit::fstring::operat_set()
{
   std::string hello(testStr);
   bool cont = true;
   {
      start();
      FileMem tf(tmpFile);
      tf.clear();
      FString t(&tf);
      t = testStr;
      bool test = *t==hello;
      cont = cont&&finish(test,"operat_set1");
   }
   if (cont)
   {
      start();
      FileMem tf(tmpFile);
      FString t(&tf,tf.head());
      bool test = false;
      try
      {
         t = testStr;
      }
      catch (FString::AlreadySet)
      {
         test = true;
      }
      cont = cont&&finish(test,"operat_set2");
   }
   return cont;
}



bool unit::fstring::final()
{
   std::string hello(testStr);
   start();
   FileMem::Ptr adr1;
   FileMem::Ptr adr2;
   FileMem::Ptr adr3;
   {
      FileMem tf(tmpFile);
      FString t(&tf);
      t = testStr;
      adr1 = t.addr();
      t.addr(FileMem::nullPtr);
      t = testStr;
      adr2 = t.addr();
      t.addr(FileMem::nullPtr);
      t = testStr;
      adr3 = t.addr();
   }
   FileMem tf(tmpFile);
   FString t(&tf,adr1);
   bool test = *t==hello;
   t.addr(adr2);
   test = test&&*t==hello;
   t.addr(adr3);
   test = test&&*t==hello;
   return finish(test,"final1");
}
*/
