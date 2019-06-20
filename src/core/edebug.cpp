#include "edebug.h"
#include <string.h>
#include <QTextStream>
#include "emetadata.h"
#include "eexception.h"
#include "ace_logserver.h"



/*!
 * Used to keep track of how many threads have been started and so each new
 * thread can then use this value as their ID, incrementing it at the same time
 * atomically.
 */
QAtomicInteger<int> EDebug::_counter;
/*!
 * The thread ID for this local thread.
 */
thread_local int EDebug::_threadId {-1};
/*!
 * The current stack depth for this thread, used to determine the number of
 * spaces to use when making output.
 */
thread_local int EDebug::_stackDepth {0};
/*!
 * True if the set arguments template functions are being called or false
 * otherwise. Used as a safety mechanism to prevent infinite recursion.
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
 * @param token The token that is used to modify a property of this debug
 *              object's stream.
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
 * Writes a floating point value to this debug thread's temporary stream object
 * as a string.
 *
 * @param value The value written as a string.
 *
 * @return Reference to this debug object.
 */
EDebug& EDebug::operator<<(float value)
{
   // Write the floating point value as a string to this object's holder byte array.
   _holder += QString::number(static_cast<double>(value)).toLocal8Bit();

   // Return a reference to this object.
   return *this;
}






/*!
 * Writes a floating point value to this debug thread's temporary stream object
 * as a string.
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
 * Writes a string to this debug thread's temporary stream object. Quotes are
 * added if that property is active.
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
   _holder += QStringLiteral("0x") + QString::number(reinterpret_cast<quint64>(value),16).toLocal8Bit();

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
 * Writes a list of Qt strings to this debug thread's temporary stream object as
 * a single string.
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
 * Writes a Qt object pointer to this debug thread's temporary stream object as
 * a string.
 *
 * @param value The value written as a string.
 *
 * @return Reference to this debug object.
 */
EDebug& EDebug::operator<<(const QObject*const value)
{
   // Write out the pointer to this object's holder byte array.
   *this << static_cast<const void*>(value);

   // If the given pointer is not null then write out the class name of the qt
   // object.
   if ( value )
   {
      *this << NoQuote
            << QStringLiteral("(")
            << value->metaObject()->className()
            << QStringLiteral(")")
            << Quote;
   }

   // Return a reference to this object.
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
   // Write out the given qt variant value as a string.
   *this << value.toString();

   // Return a reference to this object.
   return *this;
}






/*!
 * Writes an abstract analytic input type enumeration value to this debug
 * thread's temporary stream object as a string.
 *
 * @param value The value written as a string.
 *
 * @return Reference to this debug object.
 */
EDebug& EDebug::operator<<(EAbstractAnalyticInput::Type value)
{
   // Write out the given enumeration value as a string to this object's holder byte
   // array.
   switch (value)
   {
   case EAbstractAnalyticInput::Boolean:
      *this << NoQuote << QStringLiteral("Boolean") << Quote;
      break;
   case EAbstractAnalyticInput::Integer:
      *this << NoQuote << QStringLiteral("Integer") << Quote;
      break;
   case EAbstractAnalyticInput::Double:
      *this << NoQuote << QStringLiteral("Double") << Quote;
      break;
   case EAbstractAnalyticInput::String:
      *this << NoQuote << QStringLiteral("String") << Quote;
      break;
   case EAbstractAnalyticInput::Selection:
      *this << NoQuote << QStringLiteral("Selection") << Quote;
      break;
   case EAbstractAnalyticInput::FileIn:
      *this << NoQuote << QStringLiteral("FileIn") << Quote;
      break;
   case EAbstractAnalyticInput::FileOut:
      *this << NoQuote << QStringLiteral("FileOut") << Quote;
      break;
   case EAbstractAnalyticInput::DataIn:
      *this << NoQuote << QStringLiteral("DataIn") << Quote;
      break;
   case EAbstractAnalyticInput::DataOut:
      *this << NoQuote << QStringLiteral("DataOut") << Quote;
      break;
   }

   // Return a reference to this object.
   return *this;
}






/*!
 * Writes an abstract analytic input role enumeration value to this debug
 * thread's temporary stream object as a string.
 *
 * @param value The value written as a string.
 *
 * @return Reference to this debug object.
 */
