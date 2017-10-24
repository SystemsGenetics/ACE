#include "testopenclevent.h"
#include "opencldevice.h"
#include "openclbuffer.h"
#include "openclevent.h"






void TestOpenCLEvent::testIssers()
{
   EOpenCLDevice& device {EOpenCLDevice::getInstance()};
   auto buffer {device.makeBuffer<cl_float>(1000000)};
   EOpenCLEvent event;
   Q_ASSERT( event.isNull() );
   Q_ASSERT( event.isDone() );
   event = buffer->write();
   Q_ASSERT( !event.isDone() );
   event.wait();
   Q_ASSERT( event.isDone() );
   Q_ASSERT( !event.isNull() );
}
