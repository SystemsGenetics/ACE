#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <memory>
#include <QCoreApplication>
#include <../core/global.h>
#include "ace_options.h"
#include "ace_command.h"
//



/*!
 * This is the console application instantiated and executed by a program using 
 * ACE. This class initializes the ACE system with the given application 
 * information and factories, along with parsing the command line and determining 
 * which command the user has given. If the command given is a dump or inject 
 * command this class handles those commands itself, otherwise the command is 
 * passed on to another class for specialized processing. This class continues 
 * execution until ACE and the program using it has completed and is exiting. There 
 * should only be one instance of this class for the entire program, most commonly 
 * instantiated within the main function. If multiple instances of this class exist 
 * the behavior of the program is undefined. 
 */
class EApplication : public QCoreApplication
{
   Q_OBJECT
public:
   explicit EApplication(const QString& organization, const QString& application, int majorVersion, int minorVersion, int revision, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
   int exec();
private:
   static QJsonDocument getJson(const QString& path);
   static void inject(const QString& path, const QJsonDocument& document);
   static void showException(const EException& exception);
   void dump();
   void inject();
   /*!
    * The options parsed out of the command line arguments given to the main function. 
    */
   Ace::Options _options;
   /*!
    * The command arguments parsed out of the command line arguments of the main 
    * function. 
    */
   Ace::Command _command;
};



#endif
