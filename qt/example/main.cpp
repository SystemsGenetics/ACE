#include <AceCore.h>

#include "analyticfactory.h"



using namespace std;






int main(int argc, char *argv[])
{
   unique_ptr<AnalyticFactory> analyticFactory(new AnalyticFactory);
   EAbstractAnalyticFactory::setInstance(move(analyticFactory));
   EApplication application(argc,argv,"KINC");
   return application.exec();
}
