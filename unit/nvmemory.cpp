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
}