EDebug& EDebug::operator<<(EAbstractAnalyticInput::Role value)
{
   // Write out the given enumeration value as a string to this object's holder byte
   // array.
   switch (value)
   {
   case EAbstractAnalyticInput::CommandLineName:
      *this << NoQuote << QStringLiteral("CommandLineName") << Quote;
      break;
   case EAbstractAnalyticInput::Title:
      *this << NoQuote << QStringLiteral("Title") << Quote;
      break;
   case EAbstractAnalyticInput::WhatsThis:
      *this << NoQuote << QStringLiteral("WhatsThis") << Quote;
      break;
   case EAbstractAnalyticInput::Default:
      *this << NoQuote << QStringLiteral("Default") << Quote;
      break;
   case EAbstractAnalyticInput::Minimum:
      *this << NoQuote << QStringLiteral("Minimum") << Quote;
      break;
   case EAbstractAnalyticInput::Maximum:
      *this << NoQuote << QStringLiteral("Maximum") << Quote;
      break;
   case EAbstractAnalyticInput::Decimals:
      *this << NoQuote << QStringLiteral("Decimals") << Quote;
      break;
   case EAbstractAnalyticInput::SelectionValues:
      *this << NoQuote << QStringLiteral("SelectionValues") << Quote;
      break;
   case EAbstractAnalyticInput::FileFilters:
      *this << NoQuote << QStringLiteral("FileFilters") << Quote;
      break;
   case EAbstractAnalyticInput::DataType:
      *this << NoQuote << QStringLiteral("DataType") << Quote;
      break;
   }

   // Return a reference to this object.
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
   // Write out the given enumeration value as a string to this object's holder byte
   // array.
   switch (value)
   {
   case Ace::Analytic::AbstractMPI::Serial:
      *this << NoQuote << QStringLiteral("Serial") << Quote;
      break;
   case Ace::Analytic::AbstractMPI::OpenCL:
      *this << NoQuote << QStringLiteral("OpenCL") << Quote;
      break;
   case Ace::Analytic::AbstractMPI::CUDA:
      *this << NoQuote << QStringLiteral("CUDA") << Quote;
      break;
   }

   // Return a reference to this object.
   return *this;
}






/*!
 * Constructs a new debug object with the given function name and list of
 * argument names. The argument names are expected to be separated by commas.
 *
 * @param function The name of the function that is being entered and this new
 *                 debug object is responsible for tracking.
 *
 * @param argumentNames The list of argument names for this new debug object's
 *                      tracked function. The names must be separated by commas.
 */
EDebug::EDebug(const char* function, const char* argumentNames)
   :
   _function(function)
{
   // If the thread ID has not been initialized then do so.
   if ( _threadId == -1 )
   {
      _threadId = _counter++;
   }

   // Make sure another debug object is not currently active setting up its arguments
   // to avoid an infinite loop.
   if ( _active )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Logic Error"));
      e.setDetails(QObject::tr("Cannot DEBUG TRACE methods used inside a EDebug operator."));
      throw e;
   }

   // Setup the argument names for this debug object.
   setupArguments(argumentNames);

   // Output the opening function string to the logging server if it is enabled.
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

   // Increment the stack depth for the new function.
   ++_stackDepth;
}






/*!
 * Cleans up a debug object that has fallen out of scope of its tracking
 * function along with reporting the function has been left.
 */
EDebug::~EDebug()
{
   // Delete all argument name strings.
   delete[] _argumentNames;

   // Decrement the stack depth, clamping it to 0 if it goes below that value.
   --_stackDepth;
   if ( _stackDepth < 0 )
   {
      _stackDepth = 0;
   }

   // Output the closing function string to the logging server if it is enabled.
   QByteArray message(_stackDepth*3,' ');
   message.append("}");
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( log )
   {
      log->broadcast(Ace::LogServer::Debug,_threadId,message);
   }
}






/*!
 * This dummy function is for set argument calls that pass zero argument values.
 *
 * @param depth Unused depth value.
 */
void EDebug::setArgument(int depth)
{
   Q_UNUSED(depth)
}






/*!
 * Creates the list of argument names for this debug object with the given raw
 * string containing all of them.
 *
 * @param argumentNames String containing all argument names that is parsed.
 */
void EDebug::setupArguments(const char* argumentNames)
{
   // Get the size of the input string, including the null terminator.
   size_t size {strlen(argumentNames) + 1};

   // Create a new string of the same size for this object and copy over the given
   // argument string. This is done because the copied string is modified by
   // replacing each comma character with a null terminator. That along with a
   // pointer to each smaller string created within the larger one allows using a
   // single large string to contain all indexes argument names.
   _argumentNames = new char[size];
   memcpy(_argumentNames,argumentNames,size);

   // Iterate through each character of the copied argument list string.
   size_t last {0};
   for (size_t i = 0; i < size ;++i)
   {
      // Check to see if this character is a comma or the final null terminator string.
      if ( argumentNames[i] == ',' || argumentNames[i] == '\0' )
      {
         // Set the character to a null terminator, save the index of this argument name,
         // and then set the next index value.
         _argumentNames[i] = '\0';
         _argumentIndexes << static_cast<int>(last);
         last = i + 1;
         Q_ASSERT(last < std::numeric_limits<int>::max());
      }
   }

   // Resize the argument value list to the size of the argument names index list.
   _argumentValues.resize(_argumentIndexes.size());
}






/*!
 * Output all argument names and values of this debug object to the logging
 * server if it is enabled.
 */
void EDebug::dumpArguments()
{
   // Initialize the message byte array.
   QByteArray message;

   // Iterate through all argument indexes.
   for (int i = 0; i < _argumentIndexes.size() ;++i)
   {
      // Append the argument name and value to the message byte array.
      message.append(QByteArray(_stackDepth*3,' '))
             .append(&_argumentNames[_argumentIndexes[i]])
             .append(" = ")
             .append(_argumentValues[i]);

      // If this is not the last argument index append a new line to the message.
      if ( i < (_argumentIndexes.size() - 1) )
      {
         message.append("\n");
      }
   }

   // If logging is enabled output the argument names and values message to the
   // logging server.
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( log )
   {
      log->broadcast(Ace::LogServer::Debug,_threadId,message);
   }
}

