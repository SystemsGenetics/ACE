#include <AceCore.h>

#include "analyticfactory.h"
#include "datafactory.h"



using namespace std;






int main(int argc, char *argv[])
{
   unique_ptr<AnalyticFactory> analyticFactory(new AnalyticFactory);
   unique_ptr<DataFactory> dataFactory(new DataFactory);
   EAbstractAnalyticFactory::setInstance(move(analyticFactory));
   EAbstractDataFactory::setInstance(move(dataFactory));
   EApplication application(argc,argv,"KINC");
   return application.exec();
}
