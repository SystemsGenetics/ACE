#include "ace_settingsrun.h"
#include <QTextStream>
#include "../core/ace_settings.h"
#include "../core/cuda_device.h"
#include "../core/opencl_platform.h"
#include "../core/opencl_device.h"
#include "../core/eexception.h"
#include "../core/edebug.h"



namespace Ace
{






/*!
 * Constructs a new settings run object with the given command arguments.
 *
 * @param command List of command line arguments for this settings run.
 */
SettingsRun::SettingsRun(const Command& command)
   :
   _command(command)
{
   EDEBUG_FUNC(this,&command);
}






/*!
 * Executes this object, processing its command arguments to determine and
 * execute the specific settings command that has been given by the user.
 */
void SettingsRun::execute()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If the command argument size is empty then simply print all ACE settings and
   // exit.
   if ( _command.size() == 0 )
   {
      QTextStream stream(stdout);
      Ace::Settings& settings {Ace::Settings::instance()};
      stream << "SETTINGS\n\n";
      stream << "            CUDA Device: " << cudaDeviceString() << "\n";
      stream << "          OpenCL Device: " << openCLDeviceString() << "\n";
      stream << "CUDA/OpenCL Thread Size: " << QString::number(settings.threadSize()) << "\n";
      stream << "        MPI Buffer Size: " << QString::number(settings.bufferSize()) << "\n";
      stream << "Chunk Working Directory: " << settings.chunkDir() << "\n";
      stream << "           Chunk Prefix: " << settings.chunkPrefix() << "\n";
      stream << "        Chunk Extension: " << settings.chunkExtension() << "\n";
      stream << "                Logging: " << ( settings.loggingEnabled() ? QStringLiteral("on") : QStringLiteral("off") ) << "\n";
      stream << "           Logging Port: " << settings.loggingPort() << "\n";
   }

   // Else call the setting parser method to determine which command was given.
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
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Setup the string list of known arguments that is used to determine the command
   // given.
   enum {Unknown=-1,Set,List};
   QStringList commandList {"set","list"};

   // Figure out which command was given, popping the command from the command list.
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
         // The command was not recognized so throw an exception informing the user.
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Unknown sub argument '%1', exiting...").arg(command));
         throw e;
      }
   }
}






/*!
 * Returns the CUDA device setting as a string.
 *
 * @return CUDA device setting.
 */
QString SettingsRun::cudaDeviceString()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create the return string, setting it to "none" as the default if no CUDA device
   // is set.
   QString ret {"none"};

   // If a CUDA device is set then overwrite the return string with its device
   // number.
   Ace::Settings& settings {Ace::Settings::instance()};
   int device {settings.cudaDevice()};
   if ( device >= 0 )
   {
      ret = QString::number(device);
   }

   // Return the string.
   return ret;
}






/*!
 * Returns the OpenCL device setting as a string.
 *
 * @return OpenCL device setting.
 */
QString SettingsRun::openCLDeviceString()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create the return string, setting it to "none" as the default if no OpenCL
   // device is set.
   QString ret {"none"};

   // If an OpenCL device is set then overwrite the return string with its platform
   // and device numbers.
   Ace::Settings& settings {Ace::Settings::instance()};
   int platform {settings.openCLPlatform()};
   int device {settings.openCLDevice()};
   if ( platform >= 0 && device >= 0 )
   {
      ret = QString::number(platform).append(":").append(QString::number(device));
   }

   // Return the string.
   return ret;
}






/*!
 * Executes the settings set command. This simply parses for which setting is to
 * be set and calls the appropriate method. If the setting is unknown then an
 * exception is thrown.
 */
