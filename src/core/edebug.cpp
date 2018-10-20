#include "edebug.h"
#include <string.h>
#include <QTextStream>
#include "eabstractanalytic_block.h"
#include "emetadata.h"
#include "eexception.h"
#include "ace_logserver.h"



//



/*!
 */
QAtomicInteger<int> EDebug::_counter;
/*!
 */
thread_local int EDebug::_threadId {-1};
/*!
 */
thread_local int EDebug::_stackDepth {0};
/*!
 */
thread_local bool EDebug::_active {false};






/*!
 * Returns the unique integer thread ID of the caller of this function. 
 *
 * @return Unique integer thread ID. 
 */
int EDebug::threadId()
{
   // If this thread has not gotten a unique ID yet then get one from the counter. 
   if ( _threadId == -1 )
   {
      _threadId = _counter++;
   }

   // Return this thread's ID. 
   return _threadId;
}






/*!
 * Modifies a property of this debug object's streaming properties. 
 *
 * @param token The token that is used to modify a property of this debug object's 
 *              stream. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(Token token)
{
   // Set the quote property depending on the given token. 
   switch(token)
   {
   case Quote:
      _quote = true;
      break;
   case NoQuote:
      _quote = false;
      break;
   }

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a boolean value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The boolean value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(bool value)
{
   // Write the boolean value as a string to this object's holder byte array. 
   _holder += value ? QStringLiteral("TRUE") : QStringLiteral("FALSE");

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(qint8 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(qint16 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(qint32 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(qint64 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(quint8 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(quint16 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(quint32 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes an integer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The integer value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(quint64 value)
{
   // Write the integer value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a floating point value to this debug thread's temporary stream object as 
 * a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(float value)
{
   // Write the floating point value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a floating point value to this debug thread's temporary stream object as 
 * a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(double value)
{
   // Write the floating point value as a string to this object's holder byte array. 
   _holder += QString::number(value).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a string to this debug thread's temporary stream object. Quotes are added 
 * if that property is active. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const char* value)
{
   // If quoting is active then add a beginning quote to this object's holder byte 
   // array. 
   if ( _quote )
   {
      _holder += QStringLiteral("\"");
   }

   // Append the given string to this object's holder byte array. 
   _holder += value;

   // If quoting is active then append an ending quote to this object's holder byte 
   // array. 
   if ( _quote )
   {
      _holder += QStringLiteral("\"");
   }

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a pointer value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const void*const value)
{
   // Append the given pointer to this object's holder byte array as a string. 
   _holder += QStringLiteral("0x") + QString::number((quint64)value,16).toLocal8Bit();

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a byte array value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const QByteArray& value)
{
   // Append the given byte array to this object's holder byte array as a string. 
   _holder += QStringLiteral("\"") + value.data() + QStringLiteral("\"");

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a Qt string value to this debug thread's temporary stream object. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const QString& value)
{
   // If quoting is active then add a beginning quote to this object's holder byte 
   // array. 
   if ( _quote )
   {
      _holder += QStringLiteral("\"");
   }

   // Append the given string to this object's holder byte array. 
   _holder += value.toLocal8Bit();

   // If quoting is active then append an ending quote to this object's holder byte 
   // array. 
   if ( _quote )
   {
      _holder += QStringLiteral("\"");
   }

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a list of Qt strings to this debug thread's temporary stream object as a 
 * single string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const QStringList& value)
{
   // Iterate through all strings in the given string list. 
   bool first {true};
   for (auto string: value)
   {
      // Append a comma if this is the first string in the list. 
      if ( !first )
      {
         _holder += QStringLiteral(",");
      }
      else
      {
         first = false;
      }

      // Append the next string in the list. 
      _holder += QStringLiteral("\"") + string.toLocal8Bit() + QStringLiteral("\"");
   }

   // Return a reference to this object. 
   return *this;
}






/*!
 * Writes a Qt object pointer to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const QObject*const value)
{
   *this << (void*)value;
   if ( value )
   {
      *this << NoQuote
            << QStringLiteral("(")
            << value->metaObject()->className()
            << QStringLiteral(")")
            << Quote;
   }
   return *this;
}






/*!
 * Writes a qt variant value to this debug thread's temporary stream object as a 
 * string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const QVariant& value)
{
   *this << value.toString();
   return *this;
}






/*!
 * Writes an abstract analytic block pointer to this debug thread's temporary 
 * stream object as a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(const EAbstractAnalytic::Block*const value)
{
   *this << (void*)value;
   if ( value )
   {
      *this << NoQuote << QStringLiteral("(EAbstractAnalytic::Block,index=")
            << QString::number(value->index())
            << QStringLiteral(")")
            << Quote;
   }
   return *this;
}






/*!
 * Writes an abstract analytic input type enumeration value to this debug thread's 
 * temporary stream object as a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(EAbstractAnalytic::Input::Type value)
{
   switch (value)
   {
   case EAbstractAnalytic::Input::Boolean:
      *this << NoQuote << QStringLiteral("Boolean") << Quote;
      break;
   case EAbstractAnalytic::Input::Integer:
      *this << NoQuote << QStringLiteral("Integer") << Quote;
      break;
   case EAbstractAnalytic::Input::Double:
      *this << NoQuote << QStringLiteral("Double") << Quote;
      break;
   case EAbstractAnalytic::Input::String:
      *this << NoQuote << QStringLiteral("String") << Quote;
      break;
   case EAbstractAnalytic::Input::Selection:
      *this << NoQuote << QStringLiteral("Selection") << Quote;
      break;
   case EAbstractAnalytic::Input::FileIn:
      *this << NoQuote << QStringLiteral("FileIn") << Quote;
      break;
   case EAbstractAnalytic::Input::FileOut:
      *this << NoQuote << QStringLiteral("FileOut") << Quote;
      break;
   case EAbstractAnalytic::Input::DataIn:
      *this << NoQuote << QStringLiteral("DataIn") << Quote;
      break;
   case EAbstractAnalytic::Input::DataOut:
      *this << NoQuote << QStringLiteral("DataOut") << Quote;
      break;
   }
   return *this;
}






/*!
 * Writes an abstract analytic input role enumeration value to this debug thread's 
 * temporary stream object as a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(EAbstractAnalytic::Input::Role value)
{
   switch (value)
   {
   case EAbstractAnalytic::Input::CommandLineName:
      *this << NoQuote << QStringLiteral("CommandLineName") << Quote;
      break;
   case EAbstractAnalytic::Input::Title:
      *this << NoQuote << QStringLiteral("Title") << Quote;
      break;
   case EAbstractAnalytic::Input::WhatsThis:
      *this << NoQuote << QStringLiteral("WhatsThis") << Quote;
      break;
   case EAbstractAnalytic::Input::Default:
      *this << NoQuote << QStringLiteral("Default") << Quote;
      break;
   case EAbstractAnalytic::Input::Minimum:
      *this << NoQuote << QStringLiteral("Minimum") << Quote;
      break;
   case EAbstractAnalytic::Input::Maximum:
      *this << NoQuote << QStringLiteral("Maximum") << Quote;
      break;
   case EAbstractAnalytic::Input::Decimals:
      *this << NoQuote << QStringLiteral("Decimals") << Quote;
      break;
   case EAbstractAnalytic::Input::SelectionValues:
      *this << NoQuote << QStringLiteral("SelectionValues") << Quote;
      break;
   case EAbstractAnalytic::Input::FileFilters:
      *this << NoQuote << QStringLiteral("FileFilters") << Quote;
      break;
   case EAbstractAnalytic::Input::DataType:
      *this << NoQuote << QStringLiteral("DataType") << Quote;
      break;
   }
   return *this;
}






/*!
 * Writes an ace analytic abstract MPI type enumeration value to this debug 
 * thread's temporary stream object as a string. 
 *
 * @param value The value written as a string. 
 *
 * @return Reference to this debug object. 
 */
