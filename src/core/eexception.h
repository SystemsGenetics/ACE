#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#include <memory>
#define E_MAKE_EXCEPTION(e) EException e;e.setFileName(__FILE__);e.setFunctionName(__PRETTY_FUNCTION__);e.setLine(__LINE__);
//



/*!
 * This handles all errors encountered in the ACE library and should be used by 
 * programs that use the ACE framework. This contains all information about where 
 * the exception was thrown along with a description about what the error. A macro 
 * is provided for creating exceptions that fills in the file name, function name, 
 * and line fields. This class is thrown as an exception on any ACE error. If the 
 * program using ACE does not catch a thrown error ACE itself will handle it, 
 * either reporting it to the user without crashing if in GUI mode or reporting it 
 * to the user and exiting if in console mode. 
 */
class EException
{
public:
   QString functionName() const noexcept;
   QString fileName() const noexcept;
   int line() const noexcept;
   QString title() const noexcept;
   QString details() const noexcept;
   void setFunctionName(const QString& functionName) noexcept;
   void setFileName(const QString& fileName) noexcept;
   void setLine(int line) noexcept;
   void setTitle(const QString& title) noexcept;
   void setDetails(const QString& details) noexcept;
private:
   /*!
    * The function name where this exception was thrown. 
    */
   QString _functionName;
   /*!
    * The file name where this exception was thrown. 
    */
   QString _fileName;
   /*!
    * The line number where this exception was thrown. 
    */
   int _line {-1};
   /*!
    * The title describing the error that caused this exception. 
    */
   QString _title;
   /*!
    * The detailed description of what caused this exception. 
    */
   QString _details;
};



#endif
