#ifndef ABSTRACTHELPITEM_H
#define ABSTRACTHELPITEM_H
#include <string>



class AbstractHelpItem
{
public:
   virtual ~AbstractHelpItem() = default;
   virtual std::string getName() const = 0;
   virtual std::string getDescription() const = 0;
};



#endif
