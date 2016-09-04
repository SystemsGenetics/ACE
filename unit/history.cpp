#include "../src/history.h"
#include "../src/fstring.h"
#include "unit.h"



using namespace AccelCompEng;



namespace history
{



struct HistNode : public NVMemory::Node
{
   HistNode(const std::shared_ptr<NVMemory>& mem, const std::string& fileName,
            const std::string& object, const std::string& command, uint64_t timeStamp):
      Node(sizeof(Header),mem)
   {
      init_data<Header>();
      get<Header>()._timeStamp = timeStamp;
      allocate();
      write();
      FString str(mem);
      str.write(fileName);
      str.reset();
      str.write(object);
      str.reset();
      str.write(command);
   }
   using Node::get;
   struct __attribute__ ((__packed__)) Header
   {
      uint64_t _timeStamp {0};
      int64_t _next {fnullptr};
      int64_t _childHead {fnullptr};
   };
   void null_data() {}
   void flip_endian() { flip(0,8); flip(8,8); flip(16,8); }
};
const char* validFile = "histfile.tmp";
const char* testStr1 = "Hello! This is a really long test string for history.";
const char* testStr2 = "Guten tag! A totally different thingy for the past thing.";
const char* testStr3 = "Why? why oh why am I just a constant thingy to never do all!";



void in()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   HistNode hist1(mem,testStr1,testStr2,testStr3,1);
   HistNode hist2(mem,testStr1,testStr2,testStr3,2);
   HistNode hist3(mem,testStr1,testStr2,testStr3,3);
   HistNode hist4(mem,testStr1,testStr2,testStr3,4);
   HistNode hist5(mem,testStr1,testStr2,testStr3,5);
   HistNode hist6(mem,testStr1,testStr2,testStr3,6);
   hist1.get<HistNode::Header>()._childHead = hist2.addr();
   hist2.get<HistNode::Header>()._next = hist3.addr();
   hist2.get<HistNode::Header>()._childHead = hist4.addr();
   hist4.get<HistNode::Header>()._next = hist5.addr();
   hist4.get<HistNode::Header>()._childHead = hist6.addr();
   hist1.write();
   hist2.write();
   hist4.write();
}



void out()
{
   system("rm -fr *.tmp");
}



void construct1()
{
   History hist;
   if (hist.has_children())
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem);
   if (hist.has_children())
   {
      throw UTests::Fail();
   }
}



void construct3()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (!hist.has_children())
   {
      throw UTests::Fail();
   }
}



void construct4()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   auto i = hist.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=2)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=3)
   {
      throw UTests::Fail();
   }
   i = hist.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=4)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=5)
   {
      throw UTests::Fail();
   }
   i = hist.begin();
   i = i.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=6)
   {
      throw UTests::Fail();
   }
}



void load1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   hist.load(fheadptr);
   if (!hist.has_children())
   {
      throw UTests::Fail();
   }
}



void load2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem);
   hist.load(fheadptr);
   auto i = hist.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=2)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=3)
   {
      throw UTests::Fail();
   }
   i = hist.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=4)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=5)
   {
      throw UTests::Fail();
   }
   i = hist.begin();
   i = i.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string(testStr3)||i.history().time_stamp()!=6)
   {
      throw UTests::Fail();
   }
}



void init1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem);
   hist.init(testStr1,testStr2,testStr3);
   if (hist.file_name()!=std::string(testStr1)||hist.object()!=std::string(testStr2)||
       hist.command()!=std::string(testStr3))
   {
      throw UTests::Fail();
   }
}



void init2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   bool caught {false};
   try
   {
      hist.init(testStr1,testStr2,testStr3);
   }
   catch (History::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void add_child1()
{
   History copy;
   copy.init(testStr1,testStr2,testStr3);
   History root;
   root.add_child(copy);
   if (root.begin()->file_name()!=std::string(testStr1)||
       root.begin()->object()!=std::string(testStr2)||
       root.begin()->command()!=std::string(testStr3))
   {
      throw UTests::Fail();
   }
}



void add_child2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History copy(mem);
   History hist(mem,fheadptr);
   bool caught {false};
   try
   {
      hist.add_child(copy);
   }
   catch (History::AlreadySet)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void add_child3()
{
   History root;
   History child1;
   History child2;
   History child3;
   child1.init(testStr1,testStr2,"child1");
   child2.init(testStr1,testStr2,"child2");
   child3.init(testStr1,testStr2,"child3");
   child1.add_child(child3);
   root.add_child(child1);
   root.add_child(child2);
   auto i = root.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child1")||&(*i)==&child1)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child2")||&(*i)==&child2)
   {
      throw UTests::Fail();
   }
   i = root.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child3")||&(*i)==&child3||
       &(*i)==&(child1.begin().history()))
   {
      throw UTests::Fail();
   }
}



