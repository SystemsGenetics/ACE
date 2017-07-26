#include <QMessageBox>

#include "Application.h"
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
      showException(e);
      ::exit(-1);
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
      showException(e);
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






void EApplication::showException(const EException &e)
{
   // generate the message box's textual information
   QString message = QString("<h3>%1</h3><p>%2</p>").arg(e.getTitle()).arg(e.getDetails());
   QString function = e.getFunction().replace(" ","&nbsp;");
   message += QString("<ol><li><b>File:</b>&nbsp;%1</li>").arg(e.getFile());
   message += QString("<li><b>Function:</b>&nbsp;%1</li>").arg(function);
   message += QString("<li><b>Line:</b>&nbsp;%1</li></ol>").arg(e.getLine());

   // create the message box, set all exception information for the user, and modally display the
   // dialog
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);
   critical.exec();
}
