#include "../src/file.h"
#include "unit.h"
using namespace AccelCompEng;



namespace unit
{
   namespace file
   {
      class PublicFile : public File
      {
      public:
         using File::File;
         using File::ident;
         using File::head;
      };
      constexpr auto tmpFile = "File.tmp";
      constexpr auto tmpFile2 = "File2.tmp";
      constexpr auto tmpFile3 = "File3.tmp";
      constexpr auto invalidFile = "notFile.tmp";
      constexpr auto invalidFile2 = "notFile2.tmp";
      constexpr auto invalidFile3 = "notFile3.tmp";
      constexpr const char* identStr = "1234567890123456";
      constexpr int dataPtr = 9999;
      constexpr int tStamp = 8888;
   }
}



void construct_Files()
{
   {
      FileMem tf(unit::file::tmpFile);
      FileData::Header header;
      tf.allot(header);
      History hist(tf);
      hist.timeStamp(unit::file::tStamp);
      hist.sync();
      header.histHead() = hist.addr();
      header.dataHead() = unit::file::dataPtr;
      FString ident(&tf);
      ident = unit::file::identStr;
      header.ident() = ident.addr();
      memcpy(header.idString(),FileData::idString,FileData::idSz);
      tf.sync(header,FileSync::write);
   }
   {
      FileMem tf(unit::file::invalidFile);
      FileData::Header header;
      tf.allot(header);
      History hist(tf);
      header.histHead() = hist.addr();
      char buffer[FileData::idSz] {'\0'};
      memcpy(header.idString(),buffer,FileData::idSz);
      tf.sync(header,FileSync::write);
   }
   {
      FileMem tf(unit::file::invalidFile2);
      FileData::Header header;
      tf.allot(header);
      header.histHead() = FileMem::nullPtr;
      memcpy(header.idString(),FileData::idString,FileData::idSz);
      tf.sync(header,FileSync::write);
   }
   {
      FileMem tf(unit::file::invalidFile3);
      FString data(&tf);
      data = "invalid";
   }
}



bool unit::file::main()
{
   bool ret = false;
   header("File");
   try
   {
      construct_Files();
      ret = construct()&&
            history()&&
            clear()&&
            is_new()&&
            ident()&&
            head();
   }
   catch (...)
   {
      system("rm -f *.tmp");
      unit::end();
      throw;
   }
   system("rm -f *.tmp");
   unit::end();
   return ret;
}



bool unit::file::construct()
{
   bool cont = true;
   {
      start();
      File t(tmpFile2);
      bool test = t.is_new();
      cont = cont&&finish(test,"construct1");
   }
   if (cont)
   {
      start();
      File t(tmpFile2);
      bool test = !t.is_new();
      cont = cont&&finish(test,"construct2");
   }
   if (cont)
   {
      start();
      File t(tmpFile);
      bool test = !t.is_new();
      cont = cont&&finish(test,"construct3");
   }
   if (cont)
   {
      start();
      bool test = false;
      try
      {
         File t(invalidFile);
      }
      catch (File::InvalidFile)
      {
         test = true;
      }
      cont = cont&&finish(test,"construct4");
   }
   if (cont)
   {
      start();
      bool test = false;
      try
      {
         File t(invalidFile2);
      }
      catch (File::InvalidFile)
      {
         test = true;
      }
      cont = cont&&finish(test,"construct5");
   }
   if (cont)
   {
      start();
      bool test = false;
      try
      {
         File t(invalidFile3);
      }
      catch (File::InvalidFile)
      {
         test = true;
      }
      cont = cont&&finish(test,"construct6");
   }
   return cont;
}



bool unit::file::history()
{
   start();
   File t(tmpFile);
   bool test = t.history().timeStamp()==tStamp;
   return finish(test,"history1");
}



bool unit::file::clear()
{
   start();
   File t(tmpFile2);
   t.history().timeStamp(6666);
   t.history().sync();
   t.clear();
   bool test = t.history().timeStamp()==0;
   return finish(test,"clear1");
}



bool unit::file::is_new()
{
   bool cont {true};
   {
      start();
      File t(tmpFile3);
      bool test = t.is_new();
      cont = cont&&finish(test,"is_new1");
   }
   if (cont)
   {
      start();
      File t(tmpFile3);
      bool test = !t.is_new();
      cont = cont&&finish(test,"is_new2");
   }
   if (cont)
   {
      start();
      File t(tmpFile3);
      t.clear();
      bool test = t.is_new();
      cont = cont&&finish(test,"is_new3");
   }
   return cont;
}



bool unit::file::ident()
{
   std::string ident(identStr);
   bool cont = true;
   {
      start();
      PublicFile t(tmpFile);
      bool test = t.ident()==ident;
      cont = cont&&finish(test,"ident1");
   }
   if (cont)
   {
      start();
      {
         PublicFile t(tmpFile2);
         t.ident(ident);
      }
      PublicFile t(tmpFile2);
      bool test = t.ident()==ident;
      cont = cont&&finish(test,"ident2");
   }
   if (cont)
   {
      start();
      PublicFile t(tmpFile2);
      bool test = false;
      try
      {
         t.ident(ident);
      }
      catch (File::AlreadySet)
      {
         test = true;
      }
      cont = cont&&finish(test,"ident3");
   }
   return cont;
}



bool unit::file::head()
{
   bool cont = true;
   {
      start();
      PublicFile t(tmpFile);
      bool test = t.head()==dataPtr;
      cont = cont&&finish(test,"head1");
   }
   if (cont)
   {
      start();
      {
         PublicFile t(tmpFile2);
         t.head(dataPtr);
      }
      PublicFile t(tmpFile2);
      bool test = t.head()==dataPtr;
      cont = cont&&finish(test,"head2");
   }
   return cont;
}
