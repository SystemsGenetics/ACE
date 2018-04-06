#include "eapplication.h"
#include <QTextStream>
#include <eexception.h>
#include "ace_run.h"
//






/*!
 *
 * @param argc  
 *
 * @param argv  
 */
EApplication::EApplication(int& argc, char** argv):
   QCoreApplication(argc,argv),
   _options(argc,argv),
   _command(argc,argv)
{}






/*!
 */
int EApplication::exec()
{
   try
   {
      enum {Unknown = -1,Run};
      QStringList commands {"run"};
      QString command {_command.first()};
      switch (_command.pop(commands))
      {
      case Run:
         new Ace::Run(_command,_options);
         break;
      case Unknown:
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(tr("Invalid argument"));
            e.setDetails(tr("Unknown command '%1'.").arg(command));
            throw e;
         }
      }
      return QCoreApplication::exec();
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
   return -1;
}






/*!
 *
 * @param exception  
 */
void EApplication::showException(const EException& exception)
{
   QTextStream stream(stdin);
   qDebug() << exception.fileName() << ":" << exception.line();
   qDebug() << exception.functionName();
   stream << exception.title().toUpper() << "\n" << exception.details() << "\n";
}
