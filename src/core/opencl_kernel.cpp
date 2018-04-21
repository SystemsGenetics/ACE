#include "opencl_kernel.h"
#include "opencl_device.h"
#include "opencl_program.h"
#include "opencl_commandqueue.h"
#include "opencl_event.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 */
Kernel::~Kernel()
{
   clReleaseKernel(_id);
   clear();
}






/*!
 *
 * @param queue  
 */
Event Kernel::execute(CommandQueue* queue)
{
   cl_event id;
   cl_int code
   {//
      clEnqueueNDRangeKernel(queue->id(),_id,_size,_offsets,_globalSizes,_localSizes,0,nullptr,&id)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return Event(id);
}






/*!
 *
 * @param program  
 *
 * @param name  
 *
 * @param parent  
 */
Kernel::Kernel(Program* program, const QString& name, QObject* parent):
   QObject(parent)
{
   cl_int code;
   _id = clCreateKernel(program->id(),name.toLocal8Bit().data(),&code);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   resize();
}






/*!
 *
 * @param device  
 */
int Kernel::maxWorkGroupSize(Device* device) const
{
   size_t size;
   cl_int code
   {//
      clGetKernelWorkGroupInfo(_id
                               ,device->id()
                               ,CL_KERNEL_WORK_GROUP_SIZE
                               ,sizeof(size_t)
                               ,&size
                               ,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return size;
}






/*!
 *
 * @param device  
 */
int Kernel::workGroupMultiple(Device* device) const
{
   size_t size;
   cl_int code
   {//
      clGetKernelWorkGroupInfo(_id
                               ,device->id()
                               ,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
                               ,sizeof(size_t)
                               ,&size
                               ,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return size;
}






/*!
 *
 * @param size  
 */
void Kernel::setDimensions(cl_uint size)
{
   if ( size != _size )
   {
      _size = size;
      clear();
      resize();
   }
}






/*!
 *
 * @param dimension  
 *
 * @param globalSize  
 *
 * @param localSize  
 */
void Kernel::setSizes(cl_uint dimension, qint64 globalSize, qint64 localSize)
{
   if ( dimension >= _size )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Out Of Range"));
      e.setDetails(tr("Cannot set dimension %1 sizes of OpenCL kernel (%2 total).")
                   .arg(dimension)
                   .arg(_size));
      throw e;
   }
   if ( globalSize < 1 || localSize < 1 || globalSize&localSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("%1 and %2 are not valid global and local OpenCL kernel sizes, respectively."));
      throw e;
   }
   _globalSizes[dimension] = globalSize;
   _localSizes[dimension] = localSize;
}






/*!
 */
void Kernel::resize()
{
   _offsets = new size_t[_size];
   _globalSizes = new size_t[_size];
   _localSizes = new size_t[_size];
   for (cl_uint i = 0; i < _size ;++i)
   {
      _offsets[i] = 0;
      _globalSizes[i] = 1;
      _localSizes[i] = 1;
   }
}






/*!
 */
void Kernel::clear()
{
   delete[] _offsets;
   delete[] _globalSizes;
   delete[] _localSizes;
}
