#ifndef EOPENCLDEVICE_H
#define EOPENCLDEVICE_H
#include <QtCore>
#include <memory>

#include "openclevent.h"
#include "openclbuffer.h"
#include "utilities.h"



class EOpenCLProgram;
class EOpenCLKernel;



class EOpenCLDevice
{
public:
   ~EOpenCLDevice();
   ACE_MAKE_NO_COPY_OR_MOVE(EOpenCLDevice);
   static EOpenCLDevice& getInstance();
   void initialize();
   void setDevice(cl_platform_id platformID, cl_device_id deviceID);
   void clear();
   std::unique_ptr<EOpenCLProgram> makeProgram() const;
   template<class T> std::unique_ptr<EOpenCLBuffer<T>> makeBuffer(quint64 size);
   quint64 getGlobalMemorySize() const;
   quint64 getLocalMemorySize() const;
private:
   EOpenCLDevice() = default;
   void throwInitializeError();
   static EOpenCLDevice* _instance;
   cl_device_id* _deviceID {nullptr};
   cl_context* _contextID {nullptr};
   cl_command_queue* _commandQueueID {nullptr};
};






template<class T>
std::unique_ptr<EOpenCLBuffer<T>> EOpenCLDevice::makeBuffer(quint64 size)
{
   if ( !_contextID || !_commandQueueID )
   {
      return nullptr;
   }
   return std::unique_ptr<EOpenCLBuffer<T>>(new EOpenCLBuffer<T>(*_contextID,*_commandQueueID
                                                                 ,size));
}



#endif
