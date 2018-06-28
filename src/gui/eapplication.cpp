#include "eapplication.h"
#include <QMessageBox>
#include "../core/ace_settings.h"
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "ace_mainwindow.h"
#include "ace_analyticthread.h"
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
EApplication::EApplication(const QString& organization, const QString& application, int majorVersion, int minorVersion, int revision, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv):
   QApplication(argc,argv)
{
   try
   {
      // Initialize the ACE core system.
      Ace::Settings::initialize(organization,application,majorVersion,minorVersion,revision);
      EAbstractDataFactory::setInstance(std::move(data));
      EAbstractAnalyticFactory::setInstance(std::move(analytic));

      // Initialize the main window, setting its title and showing it.
      Ace::MainWindow& window {Ace::MainWindow::instance()};
      window.setWindowTitle(application);
      window.show();
   }

   // Catch any exception, reporting it to the user and forcefully closing the program.
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
 * Implements _QCoreApplication::notify_. This is implemented to allow for 
 * exception support within the event loop of Qt. This is also for catching 
 * exceptions within the analytic thread and setting its exception. 
 *
 * @param receiver The qt object that is receiving this event. 
 *
 * @param event The qt event itself. 
 */
bool EApplication::notify(QObject* receiver, QEvent* event)
{
   try
   {
      // Call the parent interface.
      return QApplication::notify(receiver,event);
   }

   // Catch any ACE exception.
   catch (EException e)
   {
      // If this is the main GUI event loop then report the exception to the user and continue
      // execution.
      if ( QThread::currentThread() == instance()->thread() )
      {
         showException(e);
      }

      // Else if this is an analytic thread event loop then set the caught exception to the
      // corresponding analytic thread object.
      else if ( Ace::AnalyticThread* valid = qobject_cast<Ace::AnalyticThread*>(QThread::currentThread()) )
      {
         valid->setException(e);
      }
   }

   // If any other exception is caught then report it and continue execution.
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
 * Opens a message dialog showing information about the given ACE exception that 
 * was caught. 
 *
 * @param exception The ACE exception whose information is presented to the user in 
 *                  a message dialog. 
 */
void EApplication::showException(const EException& exception)
{
   // Create the user message rich text and add the exception title and details.
   QString message = QString("<h3>%1</h3><p>%2</p>").arg(exception.title())
                                                    .arg(exception.details());

   // Get the function from the exception, replacing spaces with no break spaces.
   QString function = exception.functionName().replace(" ","&nbsp;");

   // Add the file name, function, and line number to the user message.
   message += QString("<ol><li><b>File:</b>&nbsp;%1</li>").arg(exception.fileName());
   message += QString("<li><b>Function:</b>&nbsp;%1</li>").arg(function);
   message += QString("<li><b>Line:</b>&nbsp;%1</li></ol>").arg(exception.line());

   // Create the message dialog, setting its title, icon, and message.
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);

   // Begin modal execution of the message dialog.
   critical.exec();
}
