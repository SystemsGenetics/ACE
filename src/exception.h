/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <sstream>
#define ACE_EXCEPTION(N,X) struct X : public ::AccelCompEng::Exception\
                           {\
                              X(int line):\
                                 Exception(#N,#X,line)\
                              {}\
                           };
namespace AccelCompEng
{



template<class T> inline void assert(bool cond, int line)
{
   if (!cond)
   {
      throw T(line);
   }
}



class Exception
{
public:
   using string = std::string;
   using ostream = std::ostringstream;
   // *
   // * BASIC METHODS
   // *
   Exception() = default;
   Exception(const string&,const string&,int);
   // *
   // * FUNCTIONS
   // *
   int line() const;
   const string& domain() const;
   const string& what() const;
   const string& detail() const;
   Exception& operator<<(short);
   Exception& operator<<(unsigned short);
   Exception& operator<<(int);
   Exception& operator<<(unsigned int);
   Exception& operator<<(long);
   Exception& operator<<(unsigned long);
   Exception& operator<<(float);
   Exception& operator<<(double);
   Exception& operator<<(char);
   Exception& operator<<(const char*);
   Exception& operator<<(const string&);
private:
   // *
   // * VARIABLES
   // *
   int _line {0};
   string _domain;
   string _what;
   string _detail;
};



}
#endif
