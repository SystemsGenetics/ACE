#include "ace_settingsrun.h"
#include <QTextStream>
#include <core/ace_settings.h>
#include <core/opencl_platform.h>
#include <core/opencl_device.h>
#include <core/eexception.h>



using namespace Ace;
//






/*!
 * Constructs a new settings run object with the given command arguments. 
 *
 * @param command  
 */
SettingsRun::SettingsRun(const Command& command):
   _command(command)
{}






/*!
 * Executes this object, processing its command arguments to determine and execute 
 * the specific settings command that has been given by the user. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the command argument size is empty then simply print all ACE settings and 
 *    exit, else call the setting parser method to determine which command was 
 *    given. 
 */
void SettingsRun::execute()
{

   if ( _command.size() == 0 )
   {
      QTextStream stream(stdout);
      Ace::Settings& settings {Ace::Settings::instance()};
      stream << "          OpenCL Device: " << openCLDeviceString() << "\n";
      stream << "        ACU Thread Size: " << QString::number(settings.threadSize()) << "\n";
      stream << "        MPI Buffer Size: " << QString::number(settings.bufferSize()) << "\n";
      stream << "Chunk Working Directory: " << settings.chunkDir() << "\n";
      stream << "           Chunk Prefix: " << settings.chunkPrefix() << "\n";
      stream << "        Chunk Extension: " << settings.chunkExtension() << "\n";
   }
   else
   {
      settings();
   }
}






/*!
 * Parses the first command argument to determine if it is set or list. If it is 
 * neither then an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call set or list methods based off the first command argument being set or 
 *    list respectively. If the first command argument is neither then throw an 
 *    exception. 
 */
void SettingsRun::settings()
{
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Return the OpenCL device setting as a string formatted as the platform and 
 *    device index separated by a colon. If no device is set then return "none". 
 */
QString SettingsRun::openCLDeviceString()
{
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Determine which setting is to be set, calling the appropriate method and 
 *    popping this object's first command argument. If the setting is unknown then 
 *    throw an exception. 
 */
void SettingsRun::set()
{
   enum {Unknown=-1,OpenCLCom,Threads,Buffer,ChunkDir,ChunkPre,ChunkExt};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set requires sub argument, exiting..."));
      throw e;
   }
   QStringList list {"opencl","threads","buffer","chunkdir","chunkpre","chunkext"};
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    go the next step. 
 *
 * 2. Attempt to parse the platform and device indexes from this object's first 
 *    command argument. If the first argument is the special string "none" then set 
 *    the platform index to -1 and device index to 0 denoting no device. If parsing 
 *    the first command argument fails then throw an exception. 
 *
 * 3. Set the new platform and device index to ACE global settings. 
 */
void SettingsRun::setOpenCL()
{
   auto invalid = [this]()
   {//
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given ACU device '%1' invalid, exiting...").arg(_command.first()));
      throw e;
}






/*!
 * Executes the settings set threads command, setting the global thread size 
 * setting for ACE. If the new thread size given by the first command argument is 
 * invalid or less than one then an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    go to the next step. 
 *
 * 2. Read in the new thread size as an integer and update the ACE global setting. 
 *    If parsing the new thread size fails or it is less than one then throw an 
 *    exception. 
 */
void SettingsRun::setThreads()
{
   auto invalid = [this]()
   {//
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given thread size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
}






/*!
 * Executes the settings set buffer command, setting the global buffer size setting 
 * for ACE. If the new buffer size given by the first command argument is invalid 
 * or less than one then an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    go to the next step. 
 *
 * 2. Read in the new buffer size as an integer and update the ACE global setting. 
 *    If parsing the new buffer size fails or it is less than one then throw an 
 *    exception. 
 */
void SettingsRun::setBuffer()
{
   auto invalid = [this]()
   {//
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given buffer size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
}






/*!
 * Executes the setting set chunkdir command, setting the global chunk working 
 * directory setting for ACE. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    set the global chunk working directory for ACE to the first command argument. 
 */
void SettingsRun::setChunkDir()
{
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    set the global chunk prefix for ACE to the first command argument. 
 */
void SettingsRun::setChunkPre()
{
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    set the global chunk extension for ACE to the first command argument. 
 */
void SettingsRun::setChunkExt()
{
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
 * Executes the settings list command, parsing the first argument to determine 
 * which specific list command is to be executed and calling the appropriate 
 * method. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is empty then throw an exception, else 
 *    go to the next step. 
 *
 * 2. Parse the first argument to determine which list command is requested, 
 *    calling the appropriate method for the command given. If the command is not 
 *    recognized then throw an exception. 
 */
void SettingsRun::list()
{
   enum {Unknown=-1,OpenCLCom};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings list requires sub argument, exiting..."));
      throw e;
   }
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
 *
 *
 * Steps of Operation: 
 *
 * 1. List all available OpenCL devices to standard output by their indexes and 
 *    name. 
 */
void SettingsRun::listOpenCL()
{
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






/*! !!! UNKNOWN FUNCTION !!! */
if ( size < 1 )
{
   invalid();
}






/*! !!! UNKNOWN FUNCTION !!! */
if ( size < 1 )
{
   invalid();
}
