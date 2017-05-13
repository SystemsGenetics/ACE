#ifndef EOPENCLDEVICE_H
#define EOPENCLDEVICE_H
#include <QtCore>
#include <memory>

#include "openclevent.h"
#include "openclbuffer.h"
#include "opencl.h"



class EOpenCLProgram;
class EOpenCLKernel;



class EOpenCLDevice : public EOpenCL
{
public:
   ~EOpenCLDevice();
   ACE_DISBALE_COPY_AND_MOVE(EOpenCLDevice)
   static EOpenCLDevice& getInstance();
   bool setDevice(cl_platform_id platformID, cl_device_id deviceID);
   std::unique_ptr<EOpenCLProgram> makeProgram() const;
   template<class T> std::unique_ptr<EOpenCLBuffer<T>> makeBuffer(quint64 size);
   quint64 getGlobalMemorySize() const;
   quint64 getLocalMemorySize() const;
private:
   void reportNoOpenCL();
   EOpenCLDevice();
   static std::unique_ptr<EOpenCLDevice> _instance;
   cl_device_id _deviceID {0};
   std::unique_ptr<cl_context> _contextID {nullptr};
   std::unique_ptr<cl_command_queue> _commandQueueID {nullptr};
};






template<class T>
std::unique_ptr<EOpenCLBuffer<T>> EOpenCLDevice::makeBuffer(quint64 size)
{
   if ( getStatus() != Ok )
   {
      return nullptr;
   }
   return std::unique_ptr<EOpenCLBuffer<T>>(new EOpenCLBuffer<T>(*_contextID,*_commandQueueID
                                                                 ,size));
}



#endif
