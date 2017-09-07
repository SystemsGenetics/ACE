#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#include <memory>

#define E_MAKE_EXCEPTION(e) EException e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__);



class EException
{
public:
   /// Set function name where exception occured.
   ///
   /// @param function Function name.
   void setFunction(const QString& function) noexcept { _function = function; }
   /// Set file name where exception occured.
   ///
   /// @param file File name.
   void setFile(const QString& file) noexcept { _file = file; }
   /// Set file line number where exception occured.
   ///
   /// @param line Line number.
   void setLine(unsigned int line) noexcept { _line = line; }
   /// Set title of exception.
   ///
   /// @param title Title.
   void setTitle(const QString& title) noexcept { _title = title; }
   /// Set detailed description of exception.
   ///
   /// @param details Detailed description.
   void setDetails(const QString& details) noexcept { _details = details; }
   /// Get function name where exception occured.
   ///
   /// @return Function name.
   QString getFunction() const noexcept { return _function; }
   /// Get file name where exception occured.
   ///
   /// @return File name.
   QString getFile() const noexcept { return _file; }
   /// Get file line number where exception occured.
   ///
   /// @return Line number.
   int getLine() const noexcept { return _line; }
   /// Get title of exception.
   ///
   /// @param Title.
   QString getTitle() const noexcept { return _title; }
   /// Get detailed description of exception.
   ///
   /// @return Detailed description.
   QString getDetails() const noexcept { return _details; }
private:
   QString _function;
   QString _file;
   int _line {-1};
   QString _title;
   QString _details;
};



class ESilent
{
public:
   /// Get last exception that occured on object.
   ///
   /// @return Exception.
   EException getException() const noexcept;
   /// Test if exception has occured on object.
   ///
   /// @return False if an exception has occured.
   operator bool() const noexcept { return !_hasException; }
protected:
   void setException(EException exception) const noexcept;
private:
   mutable std::unique_ptr<EException> _exception {nullptr};
   mutable bool _hasException {false};
};



#endif
