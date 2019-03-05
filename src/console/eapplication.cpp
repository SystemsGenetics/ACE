#include "eapplication.h"
#include <QTextStream>
#include "../core/ace_settings.h"
#include "../core/ace_dataobject.h"
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "../core/edebug.h"
#include "../core/ace_logserver.h"
#include "../core/ace_qmpi.h"
#include "ace_run.h"
#include "ace_settingsrun.h"
#include "ace_helprun.h"






/*!
 * Implements the qt interface that handles all events being passed for
 * handling. This implementation simply added an exception handler to qt so it
 * can support exceptions being thrown.
 *
 * @param receiver The qt object that is receiving this event.
 *
 * @param event The event itself.
 *
 * @return True if the event has been handled or false otherwise.
 */
bool EApplication::notify(QObject* receiver, QEvent* event)
{
   try
   {
      // Call the qt implementation of the notify interface.
      return QCoreApplication::notify(receiver,event);
   }

   // If any exception is thrown then report the exception to the command line and
   // forcefully exit the program.
   catch (EException e)
   {
      showException(e);
   }
   catch (std::exception& e)
   {
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
   }
   catch (...)
   {
      qDebug() << tr("Unknown exception caught!\n");
   }
   std::exit(-1);
}






/*!
 * Constructs a new application object, initializing the ACE system with the
 * given organization, application, version numbers, data factory, and analytic
 * factory. This also parses the command line arguments with the given command
 * line arguments.
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
EApplication::EApplication(const QString& organization, const QString& application, int majorVersion, int minorVersion, int revision, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv)
   :
   QCoreApplication(argc,argv),
   _options(argc,argv),
   _command(argc,argv),
   _runName(argv[0])
{
   try
   {
      // Initialize the ACE global settings instance using the values given.
      Ace::Settings::initialize(organization,application,majorVersion,minorVersion,revision);

      // Set the data and analytic factories to the ones given.
      EAbstractDataFactory::setInstance(std::move(data));
      EAbstractAnalyticFactory::setInstance(std::move(analytic));
   }

   // If any exception is caught then report it to the user and forcefully exit the
   // application.
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
 * Executes the console command based off the command line arguments given to
 * this program, returning when ACE and this program has finished its execution
 * and is ready to quit.
 *
 * @return The code that should be returned in the main function.
 */
int EApplication::exec()
{
   // Add the debug header.
   EDEBUG_FUNC(this);
   try
   {
      // Make sure there is at least one command argument to process, displaying the
      // basic help text if there is none.
      if ( _command.size() < 1 )
      {
         Ace::HelpRun help(_command,_runName);
         help.execute();
         return 0;
      }

      // Create an enumeration and string list used to identify the command argument.
      enum {Unknown = -1,Settings,Run,ChunkRun,Merge,Dump,Inject,Help};
      QStringList commands {"settings","run","chunkrun","merge","dump","inject","help"};

      // Determine and execute which primary command is given.
      switch (_command.peek(commands))
      {
      case Settings:
         {
            // Pop the primary command, create a new settings run object and execute it with
            // the remaining command.
            _command.pop();
            Ace::SettingsRun settings(_command);
            settings.execute();
            break;
         }
      case Run:
      case ChunkRun:
      case Merge:
         {
            // Initialize the run object.
            Ace::Run* run {new Ace::Run(_command,_options)};
            connect(run,&Ace::Run::destroyed,this,&QCoreApplication::quit);

            // Check to see if logging is enabled.
            if ( Ace::Settings::instance().loggingEnabled() )
            {
               // Get the logging port setting and initialize the log server.
               QTextStream out(stdout);
               int port {Ace::Settings::instance().loggingPort() + Ace::QMPI::instance().localRank()};
               Ace::LogServer::initialize(port);

               // Inform the user that the application is waiting for an ACE log client to signal
               // starting.
               out << tr("Log server listening on %1:%2, waiting for connection...\n").arg(Ace::LogServer::host()).arg(port);
               out.flush();
               Ace::LogServer::log()->wait();
            }

            // Start the analytic run.
            return QCoreApplication::exec();
         }
      case Dump:
         _command.pop();
         dump();
         return 0;
      case Inject:
         _command.pop();
         inject();
         return 0;
      case Help:
      case Unknown:
         {
            // If the primary command is not recognized or it is the help command then print
            // the basic help text to the user and exit.
            _command.pop();
            Ace::HelpRun help(_command,_runName);
            help.execute();
            return 0;
         }
      }
   }

   // If an exception is thrown then report it to the user and return failure.
   catch (EException e)
   {
      showException(e);
   }
   catch (std::exception& e)
   {
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
   }
   catch (...)
   {
      qDebug() << tr("Unknown exception caught!\n");
   }
   return -1;
}






