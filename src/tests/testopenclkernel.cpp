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
   event = buffer->read();
   event.wait();
   Q_ASSERT( (*buffer)[0] == 'H' );
   Q_ASSERT( (*buffer)[1] == 'e' );
   Q_ASSERT( (*buffer)[2] == 'l' );
   Q_ASSERT( (*buffer)[3] == 'l' );
   Q_ASSERT( (*buffer)[4] == 'o' );
   Q_ASSERT( (*buffer)[5] == ' ' );
   Q_ASSERT( (*buffer)[6] == 'K' );
   Q_ASSERT( (*buffer)[7] == 'e' );
   Q_ASSERT( (*buffer)[8] == 'r' );
   Q_ASSERT( (*buffer)[9] == 'n' );
   Q_ASSERT( (*buffer)[10] == 'e' );
   Q_ASSERT( (*buffer)[11] == 'l' );
   Q_ASSERT( (*buffer)[12] == '!' );
   Q_ASSERT( (*buffer)[13] == '\0' );
}
