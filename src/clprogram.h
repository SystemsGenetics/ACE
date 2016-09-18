#ifndef ACCELCOMPENG_CLPROGRAM_H
#define ACCELCOMPENG_CLPROGRAM_H
#include <CL/cl.h>
#include <string>
#include <vector>
#include "exception.h"
#include "clkernel.h"
namespace AccelCompEng
{



/// Wrapper for OpenCL program.
class CLProgram
{
public:
   struct NoSuchFile : public Exception { using Exception::Exception; };
   struct NotCompiled : public Exception { using Exception::Exception; };
   struct NotInitialized : public Exception { using Exception::Exception; };
   struct CannotBind : public Exception { using Exception::Exception; };
   struct BuildInfoFail : public Exception { using Exception::Exception; };
   struct CannotFindKern : public Exception { using Exception::Exception; };
   /// Create empty or null object that is not initialized.
   CLProgram() = default;
   ~CLProgram();
   CLProgram(const CLProgram&) = delete;
   CLProgram& operator=(const CLProgram&) = delete;
   CLProgram(CLProgram&&) = delete;
   CLProgram& operator=(CLProgram&&) = delete;
   /// Initialize object with given OpenCL context and device.
   ///
   /// @param cid OpenCL context id.
   /// @param did OpenCL device id.
   void init(cl_context cid, cl_device_id did);
protected:
   /// Add OpenCL kernel source code from direct input or a file.
   ///
   /// @param input Contains either string of source code itself or name of file where source code
   /// exists, depending on second argument.
   /// @param file Is the input string a filename? If not input is raw source code.
   void add_source(const std::string& input, bool file = false);
   /// Compile OpenCL source code contained in object with given options.
   ///
   /// @param options Additional options to pass to the compiler.
   /// @return Was the compilation of source code successful?
   bool compile(const std::string& options);
   /// Return compilation log from last compile command.
   ///
   /// @return Compile log.
   std::string log();
   /// Create a new OpenCL kernel from the compiled kernels of the object.
   ///
   /// @param name Name of the kernel in source.
   /// @return New OpenCL kernel.
   CLKernel mkernel(const std::string& name);
private:
   bool _initd {false};
   bool _binded {false};
   bool _compiled {false};
   cl_program _id;
   cl_device_id _did;
   cl_context _cid;
   std::vector<std::string> _src;
};



}
#endif
