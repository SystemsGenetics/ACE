#ifndef ACCELCOMPENG_CLPROGRAM_H
#define ACCELCOMPENG_CLPROGRAM_H
#include <CL/cl.h>
#include <string>
#include <vector>
#include "exception.h"
#include "clkernel.h"
namespace AccelCompEng
{



class CLProgram
{
public:
   struct NoSuchFile : public Exception { using Exception::Exception; };
   struct NotCompiled : public Exception { using Exception::Exception; };
   struct NotInitialized : public Exception { using Exception::Exception; };
   struct CannotBind : public Exception { using Exception::Exception; };
   struct BuildInfoFail : public Exception { using Exception::Exception; };
   struct CannotFindKern : public Exception { using Exception::Exception; };
   CLProgram() = default;
   ~CLProgram();
   CLProgram(const CLProgram&) = delete;
   CLProgram& operator=(const CLProgram&) = delete;
   CLProgram(CLProgram&&) = delete;
   CLProgram& operator=(CLProgram&&) = delete;
   void init(cl_context,cl_device_id);
   using string = std::string;
protected:
   void add_source(const string& input, bool file = false);
   bool compile(const string&);
   string log();
   CLKernel mkernel(const string& name);
private:
   bool _initd {false};
   bool _binded {false};
   bool _compiled {false};
   cl_program _id;
   cl_device_id _did;
   cl_context _cid;
   std::vector<string> _src;
};



}
#endif
