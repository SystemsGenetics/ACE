#include "openclprogram.h"
#include "openclkernel.h"
#include "exception.h"



using namespace std;






EOpenCLProgram::EOpenCLProgram(cl_device_id deviceID, cl_context contextID
                             , cl_command_queue commandQueueID):
   _deviceID(deviceID),
   _contextID(contextID),
   _commandQueueID(commandQueueID)
{
   clRetainContext(contextID);
   clRetainCommandQueue(commandQueueID);
}






EOpenCLProgram::~EOpenCLProgram()
{
   // if program exists release it
   if ( _id )
   {
      clReleaseProgram(*_id);
   }

   // release other opencl objects
   clReleaseCommandQueue(_commandQueueID);
   clReleaseContext(_contextID);
}






void EOpenCLProgram::addSource(const QString& source)
{
   if ( _id )
   {
      return;
   }
   _sources.append(source.toLatin1());
}






bool EOpenCLProgram::addFile(const QString& filePath)
{
   if ( _id )
   {
      return false;
   }
   // open given file
   QFile file(filePath);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      return false;
   }

   // read entire file and add as source
   QTextStream stream(&file);
   QString source = stream.readAll();
   _sources.append(source.toLatin1());
   return true;
}






bool EOpenCLProgram::compile(const QString& options)
{
   // if program already compiled do nothing
   if ( _id || getStatus() != Ok )
   {
      return false;
   }

   // allocate new program ID and build source arrays
   _id.reset(new cl_program);
   cl_uint codeCount = _sources.size();
   const char* codes[codeCount];
   size_t codeSizes[codeCount];
   for (unsigned int i = 0; i < codeCount ;++i)
   {
      codes[i] = nullptr;
   }
   for (unsigned int i = 0; i < codeCount ;++i)
   {
      codeSizes[i] = _sources[i].size();
      codes[i] = _sources[i].data();
   }

   // create new program with built source arrays
   cl_int code;
   *_id = clCreateProgramWithSource(_contextID,codeCount,codes,codeSizes,&code);
   if ( code != CL_SUCCESS )
   {
      reportError("clCreateProgramWithSource",code);
      return false;
   }

   // compile program
   std::string a = options.toStdString();
   code = clBuildProgram(*_id,0,nullptr,a.c_str(),nullptr,nullptr);
   if ( code != CL_SUCCESS )
   {
      // if a build error occured while compiling then report build log as error
      if ( code == Ace::OpenCL::ReturnCodes::BuildProgramFailure )
      {
         size_t strSize = 0;
         code = clGetProgramBuildInfo(*_id,_deviceID,CL_PROGRAM_BUILD_LOG,0,nullptr,&strSize);
         if ( code != CL_SUCCESS )
         {
            reportError("clGetProgramBuildInfo",code);
            return false;
         }
         char buffer[strSize];
         code = clGetProgramBuildInfo(*_id,_deviceID,CL_PROGRAM_BUILD_LOG,strSize,buffer,nullptr);
         if ( code != CL_SUCCESS )
         {
            reportError("clGetProgramBuildInfo",code);
            return false;
         }
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("OpenCL Build Error"));
         e.out() << buffer;
         e.display();
         return false;
      }
      else
      {
         // else another type of error occured and report normally
         reportError("clBuildProgram",code);
         return false;
      }
   }
   return true;
}






unique_ptr<EOpenCLKernel> EOpenCLProgram::makeKernel(const QString& name)
{
   if ( !_id || getStatus() != Ok )
   {
      return nullptr;
   }
   return unique_ptr<EOpenCLKernel>(new EOpenCLKernel(*_id,_commandQueueID,_deviceID,name));
}
