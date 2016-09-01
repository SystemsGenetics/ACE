#include "unit.h"
#include "../src/nvmemory.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>



namespace nvmemory
{



using namespace AccelCompEng;



class TestNode : public NVMemory::Node
{
public:
   using Node::Node;
   using Node::init_data;
   using Node::give_data;
   using Node::get;
   using Node::pget;
   using Node::flip;
   int _counter {0};
   int _fData {1};
private:
   void null_data() override final { _fData = 0; }
   void flip_endian() override final { ++_counter; }
};



const char* tmpFile = "memfile.tmp";
const char* validFile = "memfile2.tmp";
const char* invalidFile = "notmemfile.tmp";
const char* invalidFile2 = "notmemfile2.tmp";
const char* dataStr = "123456789012345";
constexpr int rmDelayUS {10000};



void in()
{
   constexpr static int flags = O_CREAT|O_RDWR|O_LARGEFILE;
   constexpr static mode_t modes = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
   int fd;
   char identString[6] = "\33\102\104\101\124";
   int idLen = 5;
   int64_t next = 0;
   fd = open(validFile,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(int64_t));
   close(fd);
   fd = open(invalidFile,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(int64_t)-1);
   close(fd);
   identString[1] = '\16';
   fd = open(invalidFile2,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(int64_t));
   close(fd);
}



void out()
{
   system("rm -f *.tmp");
}



void construct1()
{
   NVMemory mem;
   if (mem.is_open())
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   NVMemory mem(tmpFile);
   if (!mem.is_open())
   {
      throw UTests::Fail();
   }
   std::ostringstream tmp;
   tmp << "rm -f " << tmpFile;
   system(tmp.str().c_str());
   usleep(rmDelayUS);
}



void construct3()
{
   NVMemory mem(validFile);
   if (!mem.is_open())
   {
      throw UTests::Fail();
   }
}



void construct4()
{
   bool caught {false};
   try
   {
      NVMemory mem(invalidFile);
   }
   catch (NVMemory::InvalidFile)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void construct5()
{
   bool caught {false};
   try
   {
      NVMemory mem(invalidFile2);
   }
   catch (NVMemory::InvalidFile)
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
   NVMemory mem;
   mem.open(tmpFile);
   if (!mem.is_open())
   {
      throw UTests::Fail();
   }
   std::ostringstream tmp;
   tmp << "rm -f " << tmpFile;
   system(tmp.str().c_str());
   usleep(rmDelayUS);
}



void open2()
{
   NVMemory mem;
   mem.open(validFile);
   if (!mem.is_open())
   {
      throw UTests::Fail();
   }
}



void open3()
{
   bool caught {false};
   try
   {
      NVMemory mem;
      mem.open(invalidFile);
   }
   catch (NVMemory::InvalidFile)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void open4()
{
   bool caught {false};
   try
   {
      NVMemory mem;
      mem.open(invalidFile2);
   }
   catch (NVMemory::InvalidFile)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void is_open1()
{
   NVMemory mem;
   if (mem.is_open())
   {
      throw UTests::Fail();
   }
}



void close1()
{
   NVMemory mem(validFile);
   mem.close();
   if (mem.is_open())
   {
      throw UTests::Fail();
   }
}



void clear1()
{
   NVMemory mem(tmpFile);
   mem.reserve(1024);
   mem.allocate(1024);
   mem.clear();
   if (mem.size()!=0)
   {
      throw UTests::Fail();
   }
}



void reserve1()
{
   NVMemory mem(tmpFile);
   mem.reserve(1024);
   if (mem.available()!=2048)
   {
      throw UTests::Fail();
   }
}



void allocate1()
{
   NVMemory mem(tmpFile);
   if (mem.allocate(64)!=0)
   {
      throw UTests::Fail();
   }
   if (mem.allocate(64)!=64)
   {
      throw UTests::Fail();
   }
   if (mem.allocate(64)!=128)
   {
      throw UTests::Fail();
   }
   if (mem.allocate(64)!=192)
   {
      throw UTests::Fail();
   }
   if (mem.allocate(1793)!=fnullptr)
   {
      throw UTests::Fail();
   }
   if (mem.allocate(1792)!=256)
   {
      throw UTests::Fail();
   }
}



void size1()
{
   NVMemory mem(tmpFile);
   mem.reserve(1000);
   if (mem.size()!=2048)
   {
      throw UTests::Fail();
   }
}



void capacity1()
{
   NVMemory mem(tmpFile);
   if (mem.capacity()!=3048)
   {
      throw UTests::Fail();
   }
}



void available1()
{
   NVMemory mem(tmpFile);
   if (mem.available()!=1000)
   {
      throw UTests::Fail();
   }
}



void is_network_endian1()
{
   int16_t tmp {13};
   bool is_network {tmp==htons(tmp)};
   if (is_network!=NVMemory::is_network_endian())
   {
      throw UTests::Fail();
   }
}



namespace node
{



void construct1()
{
   TestNode node(16);
   if (node.mem()!=nullptr||node.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(16,mem);
   if (node.mem()!=mem||node.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void construct3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(16,mem,666);
   if (node.mem()!=mem||node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void construct4()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(16,mem,666);
   copy.init_data<char>(16);
   memcpy(&(copy.get<char>()),dataStr,16);
   TestNode node(copy);
   if (node.mem()!=mem||node.addr()!=666||copy._fData!=1||strcmp(&(copy.get<char>()),dataStr)!=0||
       strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void construct5()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(16,mem,666);
   copy.init_data<char>(16);
   memcpy(&(copy.get<char>()),dataStr,16);
   TestNode node(std::move(copy));
   if (node.mem()!=mem||node.addr()!=666||copy.mem()!=nullptr||copy.addr()!=fnullptr||
       copy._fData!=0||strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void construct6()
{
   bool caught {false};
   try
   {
      TestNode node(-1);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void construct7()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   bool caught {false};
   try
   {
      TestNode node(-1,mem,666);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void copy1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(16,mem,666);
   TestNode node(16);
   copy.init_data<char>(16);
   memcpy(&(copy.get<char>()),dataStr,16);
   node = copy;
   if (node.mem()!=mem||node.addr()!=666||copy._fData!=1||strcmp(&(copy.get<char>()),dataStr)!=0||
       strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void move1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(16,mem,666);
   TestNode node(16);
   copy.init_data<char>(16);
   memcpy(&(copy.get<char>()),dataStr,16);
   node = std::move(copy);
   if (node.mem()!=mem||node.addr()!=666||copy.mem()!=nullptr||copy.addr()!=fnullptr||
       copy._fData!=0||strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void is_null_memory1()
{
   TestNode node(16);
   if (!node.is_null_memory())
   {
      throw UTests::Fail();
   }
}



void mem1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(16);
   node.mem(mem);
   if (node.mem()!=mem)
   {
      throw UTests::Fail();
   }
}



void init_mem1()
{
   NVMemory* mem {new NVMemory(validFile)};
   TestNode node(16);
   node.init_mem(mem);
   if (node.pmem()!=mem)
   {
      throw UTests::Fail();
   }
}



void rmem1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(16,mem);
   if (&(node.rmem())!=mem.get())
   {
      throw UTests::Fail();
   }
}



void rmem2()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.rmem();
   }
   catch (NVMemory::Node::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void pmem1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(16,mem);
   if (node.pmem()!=mem.get())
   {
      throw UTests::Fail();
   }
}



void pmem2()
{
   TestNode node(16);
   if (node.pmem()!=nullptr)
   {
      throw UTests::Fail();
   }
}



void addr1()
{
   TestNode node(16);
   node.addr(666);
   if (node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void allocate1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   mem->reserve(1024);
   TestNode node(16,mem);
   node.allocate();
   if (node.addr()!=0)
   {
      throw UTests::Fail();
   }
}



void allocate2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem);
   node.allocate(4);
   if (node.addr()!=16||mem->size()!=80)
   {
      throw UTests::Fail();
   }
}



void allocate3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(945,mem);
   node.allocate();
   if (node.addr()!=80||mem->size()!=1025)
   {
      throw UTests::Fail();
   }
}



void allocate4()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem);
   bool caught {false};
   try
   {
      node.allocate(0);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void allocate5()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.allocate();
   }
   catch (NVMemory::Node::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void read1()
{
   int tmp {13};
   if (tmp!=htons(tmp))
   {
      std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
      TestNode node(16,mem,0);
      node.init_data<char>(16);
      node.read();
      if (node._counter!=1)
      {
         throw UTests::Fail();
      }
   }
}



void write1()
{
   int tmp {13};
   if (tmp!=htons(tmp))
   {
      std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
      TestNode node(16,mem,0);
      node.init_data<char>(16);
      node.write();
      if (node._counter!=2)
      {
         throw UTests::Fail();
      }
   }
}



void readwrite1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem,0);
   node.init_data<char>(16);
   memcpy(&(node.get<char>()),dataStr,16);
   node.write();
   node.read();
   if (strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void readwrite2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem,0);
   node.init_data<char>(16);
   node.read();
   if (strcmp(&(node.get<char>()),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void readwrite3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem,0);
   node.init_data<char>(16);
   bool caught {false};
   try
   {
      node.read(-1);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   caught = false;
   try
   {
      node.write(-1);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void readwrite4()
{
   TestNode node(16);
   node.init_data<char>(16);
   node.addr(0);
   bool caught {false};
   try
   {
      node.read();
   }
   catch (NVMemory::Node::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   caught = false;
   try
   {
      node.write();
   }
   catch (NVMemory::Node::NullMemory)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void readwrite5()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem,0);
   bool caught {false};
   try
   {
      node.read();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   caught = false;
   try
   {
      node.write();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void readwrite6()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(16,mem);
   node.init_data<char>(16);
   bool caught {false};
   try
   {
      node.read();
   }
   catch (NVMemory::Node::NullPtr)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
   caught = false;
   try
   {
      node.write();
   }
   catch (NVMemory::Node::NullPtr)
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
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node1(16);
   TestNode node2(16);
   node2.addr(666);
   if (node1==node2)
   {
      throw UTests::Fail();
   }
   node2.addr(fnullptr);
   node2.mem(mem);
   if (node1==node2)
   {
      throw UTests::Fail();
   }
}



void operator2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node1(16);
   TestNode node2(16);
   if (node1!=node2)
   {
      throw UTests::Fail();
   }
}



void operator3()
{
   TestNode node(66);
   node.addr(600);
   ++node;
   if (node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void init_data1()
{
   TestNode node(16);
   node.init_data<char>(16);
   if (node.pget<char>()==nullptr)
   {
      throw UTests::Fail();
   }
}



void init_data2()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.init_data<char>(15);
   }
   catch (NVMemory::Node::SizeMismatch)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void init_data3()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.init_data<char>(-1);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void init_data4()
{
   TestNode node(16);
   node.init_data<char>(16);
   bool caught {false};
   try
   {
      node.init_data<char>(16);
   }
   catch (NVMemory::Node::DataExists)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void give_data1()
{
   TestNode node(16);
   char outMem[16];
   node.give_data(outMem);
   try
   {
      node.get<char>();
   }
   catch (NVMemory::Node::NullData)
   {
      throw UTests::Fail();
   }
}



void give_data2()
{
   TestNode node(16);
   char outMem[16];
   node.give_data(outMem);
   bool caught {false};
   try
   {
      node.give_data(outMem);
   }
   catch (NVMemory::Node::DataExists)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void get1()
{
   TestNode node(16);
   auto p = new char[16];
   node.give_data(p);
   if (&(node.get<char>())!=p)
   {
      throw UTests::Fail();
   }
}



void get2()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.get<char>();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void get3()
{
   TestNode node(16);
   auto p = new char[16];
   node.give_data(p);
   const TestNode& rnode {node};
   if (&(rnode.get<char>())!=p)
   {
      throw UTests::Fail();
   }
}



void get4()
{
   TestNode node(16);
   const TestNode& rnode {node};
   bool caught {false};
   try
   {
      rnode.get<char>();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void pget1()
{
   TestNode node(16);
   auto p = new char[16];
   node.give_data(p);
   if (node.pget<char>()!=p)
   {
      throw UTests::Fail();
   }
}



void pget2()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.pget<char>();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void pget3()
{
   TestNode node(16);
   auto p = new char[16];
   node.give_data(p);
   const TestNode& rnode {node};
   if (rnode.pget<char>()!=p)
   {
      throw UTests::Fail();
   }
}



void pget4()
{
   TestNode node(16);
   const TestNode& rnode {node};
   bool caught {false};
   try
   {
      rnode.pget<char>();
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void flip1()
{
   TestNode node(3);
   char* data {new char[3]{0,0,1}};
   node.give_data(data);
   node.flip(1,2);
   if (data[1]!=1||data[2]!=0)
   {
      throw UTests::Fail();
   }
}



void flip2()
{
   TestNode node(5);
   char* data {new char[5]{0,0,1,2,3}};
   node.give_data(data);
   node.flip(1,4);
   if (data[1]!=3||data[2]!=2||data[3]!=1||data[4]!=0)
   {
      throw UTests::Fail();
   }
}



void flip3()
{
   TestNode node(9);
   char* data {new char[9]{0,0,1,2,3,4,5,6,7}};
   node.give_data(data);
   node.flip(1,8);
   if (data[1]!=7||data[2]!=6||data[3]!=5||data[4]!=4||data[5]!=3||data[6]!=2||data[7]!=1||
       data[8]!=0)
   {
      throw UTests::Fail();
   }
}



void flip4()
{
   TestNode node(16);
   node.init_data<char>(16);
   bool caught {false};
   try
   {
      node.flip(0,3);
   }
   catch (NVMemory::Node::InvalidInput)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void flip5()
{
   TestNode node(16);
   bool caught {false};
   try
   {
      node.flip(0,2);
   }
   catch (NVMemory::Node::NullData)
   {
      caught = true;
   }
   if (!caught)
   {
      throw UTests::Fail();
   }
}



void flip6()
{
   TestNode node(16);
   node.init_data<char>(16);
   bool caught {false};
   try
   {
      node.flip(9,8);
   }
   catch (NVMemory::Node::OutOfRange)
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
using namespace nvmemory;



void add_nvmemory(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("NVMemory",in,out));
   run->add_test("construct1",construct1);
   run->add_test("construct2",construct2);
   run->add_test("construct3",construct3);
   run->add_test("construct4",construct4);
   run->add_test("construct5",construct5);
   run->add_test("open1",open1);
   run->add_test("open2",open2);
   run->add_test("open3",open3);
   run->add_test("open4",open4);
   run->add_test("is_open1",is_open1);
   run->add_test("close1",close1);
   run->add_test("clear1",clear1);
   run->add_test("reserve1",reserve1);
   run->add_test("allocate1",allocate1);
   run->add_test("size1",size1);
   run->add_test("capacity1",capacity1);
   run->add_test("available1",available1);
   run->add_test("is_network_endian1",is_network_endian1);
   tests.attach(run);
   run.reset(new UTests::Run("NVMemory::Node",in,out));
   run->add_test("construct1",node::construct1);
   run->add_test("construct2",node::construct2);
   run->add_test("construct3",node::construct3);
   run->add_test("construct4",node::construct4);
   run->add_test("construct5",node::construct5);
   run->add_test("construct6",node::construct6);
   run->add_test("construct7",node::construct7);
   run->add_test("copy1",node::copy1);
   run->add_test("move1",node::move1);
   run->add_test("is_null_memory1",node::is_null_memory1);
   run->add_test("mem1",node::mem1);
   run->add_test("init_mem1",node::init_mem1);
   run->add_test("rmem1",node::rmem1);
   run->add_test("rmem2",node::rmem2);
   run->add_test("pmem1",node::pmem1);
   run->add_test("pmem2",node::pmem2);
   run->add_test("addr1",node::addr1);
   run->add_test("allocate1",node::allocate1);
   run->add_test("allocate2",node::allocate2);
   run->add_test("allocate3",node::allocate3);
   run->add_test("allocate4",node::allocate4);
   run->add_test("allocate5",node::allocate5);
   run->add_test("read1",node::read1);
   run->add_test("write1",node::write1);
   run->add_test("readwrite1",node::readwrite1);
   run->add_test("readwrite2",node::readwrite2);
   run->add_test("readwrite3",node::readwrite3);
   run->add_test("readwrite4",node::readwrite4);
   run->add_test("readwrite5",node::readwrite5);
   run->add_test("readwrite6",node::readwrite6);
   run->add_test("operator1",node::operator1);
   run->add_test("operator2",node::operator2);
   run->add_test("operator3",node::operator3);
   run->add_test("init_data1",node::init_data1);
   run->add_test("init_data2",node::init_data2);
   run->add_test("init_data3",node::init_data3);
   run->add_test("init_data4",node::init_data4);
   run->add_test("give_data1",node::give_data1);
   run->add_test("give_data2",node::give_data2);
   run->add_test("get1",node::get1);
   run->add_test("get2",node::get2);
   run->add_test("get3",node::get3);
   run->add_test("get4",node::get4);
   run->add_test("pget1",node::pget1);
   run->add_test("pget2",node::pget2);
   run->add_test("pget3",node::pget3);
   run->add_test("pget4",node::pget4);
   run->add_test("flip1",node::flip1);
   run->add_test("flip2",node::flip2);
   run->add_test("flip3",node::flip3);
   run->add_test("flip4",node::flip4);
   run->add_test("flip5",node::flip5);
   run->add_test("flip6",node::flip6);
   tests.attach(run);
}
