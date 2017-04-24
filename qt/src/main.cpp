#include <QApplication>

#include "opencldevicedialog.h"
#include "opencldevice.h"
#include "openclprogram.h"
#include "openclkernel.h"
#include "exception.h"






int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   try
   {
      OpenCLDevice::getInstance().initialize();
      unique_ptr<OpenCLProgram> program = OpenCLDevice::getInstance().makeProgram();
      program->addSource("__kernel void lala(__global int* ok) {}");
      program->compile(QString());
      unique_ptr<OpenCLKernel> kernel = program->makeKernel("lala");
      unique_ptr<OpenCLBuffer<int>> buffer = OpenCLDevice::getInstance().makeBuffer<int>(1024);
      OpenCLEvent event = buffer->write();
      event.wait();
      kernel->setBuffer(0,buffer.get());
      event = (*kernel)();
      event.isDone();
      event = buffer->read();
      event.wait();
      buffer.reset();
      kernel.reset();
      program.reset();
      OpenCLDeviceDialog::getInstance().show();
      OpenCLDeviceDialog::getInstance().exec();
   }
   catch (Exception e)
   {
      e.display();
      exit(1);
   }
   return a.exec();
}
