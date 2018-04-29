#include "ace_run.h"
#include <core/ace_analytic_manager.h>
#include <core/eabstractanalyticfactory.h>
#include <core/eexception.h>



using namespace Ace;
//






/*!
 *
 * @param command  
 *
 * @param options  
 */
Run::Run(const Command& command, const Options& options):
   _stream(stdout),
   _options(options),
   _command(command)
{
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("No analytic name given, exiting..."));
      throw e;
   }
   setupManager(getType());
}






/*!
 *
 * @param percentComplete  
 */
void Run::progressed(int percentComplete)
{
   _stream << "\r" << QString::number(percentComplete) << "%";
   _stream.flush();
}






/*!
 */
void Run::done()
{
   _stream << "\r100%\n";
}






/*!
 */
void Run::finished()
{
   deleteLater();
}






/*!
 */
quint16 Run::getType()
{
   QStringList commandNames;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }
   QString name {_command.pop()};
   int type {commandNames.indexOf(name)};
   if ( type < 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Unknown analytic '%1'; no analytic exists by that name.").arg(name));
      throw e;
   }
   return static_cast<quint16>(type);
}






/*!
 *
 * @param type  
 */
void Run::setupManager(quint16 type)
{
   _manager = Analytic::Manager::makeManager(type,0,1).release();
   _manager->setParent(this);
   connect(_manager,&Analytic::Manager::progressed,this,&Run::progressed);
   connect(_manager,&Analytic::Manager::done,this,&Run::done);
   connect(_manager,&Analytic::Manager::finished,this,&Run::finished);
   addArguments();
   _manager->initialize();
}






/*!
 */
void Run::addArguments()
{
   for (int i = 0; i < _manager->size() ;++i)
   {
      QString argumentName
      {//
         _manager->data(i,EAbstractAnalytic::Input::Role::CommandLineName).toString()
      };
      switch (_manager->type(i))
      {
      case EAbstractAnalytic::Input::Type::Boolean:
      case EAbstractAnalytic::Input::Type::String:
      case EAbstractAnalytic::Input::Type::FileIn:
      case EAbstractAnalytic::Input::Type::FileOut:
      case EAbstractAnalytic::Input::Type::DataIn:
      case EAbstractAnalytic::Input::Type::DataOut:
         _manager->set(i,_options.find(argumentName));
         break;
      case EAbstractAnalytic::Input::Type::Integer:
         addInteger(i,argumentName);
         break;
      case EAbstractAnalytic::Input::Type::Double:
         addDouble(i,argumentName);
         break;
      case EAbstractAnalytic::Input::Type::Selection:
         addSelection(i,argumentName);
         break;
      }
   }
}






/*!
 *
 * @param index  
 *
 * @param name  
 */
void Run::addInteger(int index, const QString& name)
{
   int value {_options.find(name).toInt()};
   int minimum {_manager->data(index,EAbstractAnalytic::Input::Role::Minimum).toInt()};
   int maximum {_manager->data(index,EAbstractAnalytic::Input::Role::Maximum).toInt()};
   if ( value < minimum || value > maximum )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Integer argument '%1' cannot be set to %2 (%3-%4 is valid range).")
                   .arg(name)
                   .arg(value)
                   .arg(minimum)
                   .arg(maximum));
      throw e;
   }
   _manager->set(index,value);
}






/*!
 *
 * @param index  
 *
 * @param name  
 */
void Run::addDouble(int index, const QString& name)
{
   double value {_options.find(name).toDouble()};
   double minimum {_manager->data(index,EAbstractAnalytic::Input::Role::Minimum).toDouble()};
   double maximum {_manager->data(index,EAbstractAnalytic::Input::Role::Maximum).toDouble()};
   if ( value < minimum || value > maximum )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Floating point argument '%1' cannot be set to %2 (%3-%4 is valid range).")
                   .arg(name)
                   .arg(value)
                   .arg(minimum)
                   .arg(maximum));
      throw e;
   }
   _manager->set(index,value);
}






/*!
 *
 * @param index  
 *
 * @param name  
 */
void Run::addSelection(int index, const QString& name)
{
   QString value {_options.find(name)};
   QStringList values
   {//
      _manager->data(index,EAbstractAnalytic::Input::Role::SelectionValues).toStringList()
   };
   if ( !values.contains(value) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Selection argument '%1' does not contain '%2' as a valid option.")
                   .arg(name)
                   .arg(value));
      throw e;
   }
   _manager->set(index,value);
}
