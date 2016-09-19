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



/// @brief Call assertion check.
///
/// Call assertion check one some condition that must be true, and if it is not true than a custom
/// exception structure is thrown.
///
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



/// @brief Call OpenCL assertion check.
///
/// Check the return status of an already called OpenCL function. If the return status is anything
/// except CL_SUCCESS than a custom exception is thrown.
///
/// @tparam T The custom exception class to possibly throw.
/// @param code The OpenCL return status of a called function.
/// @param function Name of the function where the assertion is checked.
/// @param line Line number in file where assertion is checked.
template<class T> inline void classert(cl_int code, const char* function, int line)
{
   static const char* clDescErrors[] = {
      "CL_SUCCESS",
      "CL_DEVICE_NOT_FOUND",
      "CL_DEVICE_NOT_AVAILABLE",
      "CL_COMPILER_NOT_AVAILABLE",
      "CL_MEM_OBJECT_ALLOCATION_FAILURE",
      "CL_OUT_OF_RESOURCES",
      "CL_OUT_OF_HOST_MEMORY",
      "CL_PROFILING_INFO_NOT_AVAILABLE",
      "CL_MEM_COPY_OVERLAP",
      "CL_IMAGE_FORMAT_MISMATCH",
      "CL_IMAGE_FORMAT_NOT_SUPPORTED",
      "CL_BUILD_PROGRAM_FAILURE",
      "CL_MAP_FAILURE",
      "CL_MISALIGNED_SUB_BUFFER_OFFSET",
      "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_UNKNOWN",
      "CL_INVALID_VALUE",
      "CL_INVALID_DEVICE_TYPE",
      "CL_INVALID_PLATFORM",
      "CL_INVALID_DEVICE",
      "CL_INVALID_CONTEXT",
      "CL_INVALID_QUEUE_PROPERTIES",
      "CL_INVALID_COMMAND_QUEUE",
      "CL_INVALID_HOST_PTR",
      "CL_INVALID_MEM_OBJECT",
      "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
      "CL_INVALID_IMAGE_SIZE",
      "CL_INVALID_SAMPLER",
      "CL_INVALID_BINARY",
      "CL_INVALID_BUILD_OPTIONS",
      "CL_INVALID_PROGRAM",
      "CL_INVALID_PROGRAM_EXECUTABLE",
      "CL_INVALID_KERNEL_NAME",
      "CL_INVALID_KERNEL_DEFINITION",
      "CL_INVALID_KERNEL",
      "CL_INVALID_ARG_INDEX",
      "CL_INVALID_ARG_VALUE",
      "CL_INVALID_ARG_SIZE",
      "CL_INVALID_KERNEL_ARGS",
      "CL_INVALID_WORK_DIMENSION",
      "CL_INVALID_WORK_GROUP_SIZE",
      "CL_INVALID_WORK_ITEM_SIZE",
      "CL_INVALID_GLOBAL_OFFSET",
      "CL_INVALID_EVENT_WAIT_LIST",
      "CL_INVALID_EVENT",
      "CL_INVALID_OPERATION",
      "CL_INVALID_GL_OBJECT",
      "CL_INVALID_BUFFER_SIZE",
      "CL_INVALID_MIP_LEVEL",
      "CL_INVALID_GLOBAL_WORK_SIZE",
      "CL_INVALID_PROPERTY"
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
   /// @brief Create new exception.
   ///
   /// Makes a new exception object with the given input values.
   ///
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
   /// @brief Get function name.
   ///
   /// Get function name where exception was thrown.
   /// @return Name of function.
   const char* function() const noexcept;
   /// @brief Get line number.
   ///
   /// Get line number where exception was thrown.
   /// @return Line number.
   int line() const noexcept;
   /// @brief Get custom exception name.
   ///
   /// Get name of the specific exception type that was thrown.
   /// @return Type of exception thrown.
   const char* what() const noexcept;
   /// @brief Get details.
   ///
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
