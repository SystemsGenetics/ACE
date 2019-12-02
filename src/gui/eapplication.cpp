#include "eapplication.h"
#include <QMessageBox>
#include <QSplashScreen>
#include <QWindow>
#include "../core/ace_settings.h"
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "ace_mainwindow.h"
#include "ace_analyticthread.h"
#include "eabstractcustomizer.h"



//






/*!
 * Constructs a new application object, initializing the ACE system with the given 
 * organization, application, version numbers, data factory, and analytic factory. 
 * This then starts the GUI program by creating the main window and starting the 
 * main event loop. 
 *
 * @param organization The organization name of the application using ACE. 
 *
 * @param application The name of the application using ACE. 
 *
 * @param majorVersion The major version of the application using ACE. 
 *
 * @param minorVersion The minor version of the application using ACE. 
 *
 * @param revision The revision of the application using ACE. 
 *
 * @param data The data factory of the application using ACE. 
 *
 * @param analytic The analytic factory of the application using ACE. 
 *
 * @param argc The command line argument size passed from the main function. 
 *
 * @param argv The command line arguments passed from the main function. 
 */
EApplication::EApplication(const QString& organization
                           ,const QString& application
                           ,int majorVersion
                           ,int minorVersion
                           ,int revision
                           ,std::unique_ptr<EAbstractDataFactory>&& data
                           ,std::unique_ptr<EAbstractAnalyticFactory>&& analytic
                           ,int& argc
                           ,char** argv):
   QApplication(argc,argv)
{
   try
   {
      // .
      Ace::Settings::initialize(organization,application,majorVersion,minorVersion,revision);
      EAbstractDataFactory::setInstance(std::move(data));
      EAbstractAnalyticFactory::setInstance(std::move(analytic));

      // .
      auto splashImage {EAbstractCustomizer::instance().splashImage()};
      auto icon {EAbstractCustomizer::instance().icon()};
      QSplashScreen splash(splashImage);
      if ( !splashImage.isNull() )
      {
          splash.show();
          for (int i = 0; i < 1000 ;++i)
          {
              QThread::msleep(1);
              processEvents();
          }
      }
      Ace::MainWindow& window {Ace::MainWindow::instance()};
      if ( !icon.isNull() )
      {
          window.setWindowIcon(icon);
      }
      window.setWindowTitle(application);
      window.show();
      splash.finish(&window);
   }

   // .
   catch (EException e)
   {
      showException(e);
      ::exit(-1);
   }
   catch (std::exception& e)
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
 * Implements _QCoreApplication::notify_. This is implemented to allow for 
 * exception support within the event loop of Qt. This is also for catching 
 * exceptions within the analytic thread and setting its exception. 
 *
 * @param receiver The qt object that is receiving this event. 
 *
 * @param event The qt event itself. 
 */
bool EApplication::notify(QObject* receiver
                          ,QEvent* event)
{
   try
   {
      // .
      return QApplication::notify(receiver,event);
   }

   // .
   catch (EException e)
   {
      // .
      if ( QThread::currentThread() == instance()->thread() )
      {
         showException(e);
      }

      // .
      else if ( Ace::AnalyticThread* valid = qobject_cast<Ace::AnalyticThread*>(QThread::currentThread()) )
      {
         valid->setException(e);
      }
   }

   // .
   catch (std::exception& e)
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
 * Opens a message dialog showing information about the given ACE exception that 
 * was caught. 
 *
 * @param exception The ACE exception whose information is presented to the user in 
 *                  a message dialog. 
 */
void EApplication::showException(const EException& exception)
{
   // .
   QString message = QString("<h3>%1</h3><p>%2</p>").arg(exception.title())
                                                    .arg(exception.details());

   // .
   QString function = exception.functionName().replace(" ","&nbsp;");

   // .
   message += QString("<ol><li><b>File:</b>&nbsp;%1</li>").arg(exception.fileName());
   message += QString("<li><b>Function:</b>&nbsp;%1</li>").arg(function);
   message += QString("<li><b>Line:</b>&nbsp;%1</li></ol>").arg(exception.line());

   // .
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);

   // .
   critical.exec();
}
