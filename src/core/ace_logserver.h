#ifndef ACE_LOGSERVER_H
#define ACE_LOGSERVER_H
#include <QThread>
#include <QMutex>



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This is a global singleton TCP logging server used to output logging messages
    * to all connected logging clients. This is a very low level class and only
    * outputs byte arrays. This class is thread safe, using an internal thread and
    * runs all TCP operations exclusively inside its own thread. This class acts as
    * a TCP server, relaying all byte arrays streamed to it. This also provides
    * static functions for initializing the log server and then accessing its
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
      /*!
       * This is the internal TCP server that is used by the log server. This is a qt
       * thread that runs a TCP server inside its own thread since a Qt server and its
       * sockets must all run on the same thread. To communicate with this thread a
       * mutex and atomic integer switches are used. WARNING: the TCP server is not
       * initialized and started until the thread is started.
       */
      class Thread : public QThread
      {
      public:
         Thread(int port, QObject* parent = nullptr);
         void wait();
         void broadcast(const QByteArray& data);
      protected:
         virtual void run() override final;
      private:
         /*!
          * The port number this thread's internal TCP server listens for new
          * connections.
          */
         int _port;
         /*!
          * Atomic switch that counts the number of clients that send a start signal.
          */
         QAtomicInteger<int> _ready {0};
         /*!
          * Lock mutex used for only allowing one thread at a time broadcast a message.
          */
         QMutex _lock;
         /*!
          * Switch used to signal that a data packet is ready to be broadcast at 1 and
          * switched back to 0 when it has been broadcast.
          */
         QAtomicInteger<int> _switch {0};
         /*!
          * Pointer to data message packet that will be broadcast once the switch atomic
          * integer is incremented.
          */
         const QByteArray* _data;
      };
   private:
      /*!
       * Pointer to the global instance of this application's log server.
       */
      static LogServer* _log;
   private:
      LogServer(int port);
      /*!
       * Pointer to the internal low level TCP server that runs on its own thread and
       * this class communicates with for broadcasting all messages.
       */
      Thread* _server;
   };
}

#endif
