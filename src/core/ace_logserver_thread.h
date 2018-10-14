#ifndef ACE_LOGSERVER_THREAD_H
#define ACE_LOGSERVER_THREAD_H
#include <QThread>
#include <QMutex>
#include "ace_logserver.h"
//



namespace Ace
{
   /*!
    */
   class LogServer::Thread : public QThread
   {
   public:
      Thread(int port, QObject* parent = nullptr);
      void wait();
      void broadcast(const QByteArray& data);
   private:
      virtual void run() override final;
   private:
      /*!
       */
      int _port;
      /*!
       */
      QAtomicInteger<int> _ready {0};
      /*!
       */
      QMutex _lock;
      /*!
       */
      QAtomicInteger<int> _switch {0};
      /*!
       */
      const QByteArray* _data;
   };
}



#endif