void write1()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History copy(mem);
   copy.init(testStr1,testStr2,testStr3);
   int64_t addr {copy.write()};
   History hist(mem,addr);
   if (hist.file_name()!=std::string(testStr1)||hist.object()!=std::string(testStr2)||
       hist.command()!=std::string(testStr3))
   {
      throw UTests::Fail();
   }
}



void write2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History root(mem);
   History child1;
   History child2;
   History child3;
   child1.init(testStr1,testStr2,"child1");
   child2.init(testStr1,testStr2,"child2");
   child3.init(testStr1,testStr2,"child3");
   child1.add_child(child3);
   root.add_child(child1);
   root.add_child(child2);
   int64_t addr {root.write()};
   History hist(mem,addr);
   auto i = hist.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child1")||&(*i)==&child1)
   {
      throw UTests::Fail();
   }
   ++i;
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child2")||&(*i)==&child2)
   {
      throw UTests::Fail();
   }
   i = hist.begin();
   i = i.begin();
   if (i.history().file_name()!=std::string(testStr1)||i.history().object()!=std::string(testStr2)||
       i.history().command()!=std::string("child3")||&(*i)==&child3||
       &(*i)==&(child1.begin().history()))
   {
      throw UTests::Fail();
   }
}



void write3()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   bool caught {false};
   try
   {
      hist.write();
   }
   catch (History::AlreadySet)
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
   History hist;
   bool caught {false};
   try
   {
      hist.write();
   }
   catch (History::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void file_name1()
{
   History hist;
   if (hist.file_name()!=std::string(""))
   {
      throw UTests::Fail();
   }
   hist.init(testStr1,"","");
   if (hist.file_name()!=std::string(testStr1))
   {
      throw UTests::Fail();
   }
}



void file_name2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.file_name()!=std::string(testStr1))
   {
      throw UTests::Fail();
   }
}



void object1()
{
   History hist;
   if (hist.object()!=std::string(""))
   {
      throw UTests::Fail();
   }
   hist.init("",testStr1,"");
   if (hist.object()!=std::string(testStr1))
   {
      throw UTests::Fail();
   }
}



void object2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.object()!=std::string(testStr2))
   {
      throw UTests::Fail();
   }
}



void command1()
{
   History hist;
   if (hist.command()!=std::string(""))
   {
      throw UTests::Fail();
   }
   hist.init("","",testStr1);
   if (hist.command()!=std::string(testStr1))
   {
      throw UTests::Fail();
   }
}



void command2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.command()!=std::string(testStr3))
   {
      throw UTests::Fail();
   }
}



void time_stamp1()
{
   History hist;
   if (hist.time_stamp()!=0)
   {
      throw UTests::Fail();
   }
   hist.init("","","");
   if (hist.time_stamp()==0)
   {
      throw UTests::Fail();
   }
}



void time_stamp2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.time_stamp()!=1)
   {
      throw UTests::Fail();
   }
}



void has_children1()
{
   History hist;
   if (hist.has_children())
   {
      throw UTests::Fail();
   }
   History copy;
   hist.add_child(copy);
   if (!hist.has_children())
   {
      throw UTests::Fail();
   }
}



void has_children2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (!hist.has_children())
   {
      throw UTests::Fail();
   }
}



void iterator1()
{
   History hist;
   if (hist.begin()!=hist.end())
   {
      throw UTests::Fail();
   }
   History copy;
   hist.add_child(copy);
   if (hist.begin()==hist.end())
   {
      throw UTests::Fail();
   }
}



void iterator2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.begin()==hist.end())
   {
      throw UTests::Fail();
   }
}



