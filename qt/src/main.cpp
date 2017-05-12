#include <QApplication>

#include "mainwindow.h"
#include "abstractanalyticfactory.h"
#include "AceCore.h"
using namespace std;

struct AnalyticFactory : public EAbstractAnalyticFactory
{
   quint16 getCount() override final
   {
      return 3;
   }
   QString getName(quint16 type) override final
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
   unique_ptr<EAbstractAnalytic> makeAnalytic(quint16 /*type*/) override final
   {
      return nullptr;
   }
};






int main(int argc, char *argv[])
{
   EAbstractAnalyticFactory::setInstance(unique_ptr<AnalyticFactory>(new AnalyticFactory));
   EApplication application(argc, argv);
   return application.exec();
}
// Ace::SetupAnalyticDialog, Ace::RunAnalyticDialog :)

//for data;
//!EDataStream, Ace::DataObject, EAbstractData, EAbstractDataFactory, Ace::DataManager :)
//Ace::DataWindow :))
