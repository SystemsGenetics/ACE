#ifndef OPENCLDEVICE_H
#define OPENCLDEVICE_H
#include <QtCore>
#include <memory>

#include "openclevent.h"
#include "openclbuffer.h"
#include "utilities.h"



class OpenCLProgram;
class OpenCLKernel;



class OpenCLDevice
{
public:
   ~OpenCLDevice();
   MAKE_NO_COPY_OR_MOVE(OpenCLDevice);
   static OpenCLDevice& getInstance();
   void initialize();
   void setDevice(cl_platform_id platformID, cl_device_id deviceID);
   void clear();
   std::unique_ptr<OpenCLProgram> makeProgram() const;
   template<class T> std::unique_ptr<OpenCLBuffer<T>> makeBuffer(quint64 size);
   quint64 getGlobalMemorySize() const;
   quint64 getLocalMemorySize() const;
private:
   OpenCLDevice() = default;
   void throwInitializeError();
   static OpenCLDevice* _instance;
   cl_device_id* _deviceID {nullptr};
   cl_context* _contextID {nullptr};
   cl_command_queue* _commandQueueID {nullptr};
};






template<class T>
std::unique_ptr<OpenCLBuffer<T>> OpenCLDevice::makeBuffer(quint64 size)
{
   if ( !_contextID || !_commandQueueID )
   {
      return nullptr;
   }
   return std::unique_ptr<OpenCLBuffer<T>>(new OpenCLBuffer<T>(*_contextID,*_commandQueueID,size));
}



#endif
