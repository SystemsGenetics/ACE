#include "testopenclbuffer.h"
#include "opencldevice.h"
#include "openclbuffer.h"
#include "openclevent.h"






void TestOpenCLBuffer::testReadWrite()
{
   EOpenCLDevice& device {EOpenCLDevice::getInstance()};
   auto buffer {device.makeBuffer<cl_float>(2)};
   (*buffer)[0] = 3.75;
   (*buffer)[1] = 6.875;
   EOpenCLEvent event = buffer->write();
   event.wait();
   (*buffer)[0] = (*buffer)[1] = 0;
   event = buffer->read();
   event.wait();
   Q_ASSERT( (*buffer)[0] == 3.75 );
   Q_ASSERT( (*buffer)[1] == 6.875 );
}
