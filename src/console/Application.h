#ifndef APPLICATION_H
#define APPLICATION_H
#include <QtCore>



class EException;
class EMetadata;



class EApplication : public QCoreApplication
{
   Q_OBJECT
public:
   explicit EApplication(int& argc, char** argv, const QString& title, const QString& commandName);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
   int exec();
private slots:
   void completeUpdated(int percentComplete);
   void exceptionThrown(QString file, int line, QString function, QString title, QString details);
   void finished();
private:
   void showException(const EException& e);
   int run(int argc, char** argv);
   int dump(int argc, char** argv);
   int inject(int argc, char** argv);
   QJsonValue grabMetaValues(const EMetadata& meta);
   EMetadata* buildMetaValues(QJsonValue json);
   int _argc;
   char** _argv;
   QString _command;
   int _percentComplete;
};



#endif
