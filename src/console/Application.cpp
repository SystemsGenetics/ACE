#include <memory>
#include <QJsonDocument>

#include "Application.h"
#include "exception.h"
#include "abstractanalyticfactory.h"
#include "abstractanalytic.h"
#include "ace_datamanager.h"
#include "datareference.h"
#include "ace_dataobject.h"
#include "metadata.h"
#include "ace_qmpi.h"



using namespace std;






EApplication::EApplication(int& argc, char** argv, const QString& title
                                    , const QString& commandName):
   QCoreApplication(argc,argv),
   _argc(argc),
   _argv(argv)
{
   // GUI parameters are unused in console
   Q_UNUSED(title);
   Q_UNUSED(commandName);

   // Output full command used to string
   for (int i = 0; i < argc ;++i)
   {
      if ( i )
      {
         _command.append(' ');
      }
      _command.append(argv[i]);
   }
}






bool EApplication::notify(QObject* receiver, QEvent* event)
{
   // catch any exception thrown
   try
   {
      return QCoreApplication::notify(receiver,event);
   }
   catch (EException e)
   {
      // ACE exception caught, display
      showException(e);
   }
   catch (std::exception e)
   {
      // std exception caught
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
   }
   catch (...)
   {
      // unkonwn exception caught
      qDebug() << tr("Unknown exception caught!\n");
   }

   // inform qt that this event has been handled
   return false;
}






int EApplication::exec()
{
   // catch any exception thrown
   try
   {
      // make sure there is at least one argument
      if ( _argc > 1 )
      {
         // figure out if command given is run or dump
         QString which(_argv[1]);
         if ( which == QString("run") )
         {
            return run(_argc-2,&_argv[2]);
         }
         else if ( which == QString("dump"))
         {
            return dump(_argc-2,&_argv[2]);
         }
         else if ( which == QString("inject") )
         {
            return inject(_argc-2,&_argv[2]);
         }
      }
      return -1;
   }
   catch (EException e)
   {
      // ACE exception caught, display and exit
      showException(e);
      ::exit(-1);
   }
   catch (std::exception e)
   {
      // std exception caught, exit
      qDebug() << tr("STD exception %1 caught!\n").arg(e.what());
      ::exit(-1);
   }
   catch (...)
   {
      // unkonwn exception caught, exit
      qDebug() << tr("Unknown exception caught!\n");
      ::exit(-1);
   }
}






void EApplication::completeUpdated(int percentComplete)
{
   // only update if new percent complete is higher than previous reported
   if ( percentComplete > _percentComplete )
   {
      QTextStream stream(stdout);
      stream << "\r" << percentComplete << "%";
      _percentComplete = percentComplete;
   }
}






void EApplication::exceptionThrown(QString file, int line, QString function, QString title
                                   , QString details)
{
   // reconstitute ACE exception, display it, and exit
   EException e;
   e.setFile(file);
   e.setLine(line);
   e.setFunction(function);
   e.setTitle(title);
   e.setDetails(details);
   showException(e);
   quit();
}






void EApplication::finished()
{
   // output 100% and exit on success
   QTextStream stream(stdout);
   stream << "\r100%\n";
   quit();
}






void EApplication::showException(const EException& e)
{
   // Output exception information onto std out
   QTextStream stream(stdout);
   stream << tr("CRITICAL ERROR\n\n");
   stream << e.getTitle() << tr("\n\n");
   stream << e.getDetails() << tr("\n\n");
   stream << tr("File: %1\nLine: %2\nFunction: %3\n").arg(e.getFile()).arg(e.getLine())
             .arg(e.getFunction());
}






