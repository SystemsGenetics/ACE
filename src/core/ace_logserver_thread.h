#ifndef ACE_LOGSERVER_THREAD_H
#define ACE_LOGSERVER_THREAD_H
#include <QThread>
#include <QMutex>
#include "ace_logserver.h"
//



namespace Ace
{
   /*!
    * This is the internal TCP server that is used by the log server. This is a qt 
    * thread that runs a TCP server inside its own thread since a Qt server and its 
    * sockets must all run on the same thread. To communicate with this thread a mutex 
    * and atomic integer switches are used. WARNING: the TCP server is not initialized 
    * and started until the thread is started. 
    */
   class LogServer::Thread : public QThread
   {
   public:
      Thread(int port, QObject* parent = nullptr);
      void wait();
      void broadcast(const QByteArray& data);
   protected:
      virtual void run() override final;
   private:
      /*!
       * The port number this thread's internal TCP server listens for new connections. 
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
}



#endif
