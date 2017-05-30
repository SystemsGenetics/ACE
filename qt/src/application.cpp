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
   QString message;
   QTextStream stream(&message);
   QString function = e.getFunction();
   function.replace(" ","&nbsp;");
   stream << "<h4>" << QObject::tr("An unexpected error has occured.") << "</h4>";
   stream << "<p><b>" << QObject::tr("Title:") << "</b> " << e.getTitle() << "<br/>";
   stream << "<b>" << QObject::tr("File:") << "</b> " << e.getFile() << "<br/>";
   stream << "<b>" << QObject::tr("Function:") << "</b>&nbsp;" << function << "<br/>";
   stream << "<b>" << QObject::tr("Line:") << "</b> " << e.getLine() << "</p>";

   // create the message box, set all exception information for the user, and modally display the
   // dialog
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);
   critical.setDetailedText(e.getDetails());
   critical.exec();
}