int EApplication::run(int argc, char** argv)
{
   Ace::QMPI& mpi {Ace::QMPI::initialize()};
   // using role and type enums
   using Role = EAbstractAnalytic::Role;
   using Type = EAbstractAnalytic::ArgumentType;
   unique_ptr<EAbstractAnalytic> analytic;

   if ( mpi.isMaster() )
   {
      // setup std out stream and make sure there are arguments
      QTextStream stream(stdout);
      if ( argc <= 0 )
      {
         stream << tr("No arguments given for run command, exiting.\n");
         return -1;
      }

      // get analytic name from first argument
      QString analyticName(argv[0]);
      --argc;
      argv = &argv[1];

      // go through analytics and find the one that matches the command line name given
      EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::getInstance()};
      for (int i = 0; i < factory.getCount() ;++i)
      {
         if ( factory.getCommandName(i) == analyticName )
         {
            analytic = factory.make(i);
            break;
         }
      }

      // make sure an analytic was found with given command line name
      if ( !analytic )
      {
         stream << tr("Cannot find analytic by the name \"%1\", exiting.\n").arg(analyticName);
         return -1;
      }

      // make sure the remaining number of arguments is even
      if ( argc%2 )
      {
         stream << tr("Invalid number of analytic arguments, exiting.\n");
         return -1;
      }

      // build a mapping of all analytic arguments and their command line name
      QMap<QString,int> argumentMap;
      for (int i = 0; i < analytic->getArgumentCount() ;++i)
      {
         argumentMap.insert(analytic->getArgumentData(i,Role::CommandLineName).toString(),i);
      }

      // iterate through all remaining arguments, taking two at a time, the first being the argument
      // name and the second being the value
      for (int i = 0; i < argc ;i+=2)
      {
         // get the argument name and make sure it is valid
         QString name(argv[i]);
         if ( name.at(0) != QChar('-') || name.at(1) != QChar('-') )
         {
            stream << tr("Invalid analytic argument \"%1\", exiting.\n").arg(name);
            return -1;
         }
         name.remove(0,2);

         // get the argument value
         QString value(argv[i+1]);

         // using analytic's argument map get which argument number this is, making sure one is found
         auto j = argumentMap.constFind(name);
         if ( j == argumentMap.constEnd() )
         {
            stream << tr("Cannot find analytic argument \"--%1\", exiting.\n").arg(name);
            return -1;
         }

         // now set analytic's argument depending on what type it is
         switch (analytic->getArgumentData(*j))
         {
         case Type::Bool:
         {
            // get boolean value
            bool val {false};
            if ( value == QString("true") )
            {
               val = true;
            }

            // set argument
            analytic->setArgument(*j,val);
            break;
         }
         case Type::Integer:
         {
            // get integer value
            bool ok {false};
            int val;
            val = value.toInt(&ok);

            // make sure integer value was grabbed
            if ( !ok )
            {
               stream << tr("Expected argument \"%1\" to be an integer, exiting.").arg(name);
               return -1;
            }

            // make sure integer value is within limits
            if ( val < analytic->getArgumentData(*j,Role::Minimum).toInt()
                 || val > analytic->getArgumentData(*j,Role::Maximum).toInt() )
            {
               stream << tr("Integer argument \"%1\" is out of bounds, exiting.").arg(name);
               return -1;
            }

            // set argument
            analytic->setArgument(*j,val);
            break;
         }
         case Type::Double:
         {
            // get floating point value
            bool ok {false};
            double val;
            val = value.toDouble(&ok);

            // make sure floating point was grabbed
            if ( !ok )
            {
               stream << tr("Expected argument \"%1\" to be a real number, exiting.").arg(name);
               return -1;
            }

            // make sure floating point is within limits
            if ( val < analytic->getArgumentData(*j,Role::Minimum).toDouble()
                 || val > analytic->getArgumentData(*j,Role::Maximum).toDouble() )
            {
               stream << tr("Real number argument \"%1\" is out of bounds, exiting.").arg(name);
               return -1;
            }

            // set argument
            analytic->setArgument(*j,val);
            break;
         }
         case Type::String:
            analytic->setArgument(*j,value);
            break;
         case Type::Combo:
            // make sure combo value is legal combo option
            if ( !analytic->getArgumentData(*j,Role::ComboValues).toStringList().contains(value) )
            {
               stream << tr("Combo argument \"%1\" contains illegal value, exiting.").arg(name);
               return -1;
            }

            // set argument
            analytic->setArgument(*j,value);
            break;
         case Type::FileIn:
            analytic->addFileIn(*j,value);
            break;
         case Type::FileOut:
            analytic->addFileOut(*j,value);
            break;
         case Type::DataIn:
            analytic->addDataIn(*j,value,analytic->getArgumentData(*j,Role::DataType).toUInt());
            break;
         case Type::DataOut:
            analytic->addDataOut(*j,value,analytic->getArgumentData(*j,Role::DataType).toUInt());
            break;
         }
      }

      // initialize percent complete reporting
      _percentComplete = 0;
      stream << "0%";
      stream.flush();

      // set analytic's command for metadata
      analytic->setCommand(_command);

      // connect all analytic signals
      connect(analytic.get(),SIGNAL(progressed(int)),SLOT(completeUpdated(int)));
   }
   connect(analytic.get(),SIGNAL(exceptionThrown(QString,int,QString,QString,QString)),this
           ,SLOT(exceptionThrown(QString,int,QString,QString,QString)));
   connect(analytic.get(),SIGNAL(finished()),this,SLOT(finished()));

   if ( mpi.size() == 1 )
   {
      analytic->start();
   }
   else
   {
      analytic->startMPI();
   }
   int ret {QCoreApplication::exec()};
   Ace::QMPI::shutdown();
   return ret;
}






int EApplication::dump(int argc, char** argv)
{
   // initialize stream for std out and make sure file argument is given
   QTextStream stream(stdout);
   if ( argc <= 0 )
   {
      stream << tr("No arguments given for dump command, exiting.\n");
      return -1;
   }

   // open data object file
   unique_ptr<Ace::DataReference> ref {Ace::DataManager::getInstance().open(argv[0])};
   Ace::DataObject* data {ref.get()->get()};
   data->open();

   // grab metadata of data object as JSON
   QJsonValue root = grabMetaValues(data->getMeta());
   QJsonDocument doc(root.toObject());

   // output JSON metadata and finish
   stream << doc.toJson();
   return 0;
}






