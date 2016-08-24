#include <fstream>
#include <sstream>
#include "clprogram.h"
namespace AccelCompEng
{



CLProgram::~CLProgram()
{
   if (_binded)
   {
      clReleaseProgram(_id);
   }
}



void CLProgram::init(cl_context cid, cl_device_id did)
{
   _cid = cid;
   _did = did;
   _initd = true;
}



void CLProgram::add_source(const string& input, bool file)
{
   assert<NotInitialized>(_initd,__LINE__);
   if (file)
   {
      std::ifstream sourceFile(input.c_str());
      assert<NoSuchFile>(sourceFile.is_open(),__LINE__);
      std::stringstream buffer;
      buffer << sourceFile.rdbuf();
      _src.push_back(buffer.str());
   }
   else
   {
      _src.push_back(input);
   }
}



bool CLProgram::compile(const string& options)
{
   assert<NotInitialized>(_initd,__LINE__);
   bool ret = false;
   cl_uint s = _src.size();
   const char *codes[s];
   size_t codeSzs[s];
   for (int i=0;i<s;i++)
   {
      codeSzs[i] = _src[i].size()+1;
      codes[i] = _src[i].c_str();
   }
   if (!_binded)
   {
      cl_int err;
      _id = clCreateProgramWithSource(_cid,s,codes,codeSzs,&err);
      classert<CannotBind>(err,__LINE__);
      _binded = true;
   }
   if (clBuildProgram(_id,0,NULL,options.c_str(),NULL,NULL)>=0)
   {
      _compiled = true;
      ret = true;
   }
   return ret;
}



CLProgram::string CLProgram::log()
{
   assert<NotInitialized>(_initd,__LINE__);
   std::string ret {"OpenCL program not yet binded!"};
   if (_binded)
   {
      size_t strSize = 0;
      cl_int err = clGetProgramBuildInfo(_id,_did,CL_PROGRAM_BUILD_LOG,0,NULL,
                                         &strSize);
      classert<BuildInfoFail>(err,__LINE__);
      char buffer[strSize];
      err = clGetProgramBuildInfo(_id,_did,CL_PROGRAM_BUILD_LOG,strSize,buffer,
                                  NULL);
      classert<BuildInfoFail>(err,__LINE__);
      ret = buffer;
   }
   return ret;
}



CLKernel CLProgram::mkernel(const string& name)
{
   assert<NotInitialized>(_initd,__LINE__);
   assert<NotCompiled>(_compiled,__LINE__);
   cl_int err;
   CLKernel ret(clCreateKernel(_id,name.c_str(),&err),_did);
   classert<CannotFindKern>(err,__LINE__);
   return ret;
}



}
