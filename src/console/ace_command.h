#ifndef ACE_COMMAND_H
#define ACE_COMMAND_H
#include <QStringList>
//



namespace Ace
{
   /*!
    */
   class Command
   {
   public:
      Command(int argc, char** argv);
      int size() const;
      QString at(int index) const;
      QString first() const;
      QString pop();
      int pop(const QStringList& list);
      int peek(const QStringList& list);
   private:
      /*!
       */
      QStringList _values;
   };
}



#endif
