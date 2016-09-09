#include "../src/file.h"
#include "unit.h"
namespace file
{


using namespace AccelCompEng;



struct FileNode : public NVMemory::Node
{
   FileNode(const std::shared_ptr<NVMemory>& mem, bool frag):
      Node(sizeof(Header),mem)
   {
      init_data<Header>();
      if (frag)
      {
         get<Header>()._id[2] = '\0';
      }
      allocate();
      write();
   }
   constexpr static int _idSize {4};
   constexpr static const char* _idString = "\113\111\116\103";
   struct __attribute__ ((__packed__)) Header
   {
      char _id[_idSize] {_idString[0],_idString[1],_idString[2],_idString[3]};
      int64_t _historyHead {fnullptr};
      int64_t _dataHead {fnullptr};
      int64_t _identPtr {fnullptr};
   };
   void null_data() {}
   void flip_endian()
   {
      flip(4,8);
      flip(12,8);
      flip(20,8);
   }
};
class TestFile : public File
{
public:
   using File::File;
   using File::ident;
   using File::head;
   using File::mem;
   using File::rmem;
};
const char* validFile = "file.tmp";
const char* tmpFile = "tfile.tmp";
const char* invalidFile1 = "notfile1.tmp";
const char* invalidFile2 = "notfile1.tmp";
const char* testStr = "This is a really long test string to make sure it works really swell.";
constexpr int rmDelayUS {10000};



void in()
{
   std::shared_ptr<NVMemory> validMem {new NVMemory(validFile)};
   std::shared_ptr<NVMemory> invalidMem1 {new NVMemory(invalidFile1)};
   std::shared_ptr<NVMemory> invalidMem2 {new NVMemory(invalidFile2)};
   FileNode fnode(validMem,false);
   FileNode bfnode(invalidMem1,true);
   invalidMem2->allocate(sizeof(FileNode::Header)-1);
}



void out()
{
   system("rm -fr *.tmp");
}



void construct1()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.clear();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void load1()
{
   TestFile file;
   file.load(validFile);
   if (file.is_new())
   {
      throw UTests::Fail();
   }
}



void load2()
{
   TestFile file;
   file.load(tmpFile);
   if (!file.is_new())
   {
      throw UTests::Fail();
   }
   NVMemory mem(tmpFile);
   mem.clear();
}



void load3()
{
   TestFile file;
   file.load(tmpFile);
   file.load(validFile);
   if (file.is_new())
   {
      throw UTests::Fail();
   }
}



void load4()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.load(invalidFile1);
   }
   catch (File::InvalidFile)
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
   TestFile file;
   bool caught {false};
   try
   {
      file.load(invalidFile2);
   }
   catch (File::InvalidFile)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void clear1()
{
   TestFile file;
   file.load(tmpFile);
   file.clear();
   if (!file.is_new())
   {
      throw UTests::Fail();
   }
   std::ostringstream buf;
   NVMemory mem(tmpFile);
   mem.clear();
}



