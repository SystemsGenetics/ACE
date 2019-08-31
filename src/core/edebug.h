#ifndef EDEBUG_H
#define EDEBUG_H
#include <QObject>
#include <QAtomicInteger>
#include <QVector>
#include "eabstractanalytic.h"
#include "ace_analytic_abstractmpi.h"
#ifdef QT_DEBUG
#ifdef EDEBUG
#define EDEBUG_FUNC(...) EDebug debug(__PRETTY_FUNCTION__,#__VA_ARGS__); debug.setArguments(__VA_ARGS__);
#else
#define EDEBUG_FUNC(...)
#endif
#else
#define EDEBUG_FUNC(...)
#endif



/*!
 * This is the debug class which is used at the beginning of each function to
 * track every function call for debugging. This class should NEVER be used
 * directly, instead using the macro provided in its header file that declares
 * an instance of this class with the given function variables to report. This
 * class is aware of threads, correctly reporting what thread each debug message
 * is on so the correct stack debug flow of function calls is presented for each
 * separate thread. A unique ID is given to each new thread.
 *
 * This class also acts as a stream operator for any variable type that is
 * passed to it from the list of arguments for a function call. Only literal
 * types should be copied. Any other abstract or object type MUST be passed by
 * pointer. The stream's properties can be modified by streaming special tokens
 * to it.
 *
 * The stream operators are not used in a classical sense of a streaming object,
 * since this debug object is not a basic IO stream. Instead the stream
 * operators construct the values of any passed function arguments that it then
 * uses for debug output. Because of the temporary nature each variable is
 * streamed into the same string, overwritten each time a new variable is
 * streamed into it.
 */
class EDebug
{
public:
   /*!
    * Defines special tokens for setting different properties of this debug
    * object's stream.
    */
   enum Token
   {
      /*!
       * Sets the quote property active which will add quotes to any input lines
       * following this token. This is the default behavior.
       */
      Quote
      /*!
       * Sets the quote property inactive appending any input strings as is following
       * this token.
       */
      ,NoQuote
   };
public:
   static int threadId();
public:
   EDebug& operator<<(Token token);
   EDebug& operator<<(bool value);
   EDebug& operator<<(qint8 value);
   EDebug& operator<<(qint16 value);
   EDebug& operator<<(qint32 value);
   EDebug& operator<<(qint64 value);
   EDebug& operator<<(quint8 value);
   EDebug& operator<<(quint16 value);
   EDebug& operator<<(quint32 value);
   EDebug& operator<<(quint64 value);
   EDebug& operator<<(float value);
   EDebug& operator<<(double value);
   EDebug& operator<<(const char* value);
   EDebug& operator<<(const void*const value);
   EDebug& operator<<(const QByteArray& value);
   EDebug& operator<<(const QString& value);
   EDebug& operator<<(const QStringList& value);
   EDebug& operator<<(const QObject*const value);
   EDebug& operator<<(const QVariant& value);
   EDebug& operator<<(EAbstractAnalyticInput::Type value);
   EDebug& operator<<(EAbstractAnalyticInput::Role value);
   EDebug& operator<<(Ace::Analytic::AbstractMPI::Type value);
public:
   EDebug(const char* function, const char* argumentNames);
   ~EDebug();
   template<class... Args> void setArguments(Args... arguments);
protected:
   template<class T> void setArgument(int depth, T last);
   template<class T, class... Args> void setArgument(int depth, T next, Args... arguments);
private:
   /*!
    * Used to keep track of how many threads have been started and so each new
    * thread can then use this value as their ID, incrementing it at the same time
    * atomically.
    */
   static QAtomicInteger<int> _counter;
   /*!
    * The thread ID for this local thread.
    */
   static thread_local int _threadId;
   /*!
    * The current stack depth for this thread, used to determine the number of
    * spaces to use when making output.
    */
   static thread_local int _stackDepth;
   /*!
    * True if the set arguments template functions are being called or false
    * otherwise. Used as a safety mechanism to prevent infinite recursion.
    */
   static thread_local bool _active;
private:
   void setArgument(int depth);
   void setupArguments(const char* argumentNames);
   void dumpArguments();
   /*!
    * The function name this debug object is tracking.
    */
   const char* _function;
   /*!
    * The list of argument names of the function this debug object is tracking. The
    * list is separated by null terminators within the string.
    */
   QStringList _argumentNames;
   /*!
    * List of argument values of the function this debug object is tracking.
    */
   QVector<QByteArray> _argumentValues;
   /*!
    * Temporary holder byte array used to build the argument values of each
    * argument of the function this debug object is tracking. All streaming
    * operators of this class writes their given values to this temporary object
    * that is cleared for each new argument value.
    */
   QByteArray _holder;
   /*!
    * The quote property used for streaming operators. True causes quotes to be
    * added to strings or false to add nothing.
    */
   bool _quote {true};
};






/*!
 * Sets the value of the arguments of the function this debug object is
 * tracking. The number of arguments MUST match with the number of argument
 * names given in this object's constructor.
 *
 * @tparam Args The variadic list of argument values.
 *
 * @param arguments The variadic list of argument values.
 */
template<class... Args> void EDebug::setArguments(Args... arguments)
{
   // Signal this debug object is active, call the initial variadic template
   // function, and then disable the active flag.
   _active = true;
   setArgument(0,arguments...);
   _active = false;
}






/*!
 * Sets the final argument value for the variadic function chain.
 *
 * @tparam T The type of the last argument value.
 *
 * @param depth The depth of the last argument which is the nth argument value.
 *
 * @param last The last argument.
 */
template<class T> void EDebug::setArgument(int depth, T last)
{
   // Reset this object's holder byte array for streaming.
   _quote = true;
   _holder.clear();

   // Stream the given argument's value using this object's stream operators.
   *this << last;

   // Get the holder byte array string and set it as this object's tracked argument
   // value for the given depth.
   _argumentValues[depth] = _holder;

   // Dump all argument names and values to the logging server.
   dumpArguments();
}






/*!
 * Sets the next argument value for the variadic function chain.
 *
 * @tparam T The type of the next argument value.
 *
 * @tparam Args The variadic list of remaining argument values.
 *
 * @param depth The depth of the next argument which is the nth argument value.
 *
 * @param next The next argument value.
 *
 * @param arguments The variadic list of remaining argument values.
 */
template<class T, class... Args> void EDebug::setArgument(int depth, T next, Args... arguments)
{
   // Reset this object's holder byte array for streaming.
   _quote = true;
   _holder.clear();

   // Stream the given next argument's value using this object's stream operators.
   *this << next;

   // Get the holder byte array string and set it as this object's tracked argument
   // value for the given depth.
   _argumentValues[depth] = _holder;

   // Call the next variadic function in this chain with one less argument in the
   // list.
   setArgument(depth + 1,arguments...);
}

#endif
