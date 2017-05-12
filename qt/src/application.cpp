#include "application.h"
#include "exception.h"
#include "opencldevice.h"
#include "mainwindow.h"






EApplication::EApplication(int &argc, char *argv[]):
   QApplication(argc,argv)
{
   EOpenCLDevice::getInstance().initialize();
   Ace::MainWindow::getInstance().show();
}






bool EApplication::notify(QObject *receiver, QEvent *event)
{
   try
   {
      return QApplication::notify(receiver,event);
   }
   catch (EException e)
   {
      e.display();
   }
   catch (...)
   {
      qDebug() << tr("Unknown exception caught!\n");
   }
   return false;
}