void SettingsRun::set()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is at least one command to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set requires sub argument, exiting..."));
      throw e;
   }

   // Create an enumeration and string list used to determine the command given.
   enum {Unknown=-1,CUDACom,OpenCLCom,Threads,Buffer,ChunkDir,ChunkPre,ChunkExt,Logging,LogPort};
   QStringList list {"cuda","opencl","threads","buffer","chunkdir","chunkpre","chunkext","logging","logport"};

   // Determine which setting is to be set by the command given, calling the
   // appropriate method and popping this object's first command argument.
   QString command {_command.pop()};
   switch (list.indexOf(command))
   {
   case CUDACom:
      setCUDA();
      break;
   case OpenCLCom:
      setOpenCL();
      break;
   case Threads:
      setThreads();
      break;
   case Buffer:
      setBuffer();
      break;
   case ChunkDir:
      setChunkDir();
      break;
   case ChunkPre:
      setChunkPre();
      break;
   case ChunkExt:
      setChunkExt();
      break;
   case Logging:
      setLogging();
      break;
   case LogPort:
       setLogPort();
       break;
   case Unknown:
      {
         // The set command is not known so throw an exception informing the user.
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings set sub argument '%1' unrecognized, exiting...")
                      .arg(_command.first()));
         throw e;
      }
   }
}






/*!
 * Executes the settings set cuda command. This simply takes the device index
 * and sets the CUDA device with that index. The special "none" string sets the
 * CUDA device to none.
 */
void SettingsRun::setCUDA()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set cuda requires sub argument, exiting..."));
      throw e;
   }

   // Create a new device index, initializing it to -1 which assumes it is being set
   // to none.
   int device {-1};

   // Check to see if the command is something besides the special none keyword.
   if ( _command.first() != QString("none") )
   {
      // Get the integer device index from the command argument, making sure it worked
      // and is a valid device number.
      bool ok;
      device = _command.first().toInt(&ok);
      if ( !ok || device < 0 || device >= CUDA::Device::size() )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Given CUDA device '%1' invalid, exiting...").arg(_command.first()));
         throw e;
      }
   }

   // Set the new device index to the ACE global settings.
   Ace::Settings::instance().setCUDADevice(device);
}






/*!
 * Executes the settings set opencl command. This simply takes the device
 * argument, parsing it into its platform and device index, and then setting the
 * OpenCL device with those indexes. The special "none" string sets the OpenCL
 * device to none. If parsing of the indexes fails then an exception is thrown.
 */
void SettingsRun::setOpenCL()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Define a macro function that informs the user the given OpenCL device is not
   // valid.
   auto invalid = [this]()
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given OpenCL device '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   };

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set OpenCL requires sub argument, exiting..."));
      throw e;
   }

   // Create and initialize platform and device indexes, setting them to none as
   // default.
   int platform {-1};
   int device {0};

   // Check to see if the command argument is something besides the special none
   // token.
   if ( _command.first() != QString("none") )
   {
      // Split the command argument into the platform and device index, making sure it
      // worked.
      QStringList list {_command.first().split(':')};
      if ( list.size() != 2 )
      {
         invalid();
      }

      // Get the index numbers, making sure it worked.
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

      // Make sure the extracted values are valid platform and device indexes.
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
 * setting for ACE. If the new thread size given by the first command argument
 * is invalid or less than one then an exception is thrown.
 */
void SettingsRun::setThreads()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set threads requires sub argument, exiting..."));
      throw e;
   }

   // Read in the new thread size as an integer, making sure it worked and the size
   // is valid.
   bool ok;
   int size {_command.first().toInt(&ok)};
   if ( !ok || size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given thread size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   }

   // Set the new thread size to ACE global settings.
   Ace::Settings::instance().setThreadSize(size);
}






/*!
 * Executes the settings set buffer command, setting the global buffer size
 * setting for ACE. If the new buffer size given by the first command argument
 * is invalid or less than one then an exception is thrown.
 */
void SettingsRun::setBuffer()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set buffer requires sub argument, exiting..."));
      throw e;
   }

   // Read in the new buffer size as an integer, making sure it worked and the size
   // is valid.
   bool ok;
   int size {_command.first().toInt(&ok)};
   if ( !ok || size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Given buffer size '%1' invalid, exiting...").arg(_command.first()));
      throw e;
   }

   // Set the new buffer size to ACE global settings.
   Ace::Settings::instance().setBufferSize(size);
}






/*!
 * Executes the setting set chunkdir command, setting the global chunk working
 * directory setting for ACE.
 */
void SettingsRun::setChunkDir()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkdir requires sub argument, exiting..."));
      throw e;
   }

   // Set the new chunk working directory to ACE global settings.
   Ace::Settings::instance().setChunkDir(_command.first());
}






