#ifndef ACE_QMPI_H
#define ACE_QMPI_H
#include <QObject>
#include <QVector>
#include <mpi.h>
//



namespace Ace
{
   /*!
    * This is a wrapper to the MPI interface. It provides support for basic MPI 
    * functionality including initializing MPI, getting rank and size for world and 
    * local, and sending and receiving data for world and local. This provides sending 
    * and receiving of data by using Qt slots and signals to allow for event handling 
    * through Qt's event handling system. This is a singleton class which only has a 
    * single instance you can access through a special static function. This singleton 
    * class is however special in it has to be shutdown before exiting the program and 
    * cannot be initialized more than once. This requirement is due to how MPI works. 
    * 
    * Listening for incoming MPI messages can also be enabled or disabled, and is 
    * disabled by default. 
    */
   class QMPI : public QObject
   {
      Q_OBJECT
   public:
      static QMPI& instance();
      static void shutdown();
      bool isMaster() const;
      int size() const;
      int rank() const;
      int localSize() const;
      int localRank() const;
      void start();
      void stop();
   signals:
      /*!
       * Signals that new data has been received from the world MPI comm. 
       *
       * @param data The data received from the world comm. 
       *
       * @param fromRank The rank of the node that sent this data. 
       */
      void dataReceived(const QByteArray& data, int fromRank);
      /*!
       * Signals that new data has been received from the local shared resource MPI comm. 
       *
       * @param data The data received from the local comm. 
       *
       * @param fromRank The local rank of the node that sent this data. 
       */
      void localDataReceived(const QByteArray& data, int fromRank);
   public slots:
      void sendData(int toRank, const QByteArray& data);
      void sendLocalData(int toRank, const QByteArray& data);
   protected:
      virtual void timerEvent(QTimerEvent* event) override final;
   private:
      explicit QMPI();
      ~QMPI();
      void probe(MPI_Comm comm, int rank);
      void sendData(MPI_Comm comm, int toRank, const QByteArray& data);
      void setupWorld();
      void setupLocal();
      /*!
       * The period, in milliseconds, between each time this class checks for new data 
       * received from the MPI system. 
       */
      constexpr static int _timerPeriod {50};
      /*!
       * True if an instance of this class has already been deleted or false otherwise. 
       * This is used to make sure a second class is never deleted. 
       */
      static bool _hasShutdown;
      /*!
       * Pointer to the singleton instance of this class. 
       */
      static QMPI* _instance;
      /*!
       * The total number of nodes of the MPI run. 
       */
      int _size;
      /*!
       * The rank of this node that identifies it within the world MPI comm. 
       */
      int _rank;
      /*!
       * The local MPI comm used for all nodes that share local resources with this node. 
       */
      MPI_Comm _local;
      /*!
       * The number of nodes that share local resources. 
       */
      int _localSize;
      /*!
       * The local rank of this node that identifies if within the local MPI comm. 
       */
      int _localRank;
      /*!
       * True if initialization of MPI failed and was set to a default null state or 
       * false otherwise. 
       */
      bool _failed {false};
      /*!
       * True to ignore all incoming MPI messages or false to read them in and emit 
       * signals. 
       */
      bool _ignore {true};
   };
}



#endif
