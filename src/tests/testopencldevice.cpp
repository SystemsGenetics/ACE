#include "testopencldevice.h"
#include "opencldevice.h"
#include "openclprogram.h"






void TestOpenCLDevice::testGetters()
{
   EOpenCLDevice& device {EOpenCLDevice::getInstance()};
   device.getGlobalMemorySize();
   device.getLocalMemorySize();
}






void TestOpenCLDevice::testMakers()
{
   EOpenCLDevice& device {EOpenCLDevice::getInstance()};
   auto program = device.makeProgram();
   auto buffer = device.makeBuffer<cl_float>(16);
   Q_UNUSED(program);
   Q_UNUSED(buffer);
}
