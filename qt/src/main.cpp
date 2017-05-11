#include <QApplication>

#include "mainwindow.h"
#include "abstractanalyticfactory.h"
using namespace std;

struct AnalyticFactory : public EAbstractAnalyticFactory
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
   unique_ptr<EAbstractAnalytic> makeAnalytic(int /*type*/) override final
   {
      return nullptr;
   }
};






int main(int argc, char *argv[])
{
   QApplication application(argc, argv);
   EAbstractAnalyticFactory::setInstance(unique_ptr<AnalyticFactory>(new AnalyticFactory));
   Ace::MainWindow::getInstance().show();
   return application.exec();
}
// Ace::SetupAnalyticDialog, Ace::RunAnalyticDialog :)

//for data;
//!EDataStream, Ace::DataObject, EAbstractData, EAbstractDataFactory, Ace::DataManager :)
//Ace::DataWindow :))
