#ifndef ACE_COMMAND_H
#define ACE_COMMAND_H
#include <QStringList>



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all arguments parsed from the command line using the variables
    * passed to the main function. This class also allowed for popping the very
    * first argument off the list of arguments.
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
       * List of arguments.
       */
      QStringList _values;
   };
}

#endif
