#include "unit.h"
#include "../src/nvmemory.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>



namespace nvmemory
{



using namespace AccelCompEng;



const char* tmpFile = "memfile.tmp";
const char* validFile = "memfile2.tmp";
const char* invalidFile = "notmemfile.tmp";
const char* invalidFile2 = "notmemfile2.tmp";



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



void open1() {}
void open2() {}
void open3() {}
void open4() {}



}
using namespace nvmemory;



void add_nvmemory(UTests& tests)
{
   std::shared_ptr<UTests::Run> run(new UTests::Run("nvmemory",in,out));
   run->add_test("construct1",construct1);
   run->add_test("construct2",construct2);
   run->add_test("construct3",construct3);
   run->add_test("construct4",construct4);
   run->add_test("construct5",construct5);
   tests.attach(run);
}
