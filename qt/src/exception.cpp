#include <QMessageBox>

#include "exception.h"






void Exception::setFunction(const QString& function)
{
   _function = function;
}






void Exception::setFile(const QString& file)
{
   _file = file;
}






void Exception::setLine(unsigned int line)
{
   _line = line;
}






void Exception::setTitle(const QString& title)
{
   _title = title;
}






QTextStream &Exception::out()
{
   if ( !_stream )
   {
      _stream = new QTextStream(&_description);
   }
   return *_stream;
}






void Exception::display(Exception::Type type)
{
   switch (type)
   {
   case Type::Plain:
   case Type::Data:
   case Type::Analytic:
      displayPlain();
      break;
   }
}






void Exception::displayPlain()
{
   // generate the message box's textual information
   QString message;
   QTextStream stream(&message);
   _function.replace(" ","&nbsp;");
   stream << "<h3>" << _title << "</h3>";
   stream << "<p>" << QObject::tr("An unexpected error has occured.") << "</p>";
   stream << "<p><b>" << QObject::tr("File:") << "</b> " << _file << "<br/>";
   stream << "<b>" << QObject::tr("Function:") << "</b>&nbsp;" << _function << "<br/>";
   stream << "<b>" << QObject::tr("Line:") << "</b> " << _line << "</p>";

   // create the message box, set all exception information for the user, and modally display the
   // dialog
   QMessageBox critical;
   critical.setWindowTitle(QObject::tr("Critical Error"));
   critical.setIcon(QMessageBox::Critical);
   critical.setText(message);
   critical.setDetailedText(_description);
   critical.exec();
}
