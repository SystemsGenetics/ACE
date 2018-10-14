#include "elog.h"
#include <QString>
#include <QDebug>
#include "ace_logserver.h"
#include "ace_settings.h"



//






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(qint8 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(qint16 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(qint32 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(qint64 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(quint8 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(quint16 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(quint32 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(quint64 value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(float value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(double value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(QString::number(value).toUtf8());
   return *this;
}






/*!
 * Adds the given value to this log message. 
 *
 * @param value The value that is written out. 
 *
 * @return Reference to this logger. 
 */
ELog& ELog::operator<<(const QString& value)
{
   // Append the given value to this log's message byte array and return a reference 
   // of this log entry. 
   _message.append(value.toUtf8());
   return *this;
}






/*!
 * Tests if logging is active for this application, returning true if it is. 
 *
 * @return True if the logging system is active or false otherwise. 
 */
bool ELog::isActive()
{
   return Ace::Settings::instance().loggingEnabled();
}






/*!
 * Writes the fully constructed message to the logging system of this application. 
 * Uses a mutex to safely write to the logging system inside any thread. If logging 
 * is not active this will do nothing. If logging is disabled then this reports the 
 * misuse of logging and does nothing else. 
 */
ELog::~ELog()
{
   // Get the pointer to the global log server and make sure it is not null. 
   Ace::LogServer* log {Ace::LogServer::log()};
   if ( !log )
   {
      qDebug().noquote().nospace() << QObject::tr("Attempting to write a log message when logging is disabled!");
      return;
   }

   // Write this log's message to the log server and flush its output. 
   log->broadcast(Ace::LogServer::Log,-1,_message);
}
