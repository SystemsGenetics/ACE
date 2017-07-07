#include "exception.h"






void EException::setFunction(const QString& function) noexcept
{
   _function = function;
}






void EException::setFile(const QString& file) noexcept
{
   _file = file;
}






void EException::setLine(unsigned int line) noexcept
{
   _line = line;
}






void EException::setTitle(const QString& title) noexcept
{
   _title = title;
}






void EException::setDetails(const QString &details) noexcept
{
   _details = details;
}






void EException::setType(int type) noexcept
{
   _type = type;
}






void EException::setLevel(EException::Level level) noexcept
{
   _level = level;
}






QString EException::getFunction() const noexcept
{
   return _function;
}






QString EException::getFile() const noexcept
{
   return _file;
}






int EException::getLine() const noexcept
{
   return _line;
}






QString EException::getTitle() const noexcept
{
   return _title;
}






QString EException::getDetails() const noexcept
{
   return _details;
}






int EException::getType() const noexcept
{
   return _type;
}






EException::Level EException::getLevel() const noexcept
{
   return _level;
}






/*
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
*/






bool ESilent::hasException() const noexcept
{
   return _exception.get() != nullptr;
}






EException ESilent::getException() const noexcept
{
   if ( !_exception )
   {
      return EException();
   }
   return *_exception;
}






void ESilent::setException(EException exception) noexcept
{
   _exception.reset(new EException(exception));
   if ( !_critical && exception.getLevel() == EException::Critical )
   {
      _critical = true;
   }
}






ESilent::operator bool() const noexcept
{
   return !_critical;
}