/*!
 * Gets JSON formatted text from the file with the given path. If the given file
 * is not valid JSON or opening the file for reading fails then an exception is
 * thrown.
 *
 * @param path The path of the text file which is read in and returned as JSON.
 *
 * @return The JSON of the text file with the given path.
 */
QJsonDocument EApplication::getJson(const QString& path)
{
   // Add the debug header.
   EDEBUG_FUNC(path);

   // Open the text file at the given path for reading, making sure it worked.
   QFile file(path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed opening JSON file %1: %2").arg(path).arg(file.errorString()));
      throw e;
   }

   // Read in the JSON to a document and make sure it worked.
   QJsonDocument ret {QJsonDocument::fromJson(file.readAll())};
   if ( !ret.isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading in JSON file %1: It must be a JSON object.").arg(path));
      throw e;
   }

   // Return the read in JSON document.
   return ret;
}






/*!
 * Injects the given JSON document into the given data object's user metadata,
 * converting the JSON to metadata. It is assumed the given JSON is an object
 * else the behavior of this method is undefined.
 *
 * @param path The path to the data object file that has metadata injected into
 *             it.
 *
 * @param document The JSON document that is converted to metadata and injected
 *                 into the given data object.
 */
void EApplication::inject(const QString& path, const QJsonDocument& document)
{
   // Add the debug header.
   EDEBUG_FUNC(path,&document);

   // Open the data object from the given path and set its user metadata to the given
   // JSON, converting it to ACE metadata.
   Ace::DataObject data(path);
   data.setUserMeta(EMetadata(document.object()));
}






/*!
 * Prints out the title and details of an ACE exception to standard output. If
 * debugging is enabled it also prints out the file, line number, and function
 * where the exception was thrown.
 *
 */
void EApplication::showException(const EException& exception)
{
   // Create a text stream connected to standard output.
   QTextStream stream(stdout);

   // If debugging is enabled print out the file name, line number, and function
   // where the given exception was thrown.
   qDebug().noquote().nospace() << exception.fileName() << ":" << exception.line();
   qDebug().noquote() << exception.functionName();

   // Print out the title, in all caps, and details of the given exception.
   stream << exception.title().toUpper() << "\n" << exception.details() << "\n";
}






/*!
 * Executes the dump command, dumping the system or user metadata, depending on
 * which one is specified to dump, of a given data object to standard output.
 */
void EApplication::dump()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is at least two command arguments to process.
   if ( _command.size() < 2 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Missing required arguments for dump command, exiting..."));
      throw e;
   }

   // Create a text stream connected to standard output and open the data object at
   // the given file path.
   QTextStream stream(stdout);
   Ace::DataObject data(_command.pop());

   // Create a JSON document that is loaded with the data object's system or user
   // metadata and returned.
   QJsonDocument document;

   // Create an enumeration and string list used to determine which metadata is
   // requested.
   enum {Unknown=-1,System,User};
   QStringList commands {"system","user"};

   // Determine which metadata is requested and load the appropriate type.
   switch (_command.peek(commands))
   {
   case System:
      document.setObject(data.systemMeta().toJson().toObject());
      break;
   case User:
      document.setObject(data.userMeta().toJson().toObject());
      break;
   case Unknown:
      {
         // If the command argument is not recognized as system or user then throw an
         // exception informing the user.
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Invalid Argument"));
         e.setDetails(tr("Unknown dump command '%1', exiting...").arg(_command.first()));
         throw e;
      }
   }

   // Output the entire contents of the JSON document containing the user or system
   // metadata as regular text to standard output.
   stream << document.toJson();
}






/*!
 * Executes the inject command, injecting the JSON loaded from a given path and
 * overwriting a given data object's user metadata with it.
 */
void EApplication::inject()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there are at least two command arguments to process.
   if ( _command.size() < 2 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Missing required arguments for inject command, exiting..."));
      throw e;
   }

   // Load the JSON from the path given by the second command argument and inject it
   // into a data object whose path is given by the first command argument.
   inject(_command.at(0),getJson(_command.at(1)));
}

