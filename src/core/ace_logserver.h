#ifndef ACE_LOGSERVER_H
#define ACE_LOGSERVER_H
#include <QObject>
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
   class LogServer : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * Defines all message types that can be broadcast to clients. 
       */
      enum Type
      {
         /*!
          * Defines the log message type that is meant for normal logging messages. 
          */
         Log
         /*!
          * Defines the debug message type that is meant for all debugging log messages. 
          */
         ,Debug
      };
   public:
      static void initialize(int port);
      static LogServer* log();
      static QString host();
   public:
      void wait();
      LogServer& broadcast(Type type, int thread, const QByteArray& data);
   private:
      class Thread;
   private:
      /*!
       * Pointer to the global instance of this application's log server. 
       */
      static LogServer* _log;
   private:
      LogServer(int port);
      /*!
       */
      Thread* _server;
   };
}



#endif
