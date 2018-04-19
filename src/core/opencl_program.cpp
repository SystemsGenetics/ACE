#include "opencl_program.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 *
 * @param context  
 *
 * @param paths  
 *
 * @param parent  
 */
Program::Program(Context* context, const QStringList& paths, QObject* parent):
   QObject(parent)
{
   const char* sources[paths.size()] {nullptr};
   try
   {
      size_t sizes[paths.size()];
      for (int i = 0; i < paths.size() ;++i)
      {
         sources[i] = readSourceFile(paths.at(i),&sizes[i]);
      }
      cl_int code;
      _id = clCreateProgramWithSource(context->id(),paths.size(),sources,sizes,&code);
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
      for (auto device: context->devices())
      {
         compile(device);
      }
      deleteStrings(sources,paths.size());
   }
   catch (...)
   {
      deleteStrings(sources,paths.size());
      throw;
   }
}






/*!
 */
Program::~Program()
{
   clReleaseProgram(_id);
}






/*!
 */
cl_program Program::id() const
{
   return _id;
}






/*!
 *
 * @param path  
 *
 * @param size  
 */
char* Program::readSourceFile(const QString& path, size_t* size)
{
   QFile file(path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed opening OpenCL kernel source code file %1: %2")
                   .arg(path)
                   .arg(file.errorString()));
      throw e;
   }
   QByteArray source {file.readAll()};
   char* string {new char[source.size()]};
   memcpy(string,source.data(),source.size());
   *size = source.size();
   return string;
}






/*!
 *
 * @param sources  
 *
 * @param size  
 */
void Program::deleteStrings(const char** sources, int size)
{
   for (int i = 0; i < size ;++i)
   {
      delete[] sources[i];
   }
}






/*!
 *
 * @param device  
 */
void Program::compile(Device* device)
{
   cl_device_id id {device->id()};
   cl_int code {clBuildProgram(_id,1,&id,"",nullptr,nullptr)};
   if ( code == CL_BUILD_PROGRAM_FAILURE )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("OpenCL Build Failure"));
      e.setDetails(getBuildLog(device));
      throw e;
   }
   else if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 *
 * @param device  
 */
QString Program::getBuildLog(Device* device) const
{
   size_t size;
   cl_int code {clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,0,nullptr,&size)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   char log[size];
   code = clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,size,log,nullptr);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return QString(log);
}
