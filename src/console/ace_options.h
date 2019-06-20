#ifndef ACE_OPTIONS_H
#define ACE_OPTIONS_H
#include <QHash>



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all options parsed from the command line using the variables
    * passed to the main function. This class provides methods for querying the
    * options parsed.
    */
   class Options
   {
   public:
      Options(int argc, char** argv);
      int size() const;
      bool contains(const QString& key) const;
      QString key(int index) const;
      QString value(int index) const;
      QString find(const QString& key) const;
   private:
      /*!
       * Mapping of all parsed options as key value pairs.
       */
      QHash<QString,QString> _map;
   };
}

#endif
