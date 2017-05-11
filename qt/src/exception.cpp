#include <QMessageBox>

#include "exception.h"






void EException::setFunction(const QString& function)
{
   _function = function;
}






void EException::setFile(const QString& file)
{
   _file = file;
}






void EException::setLine(unsigned int line)
{
   _line = line;
}






void EException::setTitle(const QString& title)
{
   _title = title;
}






QTextStream& EException::out()
{
   if ( !_stream )
   {
      _stream = new QTextStream(&_description);
   }
   return *_stream;
}






void EException::display(Type type)
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






void EException::displayPlain()
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
