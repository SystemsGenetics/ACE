#include "testopenclkernel.h"
#include "opencldevice.h"
#include "openclprogram.h"
#include "openclkernel.h"
#include "openclbuffer.h"
#include "openclevent.h"






void TestOpenCLKernel::testRun()
{
   EOpenCLDevice& device {EOpenCLDevice::getInstance()};
   auto program {device.makeProgram()};
   program->addFile(":/opencl/testkernel.cl");
   Q_ASSERT( program->compile() );
   Q_ASSERT( !program->hasBuildError() );
   auto kernel {program->makeKernel("hello")};
   auto buffer {device.makeBuffer<char>(30)};
   kernel->setBuffer(0,buffer.get());
   kernel->addLocalMemory<char>(1,256);
   kernel->setArgument(2,42);
   kernel->setDimensionCount(1);
   Q_ASSERT ( kernel->setGlobalSize(0,1) );
   Q_ASSERT ( kernel->setWorkgroupSize(0,1) );
   kernel->getMaxWorkgroupSize();
   kernel->getWorkgroupMultiple();
   auto event {kernel->execute()};
   event.wait();
}
