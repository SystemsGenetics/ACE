#include "eapplication.h"
#include <QMessageBox>
#include <core/ace_settings.h>
#include <core/eabstractdatafactory.h>
#include <core/eabstractanalyticfactory.h>
#include <core/eexception.h>
#include "ace_mainwindow.h"
//






/*!
 *
 * @param organization  
 *
 * @param application  
 *
 * @param majorVersion  
 *
 * @param minorVersion  
 *
 * @param revision  
 *
 * @param data  
 *
 * @param analytic  
 *
 * @param argc  
 *
 * @param argv  
 */
EApplication::EApplication(const QString& organization, const QString& application, int majorVersion, int minorVersion, int revision, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv):
   QApplication(argc,argv)
{
   try
   {
      Ace::Settings::initialize(organization,application,majorVersion,minorVersion,revision);
      EAbstractDataFactory::setInstance(std::move(data));
      EAbstractAnalyticFactory::setInstance(std::move(analytic));
      Ace::MainWindow& window {Ace::MainWindow::getInstance()};
      window.setWindowTitle(application);
      window.show();
   }
   catch (EException e)
   {
      showException(e);
      ::exit(-1);
   }
   catch (std::exception e)
   {
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
      ::exit(-1);
   }
   catch (...)
   {
      qDebug() << tr("Unknown exception caught!\n");
      ::exit(-1);
   }
}






/*!
 *
 * @param receiver  
 *
 * @param event  
 */
bool EApplication::notify(QObject* receiver, QEvent* event)
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






/*!
 *
 * @param exception  
 */
void EApplication::showException(const EException& exception)
{
   QString message = QString("<h3>%1</h3><p>%2</p>").arg(exception.title())
                                                    .arg(exception.details());
   QString function = exception.functionName().replace(" ","&nbsp;");
   message += QString("<ol><li><b>File:</b>&nbsp;%1</li>").arg(exception.fileName());
   message += QString("<li><b>Function:</b>&nbsp;%1</li>").arg(function);
   message += QString("<li><b>Line:</b>&nbsp;%1</li></ol>").arg(exception.line());
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);
   critical.exec();
}
