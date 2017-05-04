#include "helpfactory.h"
#include "abstracthelpitem.h"



using namespace std;
HelpFactory* HelpFactory::_instance = nullptr;






HelpFactory &HelpFactory::getInstance()
{
   if ( !_instance )
   {
      _instance = new HelpFactory;
   }
   return *_instance;
}





bool HelpFactory::addItem(std::unique_ptr<AbstractHelpItem>&& item)
{
   for (auto i = _items.begin(); i != _items.end() ;++i)
   {
      if ( (*i)->getName() == item->getName() )
      {
         return false;
      }
   }
   _items.push_back(move(item));
}





int HelpFactory::getItemCount() const
{
   return _items.size();
}





string HelpFactory::getItemName(int row) const
{
   if ( row >= _items.size() )
   {
      return string();
   }
   return _items.at(row)->getName();
}





string HelpFactory::getItemDescription(int row) const
{
   if ( row >= _items.size() )
   {
      return string();
   }
   return _items.at(row)->getDescription();
}
