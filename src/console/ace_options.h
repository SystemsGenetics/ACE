#ifndef ACE_OPTIONS_H
#define ACE_OPTIONS_H
#include <QHash>
//



namespace Ace
{
   /*!
    */
   class Options
   {
   public:
      Options(int argc, char** argv);
      int size() const;
      QString key(int index) const;
      QString value(int index) const;
      QString find(const QString& key) const;
   private:
      /*!
       */
      QHash<QString,QString> _map;
   };
}



#endif
