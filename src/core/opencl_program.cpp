#include "opencl_program.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"



namespace OpenCL
{






/*!
 * Construct a new program with the given context, list of OpenCL kernel source
 * code file paths, and optional parent. If any OpenCL error occurs then an
 * exception is thrown.
 *
 * @param context Pointer to context this program is created from.
 *
 * @param paths List of file paths containing OpenCL kernel source code that
 *              this program builds for all devices of the given context.
 *
 * @param parent Optional parent for this new program.
 */
Program::Program(Context* context, const QStringList& paths, QObject* parent)
   :
   QObject(parent)
{
   // Add the debug header.
   EDEBUG_FUNC(this,context,paths,parent);

   // Create the list of kernel source files that will be compiled with the given
   // context.
   QVector<const char*> sources(paths.size(),nullptr);
   try
   {
      // Build the array of C style strings containing the contents of all source files
      // from the given file paths.
      QVector<size_t> sizes(paths.size());
      for (int i = 0; i < paths.size() ;++i)
      {
         sources[i] = readSourceFile(paths.at(i),&sizes[i]);
      }

      // Create a new OpenCL program from the list of C style strings of source code. If
      // creation fails then throw an exception.
      cl_int code;
      _id = clCreateProgramWithSource(context->id()
                                      ,static_cast<cl_uint>(paths.size())
                                      ,sources.data()
                                      ,sizes.data()
                                      ,&code);
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }

      // Iterate through all devices of the given context and build the source code for
      // each device.
      for (auto device: context->devices())
      {
         build(device);
      }

      // delete the list of C style strings containing the source code.
      for (auto ptr: sources) delete[] ptr;
   }
   catch (...)
   {
      for (auto ptr: sources) delete[] ptr;
      throw;
   }
}






/*!
 * Releases the underlying OpenCL program that this object represents.
 */
Program::~Program()
{
   EDEBUG_FUNC(this);
   clReleaseProgram(_id);
}






/*!
 * Returns the OpenCL program ID of this object.
 *
 * @return OpenCL program ID of this object.
 */
cl_program Program::id() const
{
   EDEBUG_FUNC(this);
   return _id;
}






/*!
 * Reads in an OpenCL kernel source file with the given path, returning a C
 * style string of the read in source code and setting the string's size with
 * the given pointer. If the source file fails to open then an exception is
 * thrown.
 *
 * @param path Path to the OpenCL source code that is read in.
 *
 * @param size Pointer to size variable that is set to the length of the C style
 *             string returned.
 *
 * @return Pointer to C style string of OpenCL source code.
 */
char* Program::readSourceFile(const QString& path, size_t* size)
{
   EDEBUG_FUNC(this,path,size)

   // Open the source code file with the given path as read only. If opening fails
   // then throw an exception.
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

   // Read in the entire contents of the source file, saving it as a C style string
   // and return a pointer to the C style string.
   QByteArray source {file.readAll()};
   char* string {new char[source.size()]};
   memcpy(string,source.data(),static_cast<size_t>(source.size()));
   *size = static_cast<size_t>(source.size());
   return string;
}






/*!
 * Builds an OpenCL program using the given device. If building fails then an
 * exception is thrown.
 *
 * @param device Pointer to device that is used to build all the source code.
 */
void Program::build(Device* device)
{
   EDEBUG_FUNC(this,device)

   // Build the source code of this program for the given device. If a build error
   // occurred then throw an exception with the build log, else if a general error
   // occurs then throw a general OpenCL exception.
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
 * Returns the most recent build log from the given device that is generated
 * whenever compiling is attempted. If any OpenCL error occurs then an exception
 * is thrown.
 *
 * @param device Pointer to device whose most recent build log is returned.
 *
 * @return Most recent build log of the given device.
 */
QString Program::getBuildLog(Device* device) const
{
   EDEBUG_FUNC(this,device)

   // Get the size of the given device's build log, then get the C style string of
   // the given device's build log, and then return the build log as a qt string. If
   // any error occurs then throw an OpenCL exception.
   size_t size;
   cl_int code {clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,0,nullptr,&size)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   Q_ASSERT(size < std::numeric_limits<int>::max());
   QByteArray log(static_cast<int>(size),'\0');
   code = clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,size,log.data(),nullptr);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return log;
}

}