int EApplication::inject(int argc, char **argv)
{
   // initialize stream for std output and make sure there is enough arguments
   QTextStream outStream(stdout);
   if ( argc <= 2 )
   {
      outStream << tr("Not enough arguments given for inject command, exiting.") << "\n";
      return -1;
   }

   // open json file and make sure it worked
   QFile jsonFile(argv[0]);
   if ( !jsonFile.open(QIODevice::ReadOnly|QIODevice::Text) )
   {
      outStream << tr("Failed to open json file for injection, exiting.") << "\n";
      return -1;
   }

   // read json from file
   QString json = jsonFile.readAll();
   QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

   // convert read in json to generic json value and make sure it worked
   QJsonValue root;
   if ( document.isArray() )
   {
      root = document.array();
   }
   else if ( document.isObject() )
   {
      root = document.object();
   }
   else
   {
      outStream << tr("Failed reading json file provided for injection, exiting.") << "\n";
      return -1;
   }

   // build new metadata from json data
   EMetadata* newMeta {buildMetaValues(root)};

   // open data object
   unique_ptr<Ace::DataReference> ref {Ace::DataManager::getInstance().open(argv[1])};
   Ace::DataObject* data {ref.get()->get()};
   data->open();

   // make sure key does not already exist
   if ( data->getMeta().toObject()->contains(argv[2]) )
   {
      outStream << tr("Key '%1' already exists in given data object file, new metadata NOT inserted"
                      " and exiting.").arg(argv[2]) << "\n";
      return -1;
   }

   // insert new metadata into data object and write to file
   data->getMeta().toObject()->insert(argv[2],newMeta);
   data->writeMeta();

   // return success
   return 0;
}






QJsonValue EApplication::grabMetaValues(const EMetadata &meta)
{
   // determine which type of metadata this is and set the JSON accordingly
   switch (meta.getType())
   {
   case EMetadata::Bool:
      return QJsonValue(*meta.toBool());
   case EMetadata::Double:
      return QJsonValue(*meta.toDouble());
   case EMetadata::String:
      return QJsonValue(*meta.toString());
   case EMetadata::Array:
   {
      // initialize JSON array and get list from metadata
      QJsonArray ret;
      const EMetadata::List* list = meta.toArray();

      // append all metadata to JSON array
      for (auto i = list->constBegin(); i != list->constEnd() ;++i)
      {
         ret.append(grabMetaValues(**i));
      }

      // return JSON array
      return QJsonValue(ret);
   }
   case EMetadata::Object:
   {
      // initialize JSON object and get map from metadata
      QJsonObject ret;
      const EMetadata::Map* map = meta.toObject();

      // insert all key/value metadata pairs to JSON object
      for (auto i = map->constBegin(); i != map->constEnd() ;++i)
      {
         ret.insert(i.key(),grabMetaValues(**i));
      }

      // return JSON object
      return QJsonValue(ret);
   }
   default:
      return QJsonValue();
   }
}






EMetadata* EApplication::buildMetaValues(QJsonValue json)
{
   // figure out what type of data the json value is
   if ( json.isBool() )
   {
      // copy json value to new metadata object and return
      EMetadata* ret {new EMetadata(EMetadata::Bool)};
      *(ret->toBool()) = json.toBool();
      return ret;
   }
   else if ( json.isDouble() )
   {
      // copy json value to new metadata object and return
      EMetadata* ret {new EMetadata(EMetadata::Double)};
      *(ret->toDouble()) = json.toDouble();
      return ret;
   }
   else if ( json.isString() )
   {
      // copy json value to new metadata object and return
      EMetadata* ret {new EMetadata(EMetadata::String)};
      *(ret->toString()) = json.toString();
      return ret;
   }
   else if ( json.isArray() )
   {
      // convert json to array and create empty metadata array
      QJsonArray values {json.toArray()};
      EMetadata* ret {new EMetadata(EMetadata::Array)};

      // copy all values from json array to metadata and return
      for (auto i = values.constBegin(); i != values.constEnd() ;++i)
      {
         // only append metadata if it is not null
         EMetadata* meta {buildMetaValues(*i)};
         if ( meta )
         {
            ret->toArray()->append(meta);
         }
      }
      return ret;
   }
   else if ( json.isObject() )
   {
      // convert json to object and create empty metadata object
      QJsonObject values {json.toObject()};
      EMetadata* ret {new EMetadata(EMetadata::Object)};

      // copy all key/value pairs from json object to metadata and return
      for (auto i = values.constBegin(); i != values.constEnd() ;++i)
      {
         // only insert metadata if it is not null
         EMetadata* meta {buildMetaValues(*i)};
         if ( meta )
         {
            ret->toObject()->insert(i.key(),meta);
         }
      }
      return ret;
   }
   else
   {
      // json is of an unsupported type, return null
      return nullptr;
   }
}
