#ifndef EDEBUG_H
#define EDEBUG_H
#include <QObject>
#include <QAtomicInteger>
#include <QVector>
#include "eabstractanalytic.h"
#include "eabstractanalytic_input.h"
#include "ace_analytic_abstractmpi.h"
#ifdef QT_DEBUG
#define EDEBUG_FUNC(...) EDebug debug(__PRETTY_FUNCTION__,#__VA_ARGS__); debug.setArguments(__VA_ARGS__);
#else
#define EDEBUG_FUNC(...)
#endif
//



/*!
 * This is the debug class which is used at the beginning of each function to track 
 * every function call for debugging. This class should NEVER be used directly, 
 * instead using the macro provided in its header file that declares an instance of 
 * this class with the given function variables to report. This class is aware of 
 * threads, correctly reporting what thread each debug message is on so the correct 
 * stack debug flow of function calls is presented for each separate thread. A 
 * unique ID is given to each new thread. 
 * 
 * This class also acts as a stream operator for any variable type that is passed 
 * to it from the list of arguments for a function call. Only literal types should 
 * be copied. Any other abstract or object type MUST be passed by pointer. The 
 * stream's properties can be modified by streaming special tokens to it. 
 * 
 * The stream operators are not used in a classical sense of a streaming object, 
 * since this debug object is not a basic IO stream. Instead the stream operators 
 * construct the values of any passed function arguments that it then uses for 
 * debug output. Because of the temporary nature each variable is streamed into the 
 * same string, overwritten each time a new variable is streamed into it. 
 */
class EDebug
{
public:
   /*!
    * Defines special tokens for setting different properties of this debug object's 
    * stream. 
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
   EDebug& operator<<(const EAbstractAnalytic::Block*const value);
   EDebug& operator<<(EAbstractAnalytic::Input::Type value);
   EDebug& operator<<(EAbstractAnalytic::Input::Role value);
   EDebug& operator<<(Ace::Analytic::AbstractMPI::Type value);
public:
   EDebug(const char* function, const char* argumentNames);
   ~EDebug();
   template<class... Args> void setArguments(Args... arguments);
protected:
   template<class T> void setArgument(int depth, T last);
   template<class T,class... Args> void setArgument(int depth, T first, Args... arguments);
private:
   /*!
    */
   static QAtomicInteger<int> _counter;
   /*!
    */
   static thread_local int _threadId;
   /*!
    */
   static thread_local int _stackDepth;
   /*!
    */
   static thread_local bool _active;
private:
   void setArgument(int depth);
   void setupArguments(const char* argumentNames);
   void dumpArguments();
   /*!
    */
   const char* _function;
   /*!
    */
   char* _argumentNames {nullptr};
   /*!
    */
   QVector<int> _argumentIndexes;
   /*!
    */
   QVector<QByteArray> _argumentValues;
   /*!
    */
   QByteArray _holder;
   /*!
    */
   bool _quote {true};
};






/*!
 *
 * @tparam Args  
 *
 * @param arguments  
 */
template<class... Args> void EDebug::setArguments(Args... arguments)
{
   if ( _active )
   {
      return;
   }
   _active = true;
   setArgument(0,arguments...);
   _active = false;
}






/*!
 *
 * @tparam T  
 *
 * @param depth  
 *
 * @param last  
 */
template<class T> void EDebug::setArgument(int depth, T last)
{
   _quote = true;
   _holder.clear();
   *this << last;
   _argumentValues[depth] = _holder;
   dumpArguments();
}






/*!
 *
 * @tparam T  
 *
 * @tparam Args  
 *
 * @param depth  
 *
 * @param first  
 *
 * @param arguments  
 */
template<class T,class... Args> void EDebug::setArgument(int depth, T first, Args... arguments)
{
   _quote = true;
   _holder.clear();
   *this << first;
   _argumentValues[depth] = _holder;
   setArgument(depth + 1,arguments...);
}



#endif
