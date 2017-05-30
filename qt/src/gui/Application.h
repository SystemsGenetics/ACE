#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <QApplication>



class EException;



class EApplication : public QApplication
{
   Q_OBJECT
public:
   explicit EApplication(int& argc, char** argv, const QString& title);
   bool notify(QObject* receiver, QEvent* event) override final;
private:
   void showException(const EException& e);
};



#endif
