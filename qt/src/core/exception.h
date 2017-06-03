#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#include <memory>

#define E_MAKE_EXCEPTION(e) EException e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__);



class EException
{
public:
   enum Level
   {
      Notice
      ,Critical
   };
   void setFunction(const QString& function) noexcept;
   void setFile(const QString& file) noexcept;
   void setLine(unsigned int line) noexcept;
   void setTitle(const QString& title) noexcept;
   void setDetails(const QString& details) noexcept;
   void setType(int type) noexcept;
   void setLevel(Level level) noexcept;
   QString getFunction() const noexcept;
   QString getFile() const noexcept;
   int getLine() const noexcept;
   QString getTitle() const noexcept;
   QString getDetails() const noexcept;
   int getType() const noexcept;
   Level getLevel() const noexcept;
private:
   QString _function;
   QString _file;
   int _line {-1};
   QString _title;
   QString _description;
   QString _details;
   int _type {-1};
   Level _level {Critical};
};



class ESilent
{
public:
   bool hasException() const noexcept;
   EException getException() const noexcept;
   operator bool() const noexcept;
protected:
   void setException(EException exception) noexcept;
private:
   std::unique_ptr<EException> _exception {nullptr};
   bool _critical {false};
};



#endif
