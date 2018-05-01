#include "ace_settingsrun.h"
#include <QTextStream>
#include <core/ace_settings.h>
#include <core/opencl_platform.h>
#include <core/opencl_device.h>
#include <core/eexception.h>



using namespace Ace;
//






/*!
 *
 * @param command  
 */
SettingsRun::SettingsRun(const Command& command):
   _command(command)
{}






/*!
 */
void SettingsRun::execute()
{

   if ( _command.size() == 0 )
   {
      QTextStream stream(stdout);
      Ace::Settings& settings {Ace::Settings::instance()};
      stream << "             ACU Device: " << deviceString() << "\n";
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
 */
QString SettingsRun::deviceString()
{
   QString ret {"None"};
   Ace::Settings& settings {Ace::Settings::instance()};
   int platform {settings.platform()};
   int device {settings.device()};
   if ( platform >= 0 && device >= 0 )
   {
      ret = QString::number(platform).append(":").append(QString::number(device));
   }
   return ret;
}






/*!
 */
void SettingsRun::set()
{
   enum {Unknown=-1,ACU,Threads,Buffer,ChunkDir,ChunkPre,ChunkExt};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings set requires sub argument, exiting..."));
      throw e;
   }
   QStringList list {"acu","threads","buffer","chunkdir","chunkpre","chunkext"};
   QString command {_command.first()};
   switch (_command.pop(list))
   {
   case ACU:
      setACU();
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
   case Unknown:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings set sub argument '%1' unrecognized, exiting...")
                      .arg(command));
         throw e;
      }
   }
}






/*!
 */
void SettingsRun::setACU()
{
   auto invalid = [this]()
   {//
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
   Ace::Settings& settings {Ace::Settings::instance()};
   settings.setPlatform(platform);
   settings.setDevice(device);
}






/*!
 */
void SettingsRun::setThreads()
{
   auto invalid = [this]()
   {//
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
 */
void SettingsRun::setBuffer()
{
   auto invalid = [this]()
   {//
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
 */
void SettingsRun::list()
{
   enum {Unknown=-1,ACU};
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid argument"));
      e.setDetails(QObject::tr("Settings list requires sub argument, exiting..."));
      throw e;
   }
   QStringList list {"acu"};
   QString command {_command.first()};
   switch (_command.pop(list))
   {
   case ACU:
      listACU();
      break;
   case Unknown:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Invalid argument"));
         e.setDetails(QObject::tr("Settings list sub argument '%1' unrecognized, exiting...")
                      .arg(command));
         throw e;
      }
   }
}






/*!
 */
void SettingsRun::listACU()
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
