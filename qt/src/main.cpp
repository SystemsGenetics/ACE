#include <QApplication>

#include "mainwindow.h"
#include "abstractanalyticfactory.h"
using namespace std;

struct AnalyticFactory : public AbstractAnalyticFactory
{
   int getCount() override final
   {
      return 3;
   }
   QString getName(int type) override final
   {
      switch (type)
      {
      case 0:
         return QString("Test 1");
      case 1:
         return QString("Test 2");
      case 2:
         return QString("Test 3");
      default:
         return QString();
      }
   }
   unique_ptr<AbstractAnalytic> makeAnalytic(int /*type*/) override final
   {
      return nullptr;
   }
};






int main(int argc, char *argv[])
{
   QApplication application(argc, argv);
   AbstractAnalyticFactory::setInstance(unique_ptr<AnalyticFactory>(new AnalyticFactory));
   MainWindow window;
   window.show();
   return application.exec();
}
