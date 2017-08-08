#include <memory>

#include "Application.h"
#include "exception.h"
#include "abstractanalyticfactory.h"
#include "abstractanalytic.h"



using namespace std;






EApplication::EApplication(int& argc, char** argv, const QString& title
                                    , const QString& commandName):
   QCoreApplication(argc,argv),
   _argc(argc),
   _argv(argv)
{
   Q_UNUSED(title);
   Q_UNUSED(commandName);
}






bool EApplication::notify(QObject* receiver, QEvent* event)
{
   try
   {
      return QCoreApplication::notify(receiver,event);
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






int EApplication::exec()
{
   try
   {
      if ( _argc > 1 )
      {
         QString which(_argv[1]);
         if ( which == QString("run") )
         {
            return run(_argc-2,&_argv[2]);
         }
         else if ( which == QString("dump"))
         {
            return dump(_argc-2,&_argv[2]);
         }
      }
      return -1;
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






void EApplication::showException(const EException& e)
{
   QTextStream stream(stdout);
   stream << tr("CRITICAL ERROR") << "\n\n";
   stream << e.getTitle() << "\n\n";
   stream << e.getDetails() << "\n\n";
   stream << tr("File: %1\nLine: %2\nFunction: %3\n").arg(e.getFile()).arg(e.getLine())
             .arg(e.getFunction());
}






int EApplication::run(int argc, char** argv)
{
   if ( argc <= 0 )
   {
      return -1;
   }
   QString analyticName(argv[0]);
   --argc;
   argv = &argv[1];
   unique_ptr<EAbstractAnalytic> analytic;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::getInstance()};
   for (int i = 0; i < factory.getCount() ;++i)
   {
      if ( factory.getCommandName(i) == analyticName )
      {
         analytic = factory.make(i);
         break;
      }
   }
   if ( !analytic )
   {
      ;//ERROR!
   }
   if ( argc%2 )
   {
      ;//ERROR!
   }
   QMap<QString,int> argumentMap;
   for (int i = 0; i < analytic->getArgumentCount() ;++i)
   {
      argumentMap.insert(analytic->getArgumentData(i
                                           ,EAbstractAnalytic::Role::CommandLineName).toString(),i);
   }
   for (int i = 0; i < argc ;i+=2)
   {
      QString name(argv[i]);
      QString value(argv[i+1]);
      auto j = argumentMap.constFind(name);
      if ( j == argumentMap.constEnd() )
      {
         ;//ERROR!
      }
      // TODO: get argument and add it to analytic
   }
   analytic->start();
   return QCoreApplication::exec();
}






int EApplication::dump(int argc, char** argv)
{
}
