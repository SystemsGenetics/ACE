#include "opencl_common.h"
#include <QStringList>
#include "eexception.h"
//






/*!
 *
 * @param exception  
 *
 * @param code  
 */
void OpenCL::fillException(EException* exception, cl_int code)
{
   static const QStringList codeStrings
   {//
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
      ,"COMPILE_PROGRAM_FAILURE"
      ,"LINKER_NOT_AVAILABLE"
      ,"LINK_PROGRAM_FAILURE"
      ,"DEVICE_PARTITION_FAILED"
      ,"KERNEL_ARG_INFO_NOT_AVAILABLE"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"UNKNOWN"
      ,"NVIDIA_ILLEGAL_READWRITE_BUFFER"
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
      ,"INVALID_IMAGE_DESCRIPTOR"
      ,"INVALID_COMPILER_OPTIONS"
      ,"INVALID_LINKER_OPTIONS"
      ,"INVALID_DEVICE_PARTITION_COUNT"
      ,"INVALID_PIPE_SIZE"
      ,"INVALID_DEVICE_QUEUE"
   };
   cl_int index = -1*code;
   if ( index == 9999 )
   {
      index = 29;
   }
   if ( index < 0 || index >= codeStrings.size() )
   {
      index = 28;
   }
   exception->setTitle(QObject::tr("OpenCL Error"));
   exception->setTitle(QObject::tr("OpenCL system failed with the following error code: %1(%2)")
                       .arg(codeStrings.at(index))
                       .arg(code));
}
