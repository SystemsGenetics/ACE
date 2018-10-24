#include "client.cpp"
//






/*!
 * Implements the main function. 
 *
 * @param argc See GCC docs. 
 *
 * @param argv See GCC docs. 
 *
 * @return See GCC docs. 
 */
int main(int argc, char** argv)
{
   // Initialize qt core application. 
   QCoreApplication application(argc,argv);

   // Create a new client and connect its destroyed signal to the application quit 
   // slot. 
   Client* client {new Client};
   QObject::connect(client,&QObject::destroyed,&application,&QCoreApplication::quit);

   // Execute the application. 
   return application.exec();
}
