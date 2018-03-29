#ifndef ACE_QMPI_H
#define ACE_QMPI_H
#include <QObject>
//



namespace Ace
{
   /*!
    * This is a wrapper to the MPI interface. It provides support for basic MPI 
    * functionality including initializing MPI, getting rank and size, and sending and 
    * receiving data. This provides sending and receiving of data by using Qt slots 
    * and signals to allow for event handling through Qt's event handling system. This 
    * is a singleton class which only has a single instance you can access through a 
    * special static function. This singleton class is however special in it has to be 
    * shutdown before exiting the program and cannot be initialized more than once. 
    * This requirement is due to how MPI works. 
    */
   class QMPI : public QObject
   {
      Q_OBJECT
   public:
      static QMPI& initialize();
      static void shutdown();
      int size() const;
      int rank() const;
      int localRank() const;
      bool isMaster() const;
   signals:
      /*!
       * This is emitted whenever new data is received by another MPI process. 
       *
       * @param data Stores the raw data received from another process. 
       *
       * @param fromRank The rank of the process that sent this data. 
       */
      void dataReceived(const QByteArray& data, int fromRank);
   public slots:
      void sendData(int toRank, const QByteArray& data);
   protected:
      virtual void timerEvent(QTimerEvent* event) override final;
   private:
      explicit QMPI();
      ~QMPI();
      /*!
       * This is the total size of the MPI run representing the total number of processes 
       * part of it. 
       */
      int _size;
      /*!
       * This is the rank of this process that identifies it between all other processes 
       * part of the MPI run. 
       */
      int _rank;
      /*!
       * This is the local rank of this process identifying among all processes that 
       * share resources. 
       */
      int _localRank;
      /*!
       * This is the period, in milliseconds, between each time this class checks for new 
       * data received from other MPI processes. 
       */
      constexpr static int _timerPeriod {50};
      /*!
       * Keeps track if an instance of this class has already been created then destroyed 
       * in shutdown. This is so a second instance is never made after the MPI system was 
       * shutdown with finalize. 
       */
      static bool _hasShutdown;
      /*!
       * This is a pointer to the single instance of this class. 
       */
      static QMPI* _instance;
   };
}



#endif
