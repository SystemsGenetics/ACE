#include "application.h"
#include "exception.h"
#include "opencldevice.h"
#include "mainwindow.h"






EApplication::EApplication(int& argc, char** argv, const QString& title):
   QApplication(argc,argv)
{
   try
   {
      Ace::MainWindow& window {Ace::MainWindow::getInstance()};
      window.setWindowTitle(title);
      window.show();
   }
   catch (EException e)
   {
      e.display();
      ::exit(1);
   }
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
   catch (std::exception e)
   {
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
   }
   catch (...)
   {
      qDebug() << tr("Unknown exception caught!\n");
   }
   return false;
}
