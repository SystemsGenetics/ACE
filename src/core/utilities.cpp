#include "utilities.h"
#include "eexception.h"






QString Ace::OpenCL::generateErrorString(cl_int code)
{
   // static list of possible OpenCL error types
   static const char* errors[] =
   {
      "SUCCESS"
      ,"DEVICE_NOT_FOUND"
      ,"DEVICE_NOT_AVAILABLE"
      ,"COMPILER_NOT_AVAILABLE"
      ,"MEM_OBJECT_ALLOCATION_FAILURE"
      ,"OUT_OF_RESOURCES"
      ,"OUT_OF_HOST_MEMORY"
      ,"PROFILING_INFO_NOT_AVAILABLE"
      ,"MEM_COPY_OVERLAP"
      ,"IMAGE_FORMAT_MISMATCH"
      ,"IMAGE_FORMAT_NOT_SUPPORTED"
      ,"BUILD_PROGRAM_FAILURE"
      ,"MAP_FAILURE"
      ,"MISALIGNED_SUB_BUFFER_OFFSET"
      ,"EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"INVALID_VALUE"
      ,"INVALID_DEVICE_TYPE"
      ,"INVALID_PLATFORM"
      ,"INVALID_DEVICE"
      ,"INVALID_CONTEXT"
      ,"INVALID_QUEUE_PROPERTIES"
      ,"INVALID_COMMAND_QUEUE"
      ,"INVALID_HOST_PTR"
      ,"INVALID_MEM_OBJECT"
      ,"INVALID_IMAGE_FORMAT_DESCRIPTOR"
      ,"INVALID_IMAGE_SIZE"
      ,"INVALID_SAMPLER"
      ,"INVALID_BINARY"
      ,"INVALID_BUILD_OPTIONS"
      ,"INVALID_PROGRAM"
      ,"INVALID_PROGRAM_EXECUTABLE"
      ,"INVALID_KERNEL_NAME"
      ,"INVALID_KERNEL_DEFINITION"
      ,"INVALID_KERNEL"
      ,"INVALID_ARG_INDEX"
      ,"INVALID_ARG_VALUE"
      ,"INVALID_ARG_SIZE"
      ,"INVALID_KERNEL_ARGS"
      ,"INVALID_WORK_DIMENSION"
      ,"INVALID_WORK_GROUP_SIZE"
      ,"INVALID_WORK_ITEM_SIZE"
      ,"INVALID_GLOBAL_OFFSET"
      ,"INVALID_EVENT_WAIT_LIST"
      ,"INVALID_EVENT"
      ,"INVALID_OPERATION"
      ,"INVALID_GL_OBJECT"
      ,"INVALID_BUFFER_SIZE"
      ,"INVALID_MIP_LEVEL"
      ,"INVALID_GLOBAL_WORK_SIZE"
      ,"INVALID_PROPERTY"
   };

   if ( code <= 0 && code >= -64 )
   {
      // if error code is within range look it up and return
      return QString(errors[-code]);
   }
   else
   {
      // else error is not in range, return unkonwn
      return QString(errors[15]);
   }
}






void Ace::OpenCL::throwError(const QString& function, cl_int code)
{
   E_MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("OpenCL Error"));
   e.setDetails(QObject::tr("The function %1 returned the error code %2.")
                .arg(function).arg(generateErrorString(code)));
   throw e;
}






QString Ace::formatMemorySize(quint64 size)
{
   // initialize temporary variables
   enum { kb=1,mb,gb,tb };
   float a = size;
   int multi = 0;

   // divide a by 1024 until it is less than 1024 or reaches TB
   while ( a > 1024.0 && multi <= tb )
   {
      a /= 1024;
      ++multi;
   }

   // make formatted memory size output
   QString formatted;
   QTextStream stream(&formatted);
   stream.setRealNumberPrecision(4);
   stream << a << " ";
   switch (multi)
   {
   case kb:
      stream << "K";
      break;
   case mb:
      stream << "M";
      break;
   case gb:
      stream << "G";
      break;
   case tb:
      stream << "T";
      break;
   }
   stream << "B";
   return formatted;
}
