#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <QtCore>
#define E_MAKE_EXCEPTION(e) EException e;e.setFile(__FILE__);e.setFunction(__PRETTY_FUNCTION__);\
   e.setLine(__LINE__);



class EException
{
public:
   void setFunction(const QString& function);
   void setFile(const QString& file);
   void setLine(unsigned int line);
   void setTitle(const QString& title);
   void setDetails(const QString& details);
   QString getFunction() const;
   QString getFile() const;
   int getLine() const;
   QString getTitle() const;
   QString getDetails() const;
private:
   QString _function;
   QString _file;
   unsigned int _line;
   QString _title;
   QString _description;
   QString _details;
};



#endif
