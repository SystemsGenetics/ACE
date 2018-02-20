#ifndef ACE_QMPI_H
#define ACE_QMPI_H
#include <QObject>



namespace Ace
{
   /*!
    * This is a wrapper to the MPI interface. It provides support for basic MPI 
    * functionality including initializing MPI, getting rank and size, and sending 
    * and receiving data. This provides sending and receiving of data by using Qt 
    * slots and signals to allow for event handling through Qt's event handling 
    * system. This is a singleton class which only has a single instance you can 
    * access through a special static function. 
    */
   class QMPI : public QObject
   {
      Q_OBJECT
   public:
      static QMPI& instance();
      int size() const;
      int rank() const;
      bool isMaster() const;
   signals:
      /*!
       * This is signaled whenever new data is received by another MPI process. 
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
      /*!
       * This is the total size of the MPI run representing the total number of 
       * processes part of it. 
       */
      int _size;
      /*!
       * This is the rank of this process that identifies it between all other 
       * processes part of the MPI run. 
       */
      int _rank;
      /*!
       * This is the period, in milliseconds, between each time this class checks for 
       * new data received from other MPI processes. 
       */
      constexpr static int _timerPeriod {50};
      /*!
       * This is a pointer to the single instance of this class. 
       */
      static QMPI* _instance;
   };
}

#endif

