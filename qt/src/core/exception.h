#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#include <memory>

#define E_MAKE_EXCEPTION(e) EException e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__);



class EException
{
public:
   void setFunction(const QString& function) noexcept { _function = function; }
   void setFile(const QString& file) noexcept { _file = file; }
   void setLine(unsigned int line) noexcept { _line = line; }
   void setTitle(const QString& title) noexcept { _title = title; }
   void setDetails(const QString& details) noexcept { _details = details; }
   QString getFunction() const noexcept { return _function; }
   QString getFile() const noexcept { return _file; }
   int getLine() const noexcept { return _line; }
   QString getTitle() const noexcept { return _title; }
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
   EException getException() const noexcept;
   operator bool() const noexcept { return !_hasException; }
protected:
   void setException(EException exception) noexcept;
private:
   std::unique_ptr<EException> _exception {nullptr};
   bool _hasException {false};
};



#endif
