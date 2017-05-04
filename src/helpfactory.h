#ifndef HELPFACTORY_H
#define HELPFACTORY_H
#include <memory>
#include <vector>



class AbstractHelpItem;



class HelpFactory
{
public:
   static HelpFactory& getInstance();
   bool addItem(std::unique_ptr<AbstractHelpItem>&& item);
   int getItemCount() const;
   std::string getItemName(int row) const;
   std::string getItemDescription(int row) const;
private:
   HelpFactory() = default;
   static HelpFactory* _instance;
   std::vector<std::unique_ptr<AbstractHelpItem>> _items;
};



#endif
