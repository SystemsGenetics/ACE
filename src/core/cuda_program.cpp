#include "cuda_program.h"
#include "cuda_kernel.h"



using namespace CUDA;
//






/*!
 * The CUDA pre- and post-strings provide some standard functionality which is
 * not provided by NVRTC. The text currently includes several math constants, as
 * well as an 'extern "C"' block for all device functions.
 */
const char* Program::CUDA_PRE =
   "#ifdef NAN                                    \n"
   "   #undef NAN                                 \n"
   "#endif                                        \n"
   "#define NAN __int_as_float(0x7fffffff)        \n"
   "#ifdef INFINITY                               \n"
   "   #undef INFINITY                            \n"
   "#endif                                        \n"
   "#define INFINITY __int_as_float(0x7f800000)   \n"
   "#define M_E            2.71828182845904523540 \n"
   "#define M_LOG2E        1.44269504088896340740 \n"
   "#define M_LOG10E       0.43429448190325182765 \n"
   "#define M_LN2          0.69314718055994530942 \n"
   "#define M_LN10         2.30258509299404568402 \n"
   "#define M_PI           3.14159265358979323846 \n"
   "#define M_PI_2         1.57079632679489661923 \n"
   "#define M_PI_4         0.78539816339744830962 \n"
   "#define M_1_PI         0.31830988618379067154 \n"
   "#define M_2_PI         0.63661977236758134308 \n"
   "#define M_2_SQRTPI     1.12837916709551257390 \n"
   "#define M_SQRT2        1.41421356237309504880 \n"
   "#define M_SQRT1_2      0.70710678118654752440 \n"
   "\n"
   "extern \"C\" {\n";






const char* Program::CUDA_POST =
   "}\n";






/*!
 * Construct a new program with the given context, list of CUDA source file paths,
 * and optional parent. If any CUDA error occurs then an exception is thrown. 
 *
 * @param paths List of file paths containing OpenCL kernel source code that this 
 *              program builds for all devices of the given context. 
 *
 * @param parent Optional parent for this new program. 
 */
Program::Program(const QStringList& paths, QObject* parent):
   QObject(parent)
{
   // Create a string containing the contents of all source files from the
   // given file paths.
   for ( auto& path : paths )
   {
      QString source {readSourceFile(path)};
      _source.append(source);
   }

   // Prepend the CUDA pre-string and append the CUDA post-string to the source
   // string.
   _source.prepend(CUDA_PRE);
   _source.append(CUDA_POST);

   // Create a new NVRTC program from the source string. If creation fails then
   // throw an exception.
   NVRTC_SAFE_CALL(nvrtcCreateProgram(&_program, _source.toLatin1().data(), nullptr, 0, nullptr, nullptr));

   // Build the program using the current CUDA context.
   build();
}






/*!
 * Releases the underlying CUDA program that this object represents. 
 */
Program::~Program()
{
   // If the NVRTC program exists then release it.
   if ( _program )
   {
      nvrtcDestroyProgram(&_program);
   }

   // If the CUDA module exists then unload it.
   if ( _module )
   {
      cuModuleUnload(_module);
   }
}






/*!
 * Reads a CUDA kernel source file with the given path, returning a string of
 * the source code. If the source file fails to open then an exception is thrown.
 *
 * @param path Path to the CUDA source code that is read.
 *
 * @return String of CUDA source code.
 */
QString Program::readSourceFile(const QString& path)
{
   // Open the source code file with the given path as read only. If opening fails
   // then throw an exception.
   QFile file(path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("System Error"));
      e.setDetails(QObject::tr("Failed opening CUDA kernel source code file %1: %2")
                   .arg(path)
                   .arg(file.errorString()));
      throw e;
   }

   // Read the entire contents of the source file into a string and return the string.
   QTextStream stream(&file);

   return stream.readAll();
}






/*!
 * Builds this CUDA program using the given device. If building fails then an 
 * exception is thrown. 
 */
void Program::build()
{
   // Define the build options for the NVRTC compiler.
   const char *options[] = {
      "--std=c++11",
#ifdef QT_DEBUG
      "--device-debug",
      "--generate-line-info"
#endif
   };
   int numOptions = sizeof(options) / sizeof(char *);

   // Build the source code of this program for the given device. If a build error
   // occurrs then throw an exception with the build log, else if a general error
   // occurs then throw a general CUDA exception.
   nvrtcResult result = nvrtcCompileProgram(_program, numOptions, options);
   if ( result == NVRTC_ERROR_COMPILATION )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("CUDA Build Failure"));
      e.setDetails(getBuildLog());
      throw e;
   }
   else if ( result != NVRTC_SUCCESS )
   {
      NVRTC_THROW_ERROR(result);
   }

   // Get the size of the PTX source, then get the PTX source data for the
   // program. If either command fails then throw an exception.
   size_t ptxSize;
   NVRTC_SAFE_CALL(nvrtcGetPTXSize(_program, &ptxSize));

   char ptx[ptxSize];
   NVRTC_SAFE_CALL(nvrtcGetPTX(_program, ptx));

   // Load a CUDA module with the given PTX source. If this command fails then
   // throw an exception.
   CUDA_SAFE_CALL(cuModuleLoadData(&_module, ptx));
}






/*!
 * Returns the build log that is generated when this program is compiled. If any
 * CUDA error occurs then an exception is thrown.
 *
 * @return The build log.
 */
QString Program::getBuildLog() const
{
   // Get the size of the build log, then get the build log for the program. If
   // either command fails then throw an exception.
   size_t logSize;
   NVRTC_SAFE_CALL(nvrtcGetProgramLogSize(_program, &logSize));

   char log[logSize];
   NVRTC_SAFE_CALL(nvrtcGetProgramLog(_program, log));

   // Return the build log as a string.
   return QString(log);
}
