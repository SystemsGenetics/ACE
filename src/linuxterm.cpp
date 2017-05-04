#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <sys/ioctl.h>
#include <cstring>
#include <cstdio>
#include "linuxterm.h"
#include "exception.h"
namespace AccelCompEng
{



bool LinuxTerm::_lock {false};
bool LinuxTerm::_cooked {true};



void LinuxTerm::stty_raw()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(_cooked,f,__LINE__);
   struct termios term = {0};
   bool cond;
   cond = tcgetattr(0,&term)>=0;
   assert<SystemError>(cond,f,__LINE__);
   term.c_lflag &= ~ICANON;
   term.c_lflag &= ~ECHO;
   term.c_cc[VMIN] = 1;
   term.c_cc[VTIME] = 0;
   cond = tcsetattr(0,TCSANOW,&term)>=0;
   assert<SystemError>(cond,f,__LINE__);
   _cooked = false;
}



void LinuxTerm::stty_cooked()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(!_cooked,f,__LINE__);
   assert<InvalidUse>(!_lock,f,__LINE__);
   struct termios term = {0};
   bool cond;
   cond = tcgetattr(0,&term)>=0;
   assert<SystemError>(cond,f,__LINE__);
   term.c_lflag |= ICANON;
   term.c_lflag |= ECHO;
   cond = tcsetattr(0,TCSANOW,&term)>=0;
   assert<SystemError>(cond,f,__LINE__);
   _cooked = true;
}



LinuxTerm::LinuxTerm():
   _i {_line.end()},
   _chCount {0}
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(!_lock,f,__LINE__);
   assert<InvalidUse>(!_cooked,f,__LINE__);
   _lock = true;
#ifdef TIOCGSIZE
   struct ttysize ts;
   ioctl(STDIN_FILENO,TIOCGSIZE,&ts);
   int _cols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
   struct winsize ts;
   ioctl(STDIN_FILENO,TIOCGWINSZ,&ts);
   _cols = ts.ws_col;
#endif
}



LinuxTerm::~LinuxTerm()
{
   _lock = false;
}



void LinuxTerm::header(const std::string& header)
{
   _header = header;
   _header.append(" ");
}



void LinuxTerm::precision(int)
{}



LinuxTerm& LinuxTerm::operator<<(short n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(unsigned short n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(int n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(unsigned int n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(long n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(unsigned long n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(float n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(double n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(const char* n)
{
   std::cout << n;
   return *this;
}
LinuxTerm& LinuxTerm::operator<<(const std::string& n)
{
   std::cout << n;
   return *this;
}



void LinuxTerm::operator>>(std::string& buffer)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(!_cooked,f,__LINE__);
   buffer.clear();
   _line.clear();
   _i = _line.end();
   reprint(false);
   char input;
   while ( (input = getchar()) != '\n' )
   {
      if ( input >= ' ' && input <= '~' )
      {
         _i = _line.insert(_i,input);
         _i++;
      }
      else
      {
         switch (input)
         {
         case _backspaceCh:
            if (_i!=_line.begin())
            {
               _i = _line.erase(--_i);
            }
            break;
         case _escapeCh:
            if ( getchar() == '[' )
            {
               switch(getchar())
               {
               case '3':
                  if (getchar()=='~'&&_i!=_line.end())
                  {
                     _i = _line.erase(_i);
                  }
                  break;
               case _arrowRightCh:
                  if (_i!=_line.end())
                  {
                     _i++;
                  }
                  break;
               case _arrowLeftCh:
                  if (_i!=_line.begin())
                  {
                     _i--;
                  }
                  break;
               }
            }
            break;
         }
      }
      reprint(true);
   }
   for (auto i:_line)
   {
      buffer += i;
   }
}



void LinuxTerm::set_ops(Ops op)
{
   switch (op)
   {
   case Ops::newline:
      std::cout << "\n";
      break;
   case Ops::flush:
      std::cout << std::flush;
      break;
   }
}



void LinuxTerm::reset_cursor(int chCount)
{
   for (int i = 0; i < (chCount/_cols) ;i++)
   {
      std::cout << _cursorUpStr;
   }
   std::cout << "\r";
}



void LinuxTerm::reprint(bool rewind)
{
   if ( rewind )
   {
      reset_cursor(_chCount-1);
   }
   std::cout << _header;
   for (auto i:_line)
   {
      std::cout << i;
   }
   std::cout << " ";
   reset_cursor(_line.size()+_header.size());
   _chCount = _header.size();
   std::cout << _boldTextStr;
   std::cout << _header;
   std::cout << _normTextStr;
   for (auto i = _line.begin(); i != _i ;i++)
   {
      std::cout << *i;
      _chCount++;
   }
   std::cout << std::flush;
}



}
