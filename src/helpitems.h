#ifndef HELPITEMS_H
#define HELPITEMS_H
#include "abstracthelpitem.h"



class CLHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "cl";
   }
   std::string getDescription() const override final
   {
      return "cl <list|set|clear>\n"
             "\n"
             "list - list all available opencl devices\n"
             "set n:m - set active opencl device to nth platform's mth device\n"
             "clear - clear selected opencl device\n";
   }
};



#endif
