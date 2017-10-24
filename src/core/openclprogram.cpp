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
   // make sure program isn't already compiled
   if ( _id )
   {
      return;
   }

   // add new source to list
   _sources.append(source.toLatin1());
}






bool EOpenCLProgram::addFile(const QString& filePath)
{
   // make sure program isn't already compiled
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
   _buildError.clear();
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
         _buildError = buffer;
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
   // make sure program is compiled and in ok state
   if ( !_id || getStatus() != Ok )
   {
      return nullptr;
   }

   // return new kernel
   return unique_ptr<EOpenCLKernel>(new EOpenCLKernel(*_id,_commandQueueID,_deviceID,name));
}






bool EOpenCLProgram::hasBuildError() const
{
   return _buildError.size() > 0;
}






QString EOpenCLProgram::getBuildError() const
{
   return _buildError;
}
