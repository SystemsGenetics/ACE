#ifndef ELOG_H
#define ELOG_H
#include <QByteArray>



/*!
 * This is a temporary log object designed to make a single log message. This is
 * thread safe. When an object of this class is destroyed it writes its log
 * message to output. This is safe because the underlying message system will
 * not throw any exceptions when doing this.
 */
class ELog
{
public:
   static bool isActive();
public:
   ELog& operator<<(qint8 value);
   ELog& operator<<(qint16 value);
   ELog& operator<<(qint32 value);
   ELog& operator<<(qint64 value);
   ELog& operator<<(quint8 value);
   ELog& operator<<(quint16 value);
   ELog& operator<<(quint32 value);
   ELog& operator<<(quint64 value);
   ELog& operator<<(float value);
   ELog& operator<<(double value);
   ELog& operator<<(const QString& value);
public:
   ~ELog();
private:
   /*!
    * The message being constructed by this log message and written to the logging
    * system upon its destruction.
    */
   QByteArray _message;
};

#endif
