#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QtCore>
#define MAKE_EXCEPTION(e) Exception e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__)



class Exception
{
public:
   void setFunction(const QString& function);
   void setFile(const QString& file);
   void setLine(unsigned int line);
   void setTitle(const QString& title);
   QTextStream& out();
   void display();
private:
   QString _function;
   QString _file;
   unsigned int _line;
   QString _title;
   QString _description;
   QTextStream* _stream {nullptr};
};



#endif
