#ifndef EOPENCLDEVICE_H
#define EOPENCLDEVICE_H
#include <QtCore>
#include <memory>

#include "openclevent.h"
#include "openclbuffer.h"
#include "opencl.h"



class EOpenCLProgram;
class EOpenCLKernel;



/// Singleton class that represents ACE's currently selected OpenCL device.
class EOpenCLDevice : public EOpenCL
{
public:
   ~EOpenCLDevice();
   EOpenCLDevice(const EOpenCLDevice&) = delete;
   static EOpenCLDevice& getInstance();
   /// Internal command DO NOT USE.
   bool setDevice(cl_platform_id platformID, cl_device_id deviceID);
   /// Make a new opencl program object.
   ///
   /// @return Pointer to new program object.
   std::unique_ptr<EOpenCLProgram> makeProgram() const;
   /// Make a new opencl buffer object.
   ///
   /// @return Pointer to new buffer object.
   template<class T> std::unique_ptr<EOpenCLBuffer<T>> makeBuffer(quint64 size);
   /// Get global memory size of OpenCL device in bytes.
   ///
   /// @return Memory size in bytes.
   quint64 getGlobalMemorySize() const;
   /// Get local memory of OpenCL device in bytes.
   ///
   /// @return Memory size in bytes.
   quint64 getLocalMemorySize() const;
private:
   EOpenCLDevice();
   static std::unique_ptr<EOpenCLDevice> _instance;
   cl_device_id _deviceID {0};
   std::unique_ptr<cl_context> _contextID {nullptr};
   std::unique_ptr<cl_command_queue> _commandQueueID {nullptr};
};






template<class T>
std::unique_ptr<EOpenCLBuffer<T>> EOpenCLDevice::makeBuffer(quint64 size)
{
   // make sure device is in ok state
   if ( getStatus() != Ok )
   {
      return nullptr;
   }

   // return new OpenCL buffer
   return std::unique_ptr<EOpenCLBuffer<T>>(new EOpenCLBuffer<T>(*_contextID,*_commandQueueID
                                                                 ,size));
}



#endif
