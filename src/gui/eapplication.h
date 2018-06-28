#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <memory>
#include <QApplication>
#include "../core/global.h"
//



/*!
 * This is the GUI application for the ACE program. This class initializes the ACE 
 * system with the given application information and factories, along with passing 
 * any command line arguments to Qt for parsing. 
 */
class EApplication : public QApplication
{
   Q_OBJECT
public:
   explicit EApplication(const QString& organization, const QString& application, int majorVersion, int minorVersion, int revision, std::unique_ptr<EAbstractDataFactory>&& data, std::unique_ptr<EAbstractAnalyticFactory>&& analytic, int& argc, char** argv);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
private:
   void showException(const EException& exception);
};



#endif
