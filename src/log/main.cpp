#include "client.cpp"
//






/*!
 *
 * @param argc  
 *
 * @param argv  
 */
int main(int argc, char** argv)
{
   QCoreApplication application(argc,argv);
   Client* client {new Client};
   QObject::connect(client,&QObject::destroyed,&application,&QCoreApplication::quit);
   return application.exec();
}
