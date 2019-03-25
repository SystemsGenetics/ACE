#include <core/core.h>
#if(GUI == 0)
#include <console/eapplication.h>
#else
#include <gui/eapplication.h>
#endif
#include "analyticfactory.h"
#include "datafactory.h"



using namespace std;






int main(int argc, char *argv[])
{
   EApplication application("SystemsGenetics"
                            ,"aceex"
                            ,MAJOR_VERSION
                            ,MINOR_VERSION
                            ,REVISION
                            ,unique_ptr<DataFactory>(new DataFactory)
                            ,unique_ptr<AnalyticFactory>(new AnalyticFactory)
                            ,argc
                            ,argv);
   return application.exec();
}
