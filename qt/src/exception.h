#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#define E_MAKE_EXCEPTION(e) EException e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__);



class EException
{
public:
   enum class Type
   {
      Plain
      ,Analytic
      ,Data
   };
   void setFunction(const QString& function);
   void setFile(const QString& file);
   void setLine(unsigned int line);
   void setTitle(const QString& title);
   QTextStream& out();
   void display(Type type = Type::Plain);
private:
   void displayPlain();
   QString _function;
   QString _file;
   unsigned int _line;
   QString _title;
   QString _description;
   QTextStream* _stream {nullptr};
};



#endif
