#include "ace_settingsrun.h"
#include <QTextStream>
#include "../core/ace_settings.h"
#include "../core/opencl_platform.h"
#include "../core/opencl_device.h"
#include "../core/eexception.h"
#include "../core/edebug.h"



using namespace Ace;
//






/*!
 * Constructs a new settings run object with the given command arguments. 
 *
 * @param command  
 */
SettingsRun::SettingsRun(const Command& command):
   _command(command)
{
   EDEBUG_FUNC(this,&command)
}






/*!
 * Executes this object, processing its command arguments to determine and execute 
 * the specific settings command that has been given by the user. 
 */
void SettingsRun::execute()
{
   EDEBUG_FUNC(this)

   // If the command argument size is empty then simply print all ACE settings and 
   // exit, else call the setting parser method to determine which command was given. 
   if ( _command.size() == 0 )
   {
      QTextStream stream(stdout);
      Ace::Settings& settings {Ace::Settings::instance()};
      stream << "SETTINGS\n\n";
      stream << "          OpenCL Device: " << openCLDeviceString() << "\n";
      stream << "        ACU Thread Size: " << QString::number(settings.threadSize()) << "\n";
      stream << "        MPI Buffer Size: " << QString::number(settings.bufferSize()) << "\n";
      stream << "Chunk Working Directory: " << settings.chunkDir() << "\n";
      stream << "           Chunk Prefix: " << settings.chunkPrefix() << "\n";
      stream << "        Chunk Extension: " << settings.chunkExtension() << "\n";
      stream << "                Logging: " << ( settings.loggingEnabled() ? QStringLiteral("on") : QStringLiteral("off") ) << "\n";
   }
   else
   {
      settings();
   }
}






/*!
 * Parses the first command argument to determine if it is set or list. If it is 
 * neither then an exception is thrown. 
 */
void SettingsRun::settings()
{
   EDEBUG_FUNC(this)

   // Call set or list methods based off the first command argument being set or list 
   // respectively. If the first command argument is neither then throw an exception. 
   enum {Unknown=-1,Set,List};
   QStringList commandList {"set","list"};
   QString command {_command.first()};
   switch (_command.pop(commandList))
   {
   case Set:
      set();
      break;
   case List:
      list();
      break;
   case Unknown:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings requires sub argument, exiting...").arg(command));
         throw e;
      }
   }
}






/*!
 * Returns the OpenCL device setting as a string. 
 *
 * @return OpenCL device setting. 
 */
QString SettingsRun::openCLDeviceString()
{
   EDEBUG_FUNC(this)

   // Return the OpenCL device setting as a string formatted as the platform and 
   // device index separated by a colon. If no device is set then return "none". 
   QString ret {"none"};
   Ace::Settings& settings {Ace::Settings::instance()};
   int platform {settings.openCLPlatform()};
   int device {settings.openCLDevice()};
   if ( platform >= 0 && device >= 0 )
   {
      ret = QString::number(platform).append(":").append(QString::number(device));
   }
   return ret;
}






/*!
 * Executes the settings set command. This simply parses for which setting is to be 
 * set and calls the appropriate method. If the setting is unknown then an 
 * exception is thrown. 
 */
void SettingsRun::set()
{
   EDEBUG_FUNC(this)

   // Determine which setting is to be set, calling the appropriate method and 
   // popping this object's first command argument. If the setting is unknown then 
   // throw an exception. 
   enum {Unknown=-1,OpenCLCom,Threads,Buffer,ChunkDir,ChunkPre,ChunkExt,Logging};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set requires sub argument, exiting..."));
      throw e;
   }
   QStringList list {"opencl","threads","buffer","chunkdir","chunkpre","chunkext","logging"};
   switch (_command.peek(list))
   {
   case OpenCLCom:
      _command.pop();
      setOpenCL();
      break;
   case Threads:
      _command.pop();
      setThreads();
      break;
   case Buffer:
      _command.pop();
      setBuffer();
      break;
   case ChunkDir:
      _command.pop();
      setChunkDir();
      break;
   case ChunkPre:
      _command.pop();
      setChunkPre();
      break;
   case ChunkExt:
      _command.pop();
      setChunkExt();
      break;
   case Logging:
      _command.pop();
      setLogging();
      break;
   case Unknown:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings set sub argument '%1' unrecognized, exiting...")
                      .arg(_command.first()));
         throw e;
      }
   }
}






/*!
 * Executes the settings set opencl command. This simply takes the device argument, 
 * parsing it into its platform and device index, and then setting the OpenCL 
 * device with those indexes. The special "none" string sets the OpenCL device to 
 * none. If parsing of the indexes fails then an exception is thrown. 
 */
