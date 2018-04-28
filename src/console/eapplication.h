#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <memory>
#include <QCoreApplication>
#include <core/global.h>
#include "ace_options.h"
#include "ace_command.h"
//



/*!
 */
class EApplication : public QCoreApplication
{
   Q_OBJECT
public:
   EApplication(const QString& organization, const QString& application, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv);
   int exec();
private:
   void dump();
   void inject();
   QJsonDocument getJson();
   void inject(const QJsonDocument& document);
   void showException(const EException& exception);
   /*!
    */
   Ace::Options _options;
   /*!
    */
   Ace::Command _command;
};



#endif
