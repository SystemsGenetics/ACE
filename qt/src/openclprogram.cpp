#include "openclprogram.h"
#include "openclkernel.h"
#include "exception.h"
#include "utilities.h"






OpenCLProgram::OpenCLProgram(cl_device_id deviceID, cl_context contextID
                             , cl_command_queue commandQueueID):
   _deviceID(deviceID),
   _contextID(contextID),
   _commandQueueID(commandQueueID)
{
   clRetainContext(contextID);
   clRetainCommandQueue(commandQueueID);
}






OpenCLProgram::~OpenCLProgram()
{
   if ( _id )
   {
      clReleaseProgram(*_id);
   }
   clReleaseCommandQueue(_commandQueueID);
   clReleaseContext(_contextID);
   delete _id;
}






void OpenCLProgram::addSource(const QString& source)
{
   _sources.append(source.toLatin1());
}






void OpenCLProgram::addFile(const QString& filePath)
{
   QFile file(filePath);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Cannot Open File"));
      e.out() << QObject::tr("Failed to open file %1 as read only.").arg(filePath);
      throw e;
   }
   QTextStream stream(&file);
   QString source = stream.readAll();
   _sources.append(source.toLatin1());
}






bool OpenCLProgram::compile(const QString& options)
{
   try
   {
      if ( _id )
      {
         return false;
      }
      _id = new cl_program;
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
      cl_int code;
      *_id = clCreateProgramWithSource(_contextID,codeCount,codes,codeSizes,&code);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clCreateProgramWithSource",code);
      }
      std::string a = options.toStdString();
      code = clBuildProgram(*_id,0,nullptr,a.c_str(),nullptr,nullptr);
      if ( code != CL_SUCCESS )
      {
         if ( code == OpenCL::ReturnCodes::BuildProgramFailure )
         {
            size_t strSize = 0;
            clGetProgramBuildInfo(*_id,_deviceID,CL_PROGRAM_BUILD_LOG,0,nullptr,&strSize);
            char buffer[strSize];
            clGetProgramBuildInfo(*_id,_deviceID,CL_PROGRAM_BUILD_LOG,strSize,buffer,nullptr);
            MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("OpenCL Build Error"));
            e.out() << buffer;
            throw e;
         }
         else
         {
            OpenCL::throwError("clBuildProgram",code);
         }
      }
      return true;
   }
   catch (...)
   {
      delete _id;
      _id = nullptr;
      throw;
   }
}






unique_ptr<OpenCLKernel> OpenCLProgram::makeKernel(const QString& name)
{
   if ( !_id )
   {
      return nullptr;
   }
   return unique_ptr<OpenCLKernel>(new OpenCLKernel(*_id,_commandQueueID,_deviceID,name));
}
