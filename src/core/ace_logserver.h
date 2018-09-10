#ifndef ACE_LOGSERVER_H
#define ACE_LOGSERVER_H
#include <QTcpServer>
#include <QMutex>
//



namespace Ace
{
   /*!
    * This is a TCP logging server used to output logging messages to all connected 
    * clients. This is a very low level class and only outputs byte arrays. This class 
    * is thread safe, using an internal mutex for all thread critical situations. This 
    * class acts as a TCP server, relaying all byte arrays streamed to it. This also 
    * provides static functions for initializing the log server and then accessing its 
    * instance. 
    */
   class LogServer : public QTcpServer
   {
      Q_OBJECT
   public:
      /*!
       * Defines all message types that can be broadcast to clients. 
       */
      enum Types
      {
         /*!
          * Defines the log message type that is meant for normal logging messages. 
          */
         Log
      };
      LogServer& broadcast(int type, const QByteArray& data);
   public:
      static void initialize(int port);
      static LogServer* log();
      static QString host();
      void flush();
   signals:
      /*!
       * Signals that the this ACE program is ready to begin execution. 
       */
      void readyToStart();
   private slots:
      void clientSignalsStart();
      void newConnectionMade();
   private:
      explicit LogServer(int port);
      /*!
       * Pointer to the global instance of this application's log server. 
       */
      static LogServer* _log;
      /*!
       * List of pointers to the sockets of connected clients to this log server. 
       */
      QVector<QTcpSocket*> _clients;
      /*!
       * True if the first client already connected to this log server or false 
       * otherwise. 
       */
      bool _first {false};
      /*!
       * This log server's internal mutex used for locking thread sensitive code. 
       */
      QMutex _mutex;
   };
}



#endif
