#include "edebug.h"
#include <string.h>



//



/*!
 */
QAtomicInteger<int> EDebug::_counter;
/*!
 */
int thread_local EDebug::_threadId {-1};
/*!
 */
int thread_local EDebug::_stackDepth {0};






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(bool value)
{
   _holder += value ? QStringLiteral("TRUE") : QStringLiteral("FALSE");
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(qint8 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(qint16 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(qint32 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(qint64 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(quint8 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(quint16 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(quint32 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(quint64 value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(float value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(double value)
{
   _holder += QString::number(value).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(const void*const value)
{
   _holder += QStringLiteral("0x") + QString::number((quint64)value,16).toLocal8Bit();
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(const QObject*const value)
{
   _holder += QStringLiteral("0x")
              + QString::number((quint64)value,16).toLocal8Bit()
              + QStringLiteral("(")
              + value->metaObject()->className()
              + QStringLiteral(")");
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(const QString& value)
{
   _holder += QStringLiteral("\"") + value.toLocal8Bit() + QStringLiteral("\"");
   return *this;
}






/*!
 *
 * @param value  
 */
EDebug& EDebug::operator<<(const QStringList& value)
{
   bool first {true};
   for (auto string: value)
   {
      if ( !first )
      {
         _holder += QStringLiteral(",");
      }
      else
      {
         first = false;
      }
      _holder += QStringLiteral("\"") + string.toLocal8Bit() + QStringLiteral("\"");
   }
   return *this;
}






/*!
 */
int EDebug::threadId()
{
   return _threadId;
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
      _threadId = _counter.fetchAndAddAcquire(1);
   }
   setupArguments(argumentNames);
   QTextStream out(stdout);
   out << "[" << _threadId << "]" << QString(_stackDepth*3,' ') << function << "\n";
   out << "[" << _threadId << "]" << QString(_stackDepth*3,' ') << "{\n";
   ++_stackDepth;
}






/*!
 */
EDebug::~EDebug()
{
   delete[] _argumentNames;
   --_stackDepth;
   if ( _stackDepth < 0 )
   {
      _stackDepth = 0;
   }
   QTextStream out(stdout);
   out << "[" << _threadId << "]" << QString(_stackDepth*3,' ') << "}\n";
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
