#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <QApplication>



class EApplication : public QApplication
{
   Q_OBJECT
public:
   EApplication(int& argc, char** argv, const QString& title);
   bool notify(QObject* receiver, QEvent* event) override final;
};



#endif
