/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#ifndef ACCELCOMPENG_EXCEPTION_H
#define ACCELCOMPENG_EXCEPTION_H
#include <typeinfo>
#include <CL/cl.h>
namespace AccelCompEng
{



/// Call assertion check one some condition that must be true, and if it is not true than a custom
/// exception structure is thrown.
/// @tparam T The custom exception class to possibly throw.
/// @param cond The condition that must be true for no exception to be thrown.
/// @param function Name of the function where the assertion is checked.
/// @param line Line number in file where assertion is checked.
/// @param detail Any additional details that describes the assertion check.
template<class T> inline void assert(bool cond, const char* function, int line,
                                     const char* detail = nullptr)
{
   if ( !cond )
   {
      throw T(typeid(T).name(),detail,function,line);
   }
}



/// Check the return status of an already called OpenCL function. If the return status is anything
/// except CL_SUCCESS than a custom exception is thrown.
/// @tparam T The custom exception class to possibly throw.
/// @param code The OpenCL return status of a called function.
/// @param function Name of the function where the assertion is checked.
/// @param line Line number in file where assertion is checked.
template<class T> inline void classert(cl_int code, const char* function, int line)
{
   static const char* clDescErrors[] = {
      "SUCCESS",
      "DEVICE_NOT_FOUND",
      "DEVICE_NOT_AVAILABLE",
      "COMPILER_NOT_AVAILABLE",
      "MEM_OBJECT_ALLOCATION_FAILURE",
      "OUT_OF_RESOURCES",
      "OUT_OF_HOST_MEMORY",
      "PROFILING_INFO_NOT_AVAILABLE",
      "MEM_COPY_OVERLAP",
      "IMAGE_FORMAT_MISMATCH",
      "IMAGE_FORMAT_NOT_SUPPORTED",
      "BUILD_PROGRAM_FAILURE",
      "MAP_FAILURE",
      "MISALIGNED_SUB_BUFFER_OFFSET",
      "EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "UNKNOWN",
      "INVALID_VALUE",
      "INVALID_DEVICE_TYPE",
      "INVALID_PLATFORM",
      "INVALID_DEVICE",
      "INVALID_CONTEXT",
      "INVALID_QUEUE_PROPERTIES",
      "INVALID_COMMAND_QUEUE",
      "INVALID_HOST_PTR",
      "INVALID_MEM_OBJECT",
      "INVALID_IMAGE_FORMAT_DESCRIPTOR",
      "INVALID_IMAGE_SIZE",
      "INVALID_SAMPLER",
      "INVALID_BINARY",
      "INVALID_BUILD_OPTIONS",
      "INVALID_PROGRAM",
      "INVALID_PROGRAM_EXECUTABLE",
      "INVALID_KERNEL_NAME",
      "INVALID_KERNEL_DEFINITION",
      "INVALID_KERNEL",
      "INVALID_ARG_INDEX",
      "INVALID_ARG_VALUE",
      "INVALID_ARG_SIZE",
      "INVALID_KERNEL_ARGS",
      "INVALID_WORK_DIMENSION",
      "INVALID_WORK_GROUP_SIZE",
      "INVALID_WORK_ITEM_SIZE",
      "INVALID_GLOBAL_OFFSET",
      "INVALID_EVENT_WAIT_LIST",
      "INVALID_EVENT",
      "INVALID_OPERATION",
      "INVALID_GL_OBJECT",
      "INVALID_BUFFER_SIZE",
      "INVALID_MIP_LEVEL",
      "INVALID_GLOBAL_WORK_SIZE",
      "INVALID_PROPERTY"
   };
   if ( code != CL_SUCCESS )
   {
      const char* tmp;
      if ( code <= 0 && code >= -64 )
      {
         tmp = clDescErrors[-code];
      }
      else
      {
         tmp = clDescErrors[15];
      }
      throw T(typeid(T).name(),tmp,function,line);
   }
}



/// @brief Exception class for ACE.
///
/// This is the primary exception class for the entire ACE program. Any custom exception structure
/// should inherit from this class as its primary parent class.
class Exception
{
public:
   /// Makes a new exception object with the given input values.
   /// @param what Name of the custom exception class being thrown.
   /// @param detail Specific details about the exception being thrown.
   /// @param function Function name where this exception is thrown.
   /// @param line File line number where this exception is thrown.
   Exception(const char* what, const char* detail, const char* function, int line) noexcept;
   ~Exception();
   Exception(const Exception&) = default;
   Exception(Exception&&) = default;
   Exception& operator=(const Exception&) = default;
   Exception& operator=(Exception&&) = default;
   /// Get function name where exception was thrown.
   /// @return Name of function.
   const char* function() const noexcept;
   /// Get line number where exception was thrown.
   /// @return Line number.
   int line() const noexcept;
   /// Get name of the specific exception type that was thrown.
   /// @return Type of exception thrown.
   const char* what() const noexcept;
   /// Get details about the specific exception that was thrown, if any.
   /// @return String to details about exception or nullptr if no details.
   const char* detail() const noexcept;
private:
   const char* _function;
   int _line;
   const char* _what;
   const char* _detail;
   mutable char* _demangle {nullptr};
};



}
#endif
