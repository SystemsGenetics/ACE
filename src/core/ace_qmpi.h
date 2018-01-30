#ifndef ACE_MPI_H
#define ACE_MPI_H
#include <QObject>



namespace Ace
{
   class QMPI : public QObject
   {
      Q_OBJECT
   public:
      static QMPI& instance();
      int size() const;
      int rank() const;
      bool isMaster() const;
      bool isSlave() const;
   signals:
      void dataReceived(QByteArray data, int fromRank);
   public slots:
      void sendData(int toRank, const QByteArray data);
   private:
      virtual void timerEvent(QTimerEvent* event) override final;
      explicit QMPI(QObject* parent = nullptr);
      int _size;
      int _rank;
      static QMPI* _instance;
   };
}



#endif