void clear2()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.clear();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void clear3()
{
   TestFile file;
   file.load(tmpFile);
   bool caught {false};
   try
   {
      file.clear();
   }
   catch (File::AlreadyNew)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void is_new1()
{
   TestFile file;
   file.load(tmpFile);
   if (file.is_new())
   {
      throw UTests::Fail();
   }
}



void is_new2()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.is_new();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void init_history1()
{
   TestFile file;
   file.load(tmpFile);
   file.init_history();
   if (file.history().has_children())
   {
      throw UTests::Fail();
   }
}



void init_history2()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.init_history();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void init_history3()
{
   TestFile file;
   file.load(tmpFile);
   file.init_history();
   bool caught {false};
   try
   {
      file.init_history();
   }
   catch (File::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void history1()
{
   TestFile file;
   file.load(tmpFile);
   file.init_history();
   if (file.history().has_children())
   {
      throw UTests::Fail();
   }
}



void history2()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.history();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void history3()
{
   TestFile file;
   file.load(tmpFile);
   bool caught {false};
   try
   {
      file.history();
   }
   catch (File::NullHistory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write_history1()
{
   TestFile file;
   file.load(tmpFile);
   file.init_history();
   file.write_history();
   TestFile check;
   check.load(tmpFile);
   if (check.history().has_children())
   {
      throw UTests::Fail();
   }
   NVMemory mem(tmpFile);
   mem.clear();
}



void write_history2()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.write_history();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write_history3()
{
   TestFile file;
   file.load(tmpFile);
   bool caught {false};
   try
   {
      file.write_history();
   }
   catch (File::NullHistory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void write_history4()
{
   TestFile file;
   file.load(tmpFile);
   file.init_history();
   file.write_history();
   bool caught {false};
   try
   {
      file.write_history();
   }
   catch (File::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   NVMemory mem(tmpFile);
   mem.clear();
}



void ident1()
{
   TestFile file;
   file.load(tmpFile);
   file.ident(testStr);
   if (file.ident()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
}



void ident2()
{
   TestFile file;
   file.load(tmpFile);
   if (file.ident()!=std::string(testStr))
   {
      throw UTests::Fail();
   }
   NVMemory mem(tmpFile);
   mem.clear();
}



void ident3()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.ident();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void ident4()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.ident(testStr);
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void ident5()
{
   TestFile file;
   file.load(tmpFile);
   bool caught {false};
   try
   {
      file.ident();
   }
   catch (File::NullIdent)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void ident6()
{
   TestFile file;
   file.load(tmpFile);
   file.ident(testStr);
   bool caught {false};
   try
   {
      file.ident(testStr);
   }
   catch (File::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void head1()
{
   TestFile file;
   file.load(tmpFile);
   file.head(666);
   if (file.head()!=666)
   {
      throw UTests::Fail();
   }
}



void head2()
{
   TestFile file;
   file.load(tmpFile);
   if (file.head()!=666)
   {
      throw UTests::Fail();
   }
}



void head3()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.head();
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void head4()
{
   TestFile file;
   bool caught {false};
   try
   {
      file.head(666);
   }
   catch (File::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void mem1()
{
   TestFile file;
   file.load(tmpFile);
   if (file.mem().get()==nullptr)
   {
      throw UTests::Fail();
   }
}



void rmem1()
{
   TestFile file;
   file.load(tmpFile);
   if (file.rmem().size()<0)
   {
      throw UTests::Fail();
   }
}



}
using namespace file;



void add_file(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("File",in,out));
   run->add_test("construct1",construct1);
   run->add_test("load1",load1);
   run->add_test("load2",load2);
   run->add_test("load3",load3);
   run->add_test("load4",load4);
   run->add_test("load5",load5);
   run->add_test("clear1",clear1);
   run->add_test("clear2",clear2);
   run->add_test("clear3",clear3);
   run->add_test("is_new1",is_new1);
   run->add_test("is_new2",is_new2);
   run->add_test("init_history1",init_history1);
   run->add_test("init_history2",init_history2);
   run->add_test("init_history3",init_history3);
   run->add_test("history1",history1);
   run->add_test("history2",history2);
   run->add_test("history3",history3);
   run->add_test("write_history1",write_history1);
   run->add_test("write_history2",write_history2);
   run->add_test("write_history3",write_history3);
   run->add_test("write_history4",write_history4);
   run->add_test("ident1",ident1);
   run->add_test("ident2",ident2);
   run->add_test("ident3",ident3);
   run->add_test("ident4",ident4);
   run->add_test("ident5",ident5);
   run->add_test("ident6",ident6);
   run->add_test("head1",head1);
   run->add_test("head2",head2);
   run->add_test("head3",head3);
   run->add_test("head4",head4);
   run->add_test("mem1",mem1);
   run->add_test("rmem1",rmem1);
   tests.attach(run);
}
