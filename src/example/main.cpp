#include <core/core.h>
#include <console/eapplication.h>
#include "analyticfactory.h"
#include "datafactory.h"



using namespace std;






int main(int argc, char *argv[])
{
   EAbstractAnalyticFactory::setInstance(unique_ptr<AnalyticFactory>(new AnalyticFactory));
   EAbstractDataFactory::setInstance(unique_ptr<DataFactory>(new DataFactory));
   EApplication application(argc,argv,"Example ACE Program","example");
   return application.exec();
}
