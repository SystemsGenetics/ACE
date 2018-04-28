#include <core/core.h>
#include <console/eapplication.h>
#include "analyticfactory.h"
#include "datafactory.h"



using namespace std;






int main(int argc, char *argv[])
{
   EApplication application(""
                            ,"aceex"
                            ,unique_ptr<DataFactory>(new DataFactory)
                            ,unique_ptr<AnalyticFactory>(new AnalyticFactory)
                            ,argc
                            ,argv);
   return application.exec();
}
