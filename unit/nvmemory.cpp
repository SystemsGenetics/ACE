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
   using Node::get_ptr;
   using Node::resize;
   int _counter {0};
private:
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
   FileMem::Ptr next = 0;
   fd = open(validFile,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(FileMem::Ptr));
   close(fd);
   fd = open(invalidFile,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(FileMem::Ptr)-1);
   close(fd);
   identString[1] = '\16';
   fd = open(invalidFile2,flags,modes);
   ::write(fd,identString,idLen);
   ::write(fd,&next,sizeof(FileMem::Ptr));
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
   TestNode node;
   if (node.mem()!=nullptr||node.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void construct2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(mem);
   if (node.mem()!=mem||node.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void construct3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node(mem,666);
   if (node.mem()!=mem||node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void construct4()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(mem,666);
   TestNode node(copy);
   if (node.mem()!=mem||node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void construct5()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(mem,666);
   TestNode node(std::move(copy));
   if (node.mem()!=mem||node.addr()!=666||copy.mem()!=nullptr||copy.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void copy1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(mem,666);
   TestNode node;
   node = copy;
   if (node.mem()!=mem||node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void move1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode copy(mem,666);
   TestNode node;
   node = std::move(copy);
   if (node.mem()!=mem||node.addr()!=666||copy.mem()!=nullptr||copy.addr()!=fnullptr)
   {
      throw UTests::Fail();
   }
}



void mem1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node;
   node.mem(mem);
   if (node.mem()!=mem)
   {
      throw UTests::Fail();
   }
}



void rmem1()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(validFile));
   TestNode node;
   node.mem(mem);
   if (&(node.rmem())!=mem.get())
   {
      throw UTests::Fail();
   }
}



void addr1()
{
   TestNode node;
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
   TestNode node(mem);
   node.resize(16);
   node.allocate();
   if (node.addr()!=0)
   {
      throw UTests::Fail();
   }
}



void allocate2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem);
   node.resize(16);
   node.allocate(4);
   if (node.addr()!=16||mem->size()!=80)
   {
      throw UTests::Fail();
   }
}



void allocate3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem);
   node.resize(945);
   node.allocate();
   if (node.addr()!=80||mem->size()!=1025)
   {
      throw UTests::Fail();
   }
}



void allocate4()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem);
   node.resize(16);
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
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem);
   bool caught {false};
   try
   {
      node.allocate();
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



void allocate6()
{
   TestNode node;
   node.resize(16);
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
      TestNode node(mem,0);
      node.resize(16);
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
      TestNode node(mem,0);
      node.resize(16);
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
   TestNode node(mem,0);
   node.resize(16);
   memcpy(node.get_ptr<char>(),dataStr,16);
   node.write();
   node.read();
   if (strcmp(node.get_ptr<char>(),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void readwrite2()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem,0);
   node.resize(16);
   node.read();
   if (strcmp(node.get_ptr<char>(),dataStr)!=0)
   {
      throw UTests::Fail();
   }
}



void readwrite3()
{
   std::shared_ptr<NVMemory> mem(new NVMemory(tmpFile));
   TestNode node(mem,0);
   node.resize(16);
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
   TestNode node;
   node.addr(0);
   node.resize(16);
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
   TestNode node(mem,0);
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
   TestNode node(mem);
   node.resize(16);
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
   TestNode node1;
   TestNode node2;
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
   TestNode node1;
   TestNode node2;
   if (node1!=node2)
   {
      throw UTests::Fail();
   }
}



void operator3()
{
   TestNode node;
   node.addr(600);
   node.resize(66);
   ++node;
   if (node.addr()!=666)
   {
      throw UTests::Fail();
   }
}



void get_ptr1()
{
   TestNode node;
   node.resize(16);
   if (node.get_ptr<void>()==nullptr)
   {
      throw UTests::Fail();
   }
}



void resize1()
{
   TestNode node;
   node.resize(16);
   sprintf(node.get_ptr<char>(),"123456789012345");
   void* oldptr {node.get_ptr<void>()};
   node.resize(16,true);
   if (oldptr==node.get_ptr<void>()||strcmp("123456789012345",node.get_ptr<char>())!=0)
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
   run->add_test("copy1",node::copy1);
   run->add_test("move1",node::move1);
   run->add_test("mem1",node::mem1);
   run->add_test("rmem1",node::mem1);
   run->add_test("addr1",node::addr1);
   run->add_test("allocate1",node::allocate1);
   run->add_test("allocate2",node::allocate2);
   run->add_test("allocate3",node::allocate3);
   run->add_test("allocate4",node::allocate4);
   run->add_test("allocate5",node::allocate5);
   run->add_test("allocate6",node::allocate6);
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
   run->add_test("get_ptr1",node::get_ptr1);
   run->add_test("resize1",node::resize1);
   tests.attach(run);
}