void SettingsRun::setOpenCL()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // go the next step. 
   auto invalid = [this]()
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given ACU device '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   };
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set ACU requires sub argument, exiting..."));
      throw e;
   }

   // Attempt to parse the platform and device indexes from this object's first 
   // command argument. If the first argument is the special string "none" then set 
   // the platform index to -1 and device index to 0 denoting no device. If parsing 
   // the first command argument fails then throw an exception. 
   int platform {-1};
   int device {0};
   if ( _command.first() != QString("none") )
   {
      QStringList list {_command.first().split(':')};
      if ( list.size() != 2 )
      {
         invalid();
      }
      bool ok;
      platform = list.at(0).toInt(&ok);
      if ( !ok )
      {
         invalid();
      }
      device = list.at(1).toInt(&ok);
      if ( !ok )
      {
         invalid();
      }
      if ( platform < 0 || platform >= OpenCL::Platform::size() )
      {
         invalid();
      }
      if ( device < 0 || device >= OpenCL::Platform::get(platform)->deviceSize() )
      {
         invalid();
      }
   }

   // Set the new platform and device index to ACE global settings. 
   Ace::Settings& settings {Ace::Settings::instance()};
   settings.setOpenCLPlatform(platform);
   settings.setOpenCLDevice(device);
}






/*!
 * Executes the settings set threads command, setting the global thread size 
 * setting for ACE. If the new thread size given by the first command argument is 
 * invalid or less than one then an exception is thrown. 
 */
void SettingsRun::setThreads()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // go to the next step. 
   auto invalid = [this]()
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given thread size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   };
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set threads requires sub argument, exiting..."));
      throw e;
   }

   // Read in the new thread size as an integer and update the ACE global setting. If 
   // parsing the new thread size fails or it is less than one then throw an 
   // exception. 
   bool ok;
   int size {_command.first().toInt(&ok)};
   if ( !ok )
   {
      invalid();
   }
   if ( size < 1 )
   {
      invalid();
   }
   Ace::Settings::instance().setThreadSize(size);
}






/*!
 * Executes the settings set buffer command, setting the global buffer size setting 
 * for ACE. If the new buffer size given by the first command argument is invalid 
 * or less than one then an exception is thrown. 
 */
void SettingsRun::setBuffer()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // go to the next step. 
   auto invalid = [this]()
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given buffer size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   };
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set buffer requires sub argument, exiting..."));
      throw e;
   }

   // Read in the new buffer size as an integer and update the ACE global setting. If 
   // parsing the new buffer size fails or it is less than one then throw an 
   // exception. 
   bool ok;
   int size {_command.first().toInt(&ok)};
   if ( !ok )
   {
      invalid();
   }
   if ( size < 1 )
   {
      invalid();
   }
   Ace::Settings::instance().setBufferSize(size);
}






/*!
 * Executes the setting set chunkdir command, setting the global chunk working 
 * directory setting for ACE. 
 */
void SettingsRun::setChunkDir()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // set the global chunk working directory for ACE to the first command argument. 
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkdir requires sub argument, exiting..."));
      throw e;
   }
   Ace::Settings::instance().setChunkDir(_command.first());
}






/*!
 * Executes the setting set chunkpre command, setting the global chunk prefix 
 * setting for ACE. 
 */
void SettingsRun::setChunkPre()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // set the global chunk prefix for ACE to the first command argument. 
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkpre requires sub argument, exiting..."));
      throw e;
   }
   Ace::Settings::instance().setChunkPrefix(_command.first());
}






/*!
 * Executes the setting set chunkdir command, setting the global chunk extension 
 * setting for ACE. 
 */
void SettingsRun::setChunkExt()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // set the global chunk extension for ACE to the first command argument. 
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkext requires sub argument, exiting..."));
      throw e;
   }
   Ace::Settings::instance().setChunkExtension(_command.first());
}






/*!
 * Executes the setting set logging command, setting the global logging enabled 
 * state for ACE. 
 */
void SettingsRun::setLogging()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // set the global logging state for ACE to the first command argument. 
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set logging requires sub argument, exiting..."));
      throw e;
   }
   Ace::Settings::instance().setLoggingEnabled(_command.first() == QStringLiteral("on"));
}






/*!
 * Executes the settings list command, parsing the first argument to determine 
 * which specific list command is to be executed and calling the appropriate 
 * method. 
 */
void SettingsRun::list()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is empty then throw an exception, else 
   // go to the next step. 
   enum {Unknown=-1,OpenCLCom};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings list requires sub argument, exiting..."));
      throw e;
   }

   // Parse the first argument to determine which list command is requested, calling 
   // the appropriate method for the command given. If the command is not recognized 
   // then throw an exception. 
   QStringList list {"opencl"};
   switch (_command.peek(list))
   {
   case OpenCLCom:
      listOpenCL();
      break;
   case Unknown:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings list sub argument '%1' unrecognized, exiting...")
                      .arg(_command.first()));
         throw e;
      }
   }
}






/*!
 * Executes the settings list opencl command, listing all available OpenCL devices 
 * by their platform and device indexes and name to standard output. The indexes 
 * are formatted the same way an OpenCL device is specified in the set command. 
 */
void SettingsRun::listOpenCL()
{
   EDEBUG_FUNC(this)

   // List all available OpenCL devices to standard output by their indexes and name. 
   QTextStream stream (stdout);
   for (int p = 0; p < OpenCL::Platform::size() ;++p)
   {
      for(int d = 0; d < OpenCL::Platform::get(p)->deviceSize() ;++d)
      {
         stream << QString::number(p)
                << ":"
                << QString::number(d)
                << " "
                << OpenCL::Platform::get(p)->device(d)->name()
                << "\n";
      }
   }
}