/*!
 * Executes the setting set chunkpre command, setting the global chunk prefix
 * setting for ACE.
 */
void SettingsRun::setChunkPre()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkpre requires sub argument, exiting..."));
      throw e;
   }

   // Set the new chunk prefix to ACE global settings.
   Ace::Settings::instance().setChunkPrefix(_command.first());
}






/*!
 * Executes the setting set chunkdir command, setting the global chunk extension
 * setting for ACE.
 */
void SettingsRun::setChunkExt()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set chunkext requires sub argument, exiting..."));
      throw e;
   }

   // Set the new chunk extension to ACE global settings.
   Ace::Settings::instance().setChunkExtension(_command.first());
}






/*!
 * Executes the setting set logging command, setting the global logging enabled
 * state for ACE.
 */
void SettingsRun::setLogging()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set logging requires sub argument, exiting..."));
      throw e;
   }

   // Set the state of logging to ACE global settings, using the on keyword to enable
   // it or disable it if the argument is any other string.
   Ace::Settings::instance().setLoggingEnabled(_command.first() == QStringLiteral("on"));
}






void SettingsRun::setLogPort()
{
    // Add the debug header.
    EDEBUG_FUNC(this);

    // Make sure there is a command argument to process.
    if ( _command.size() < 1 )
    {
       E_MAKE_EXCEPTION(e);
       e.setTitle(QObject::tr("Invalid argument"));
       e.setDetails(QObject::tr("Settings set logport requires sub argument, exiting..."));
       throw e;
    }

    bool ok;
    int port {_command.first().toInt(&ok)};
    if ( !ok || port < 0 || port > 65535 )
    {
       E_MAKE_EXCEPTION(e);
       e.setTitle(QObject::tr("Invalid argument"));
       e.setDetails(QObject::tr("Given logging port '%1' invalid, exiting...").arg(_command.first()));
       throw e;
    }

    Ace::Settings::instance().setLoggingPort(port);
}






/*!
 * Executes the settings list command, parsing the first argument to determine
 * which specific list command is to be executed and calling the appropriate
 * method.
 */
void SettingsRun::list()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there is a command argument to process.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings list requires sub argument, exiting..."));
      throw e;
   }

   // Create the list and enumeration of possible list commands that are recognized.
   enum {Unknown=-1,CUDACom,OpenCLCom};
   QStringList list {"cuda","opencl"};

   // Determine which list command has been given by the user and execute it.
   switch (_command.peek(list))
   {
   case CUDACom:
      listCUDA();
      break;
   case OpenCLCom:
      listOpenCL();
      break;
   case Unknown:
      {
         // The list command was not recognized so throw an exception informing the user.
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings list sub argument '%1' unrecognized, exiting...")
                      .arg(_command.first()));
         throw e;
      }
   }
}






/*!
 * Executes the settings list cuda command, listing all available CUDA devices
 * by their device index and name to standard output.
 */
void SettingsRun::listCUDA()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create a text stream connected to standard output.
   QTextStream stream (stdout);

   // Iterate through all available CUDA devices, printing each device index and name
   // to standard input for the user.
   for(int d = 0; d < CUDA::Device::size() ;++d)
   {
      stream << QString::number(d) << " " << CUDA::Device::get(d)->name() << "\n";
   }
}






/*!
 * Executes the settings list OpenCL command, listing all available OpenCL
 * devices by their platform and device indexes and name to standard output. The
 * indexes are formatted the same way an OpenCL device is specified in the set
 * command.
 */
void SettingsRun::listOpenCL()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create a text stream connected to standard output.
   QTextStream stream (stdout);

   // Iterate through all available OpenCL platforms.
   for (int p = 0; p < OpenCL::Platform::size() ;++p)
   {
      // Iterate through all available devices of the OpenCL platform.
      for(int d = 0; d < OpenCL::Platform::get(p)->deviceSize() ;++d)
      {
         // Print the platform index, device index, and device name of the current device
         // to standard output.
         stream << QString::number(p)
                << ":"
                << QString::number(d)
                << " "
                << OpenCL::Platform::get(p)->device(d)->name()
                << "\n";
      }
   }
}

}
