#include "cuda_program.h"
#include "cuda_kernel.h"



using namespace CUDA;






/**
 * The CUDA pre- and post-strings provide some standard functionality which is
 * not provided by NVRTC. The text currently includes several math constants, as
 * well as an 'extern "C"' block for all device functions.
 */
const char* CUDA_PRE =
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






const char* CUDA_POST =
   "}\n";






/*!
 * Construct a new program with the given context, list of CUDA source file paths,
 * and parent.
 *
 * @param paths
 * @param parent
 */
Program::Program(const QStringList& paths, QObject* parent):
   QObject(parent)
{
   // append each source file to program source
   for ( auto& path : paths )
   {
      QString source {readSourceFile(path)};
      _source.append(source);
   }

   // insert the CUDA pre- and post-strings
   _source.prepend(CUDA_PRE);
   _source.append(CUDA_POST);

   // create new program with source
   NVRTC_SAFE_CALL(nvrtcCreateProgram(&_program, _source.toLatin1().data(), nullptr, 0, nullptr, nullptr));

   // build program on current context
   build();
}






Program::~Program()
{
   // if program exists release it
   if ( _program )
   {
      NVRTC_SAFE_CALL(nvrtcDestroyProgram(&_program));
   }

   // if module exists unload it
   if ( _module )
   {
      CUDA_SAFE_CALL(cuModuleUnload(_module));
   }
}






/*!
 * Reads a CUDA kernel source file with the given path. If the source file
 * fails to open then an exception is thrown.
 *
 * @param path
 */
QString Program::readSourceFile(const QString& path)
{
   // open file
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

   // read entire file and return source
   QTextStream stream(&file);

   return stream.readAll();
}






/*!
 * Build CUDA program.
 */
void Program::build()
{
   // compile program
   const char *options[] = {
      "--std=c++11",
#ifdef QT_DEBUG
      "--device-debug",
      "--generate-line-info"
#endif
   };
   int numOptions = sizeof(options) / sizeof(char *);

   nvrtcResult result = nvrtcCompileProgram(_program, numOptions, options);

   if ( result == NVRTC_ERROR_COMPILATION )
   {
      // if a build error occured while compiling then report build log as error
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("CUDA Build Failure"));
      e.setDetails(getBuildLog());
      throw e;
   }
   else if ( result != NVRTC_SUCCESS )
   {
      // else another type of error occured and handle normally
      NVRTC_THROW_ERROR(result);
   }

   // get PTX source from program
   size_t ptxSize;
   NVRTC_SAFE_CALL(nvrtcGetPTXSize(_program, &ptxSize));

   char ptx[ptxSize];
   NVRTC_SAFE_CALL(nvrtcGetPTX(_program, ptx));

   // load module from PTX source
   CUDA_SAFE_CALL(cuModuleLoadData(&_module, ptx));
}






/*!
 * Get build log from CUDA program.
 */
QString Program::getBuildLog() const
{
   size_t logSize;
   NVRTC_SAFE_CALL(nvrtcGetProgramLogSize(_program, &logSize));

   char log[logSize];
   NVRTC_SAFE_CALL(nvrtcGetProgramLog(_program, log));

   return QString(log);
}