EDebug& EDebug::operator<<(Ace::Analytic::AbstractMPI::Type value)
{
   switch (value)
   {
   case Ace::Analytic::AbstractMPI::Serial:
      *this << NoQuote << QStringLiteral("Serial") << Quote;
      break;
   case Ace::Analytic::AbstractMPI::OpenCL:
      *this << NoQuote << QStringLiteral("OpenCL") << Quote;
      break;
   }
   return *this;
}






/*!
 *
 * @param function  
 *
 * @param argumentNames  
 */
EDebug::EDebug(const char* function, const char* argumentNames):
   _function(function)
{
   if ( _threadId == -1 )
   {
      _threadId = _counter++;
   }
   if ( _active )
   {
      return;
   }
   setupArguments(argumentNames);
   QByteArray message(_stackDepth*3,' ');
   message.append(function)
          .append("\n")
          .append(QByteArray(_stackDepth*3,' '))
          .append("{");
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( log )
   {
      log->broadcast(Ace::LogServer::Debug,_threadId,message);
   }
   if ( _active )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Logic Error"));
      e.setDetails(QObject::tr("Cannot DEBUG TRACE methods used inside a EDebug operator."));
      throw e;
   }
   ++_stackDepth;
}






/*!
 */
EDebug::~EDebug()
{
   if ( _active )
   {
      return;
   }
   delete[] _argumentNames;
   --_stackDepth;
   if ( _stackDepth < 0 )
   {
      _stackDepth = 0;
   }
   QByteArray message(_stackDepth*3,' ');
   message.append("}");
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( log )
   {
      log->broadcast(Ace::LogServer::Debug,_threadId,message);
   }
}






/*!
 *
 * @param depth  
 */
void EDebug::setArgument(int depth)
{
   Q_UNUSED(depth)
}






/*!
 *
 * @param argumentNames  
 */
void EDebug::setupArguments(const char* argumentNames)
{
   size_t size {strlen(argumentNames) + 1};
   _argumentNames = new char[size];
   memcpy(_argumentNames,argumentNames,size);
   int last {0};
   for (size_t i = 0; i < size ;++i)
   {
      if ( argumentNames[i] == ',' || argumentNames[i] == '\0' )
      {
         _argumentNames[i] = '\0';
         _argumentIndexes << last;
         last = i + 1;
      }
      else
      {
         _argumentNames[i] = argumentNames[i];
      }
   }
   _argumentValues.resize(_argumentIndexes.size());
}






/*!
 */
void EDebug::dumpArguments()
{
   QByteArray message;
   for (int i = 0; i < _argumentIndexes.size() ;++i)
   {
      message.append(QByteArray(_stackDepth*3,' '))
             .append(&_argumentNames[_argumentIndexes[i]])
             .append(" = ")
             .append(_argumentValues[i]);
      if ( i < (_argumentIndexes.size() - 1) )
      {
         message.append("\n");
      }
   }
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( log )
   {
      log->broadcast(Ace::LogServer::Debug,_threadId,message);
   }
}
