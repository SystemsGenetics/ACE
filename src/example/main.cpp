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
   EApplication application(""
                            ,"aceex"
                            ,0
                            ,0
                            ,999
                            ,unique_ptr<DataFactory>(new DataFactory)
                            ,unique_ptr<AnalyticFactory>(new AnalyticFactory)
                            ,argc
                            ,argv);
   return application.exec();
}