namespace iterator
{



void iterator1()
{
   History root;
   History hist;
   root.add_child(hist);
   History::Iterator i = root.begin();
   if (i.begin()!=i.end())
   {
      throw UTests::Fail();
   }
   History copy;
   hist.add_child(copy);
   root.add_child(hist);
   ++i;
   if (i.begin()==i.end())
   {
      throw UTests::Fail();
   }
}



void iterator2()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   if (hist.begin().begin()==hist.begin().end())
   {
      throw UTests::Fail();
   }
}



void iterator3()
{
   History hist;
   History::Iterator i = hist.begin();
   bool caught {false};
   try
   {
      i.begin();
   }
   catch (History::Iterator::OutOfRange)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void iterator4()
{
   History hist;
   History::Iterator i = hist.begin();
   bool caught {false};
   try
   {
      i.end();
   }
   catch (History::Iterator::OutOfRange)
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
   History root;
   History hist;
   root.add_child(hist);
   auto i = root.begin();
   if (i.history().has_children())
   {
      throw UTests::Fail();
   }
}



void history2()
{
   History hist;
   History::Iterator i = hist.begin();
   bool caught {false};
   try
   {
      i.history();
   }
   catch (History::Iterator::OutOfRange)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void operator1()
{
   History root;
   History hist;
   root.add_child(hist);
   History::Iterator i = root.begin();
   if ((*i).has_children())
   {
      throw UTests::Fail();
   }
}



void operator2()
{
   History root;
   History hist;
   root.add_child(hist);
   History::Iterator i = root.begin();
   if (i->time_stamp()!=0)
   {
      throw UTests::Fail();
   }
}



void operator3()
{
   History root;
   if (!(root.end()==root.begin()))
   {
      throw UTests::Fail();
   }
}



void operator4()
{
   History root;
   History hist;
   root.add_child(hist);
   if (!(root.begin()!=root.end()))
   {
      throw UTests::Fail();
   }
}



void operator5()
{
   std::shared_ptr<NVMemory> mem {new NVMemory(validFile)};
   History hist(mem,fheadptr);
   History::Iterator i = hist.begin();
   ++i;
   if (i.history().time_stamp()!=3)
   {
      throw UTests::Fail();
   }
   ++i;
   bool caught {false};
   try
   {
      i.history();
   }
   catch (History::Iterator::OutOfRange)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   ++i;
   caught = false;
   try
   {
      i.history();
   }
   catch (History::Iterator::OutOfRange)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



}
}
using namespace history;



void add_history(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("History",in,out));
   run->add_test("construct1",construct1);
   run->add_test("construct2",construct2);
   run->add_test("construct3",construct3);
   run->add_test("construct4",construct4);
   run->add_test("load1",load1);
   run->add_test("load2",load2);
   run->add_test("init1",init1);
   run->add_test("init2",init2);
   run->add_test("add_child1",add_child1);
   run->add_test("add_child2",add_child2);
   run->add_test("add_child3",add_child3);
   run->add_test("write1",write1);
   run->add_test("write2",write1);
   run->add_test("write3",write1);
   run->add_test("write4",write1);
   run->add_test("file_name1",file_name1);
   run->add_test("file_name2",file_name2);
   run->add_test("object1",object1);
   run->add_test("object2",object2);
   run->add_test("command1",command1);
   run->add_test("command2",command2);
   run->add_test("time_stamp1",time_stamp1);
   run->add_test("time_stamp2",time_stamp2);
   run->add_test("has_children1",has_children1);
   run->add_test("has_children2",has_children2);
   run->add_test("iterator1",iterator1);
   run->add_test("iterator2",iterator2);
   tests.attach(run);
   run.reset(new UTests::Run("History::Iterator",in,out));
   run->add_test("iterator1",iterator::iterator1);
   run->add_test("iterator2",iterator::iterator2);
   run->add_test("iterator3",iterator::iterator3);
   run->add_test("iterator4",iterator::iterator4);
   run->add_test("history1",iterator::history1);
   run->add_test("history2",iterator::history2);
   run->add_test("operator1",iterator::operator1);
   run->add_test("operator2",iterator::operator2);
   run->add_test("operator3",iterator::operator3);
   run->add_test("operator4",iterator::operator4);
   run->add_test("operator5",iterator::operator5);
   tests.attach(run);
}
