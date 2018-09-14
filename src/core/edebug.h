#ifndef EDEBUG_H
#define EDEBUG_H
#include <QObject>
#include <QAtomicInteger>
#include <QVector>
#include <QTextStream>
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
 */
class EDebug
{
public:
   /*!
    */
   enum Token
   {
      /*!
       */
      Quote
      /*!
       */
      ,NoQuote
   };
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
   EDebug& operator<<(const QString& value);
   EDebug& operator<<(const QStringList& value);
   EDebug& operator<<(const QObject*const value);
   EDebug& operator<<(const QVariant& value);
   EDebug& operator<<(const EMetadata& value);
   EDebug& operator<<(const EAbstractAnalytic::Block*const value);
   EDebug& operator<<(EAbstractAnalytic::Input::Type value);
   EDebug& operator<<(EAbstractAnalytic::Input::Role value);
   EDebug& operator<<(Ace::Analytic::AbstractMPI::Type value);
public:
   static int threadId();
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
   void setArgument(int depth);
   void setupArguments(const char* argumentNames);
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
   QTextStream out(stdout);
   for (int i = 0; i < _argumentIndexes.size() ;++i)
   {
      out << "[" << _threadId << "]" << QString(_stackDepth*3,' ') << &_argumentNames[_argumentIndexes[i]] << " = " << _argumentValues[i] << "\n";
   }
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
