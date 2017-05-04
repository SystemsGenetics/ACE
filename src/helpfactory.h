#ifndef HELPFACTORY_H
#define HELPFACTORY_H



class HelpFactory
{
public:
   static HelpFactory& getInstance();
   bool fartyPants();
private:
   HelpFactory();
   static HelpFactory* _instance;
};



#endif
