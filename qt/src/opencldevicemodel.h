#ifndef ACE_OPENCLDEVICEMODEL_H
#define ACE_OPENCLDEVICEMODEL_H
#include <QtCore>
#include <CL/cl.h>
#include <memory>



namespace Ace
{
   class OpenCLDeviceModel : public QAbstractItemModel
   {
      Q_OBJECT
   public:
      explicit OpenCLDeviceModel(QObject* parent = 0);
      int columnCount(const QModelIndex& parent = QModelIndex()) const;
      QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
      QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
      QModelIndex parent(const QModelIndex &index) const;
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      QString getDetailedInfo(const QModelIndex& index) const;
      bool isDevice(const QModelIndex& index) const;
      void setDevice(const QModelIndex& index) const;
      void reset();
   private:
      using StringPointer = std::unique_ptr<char,std::default_delete<char[]>>;
      int getPlatformCount() const;
      cl_platform_id getPlatformID(int row) const;
      QString getPlatformName(int row) const;
      int getDeviceCount(int platformRow) const;
      cl_device_id getDeviceID(int platformRow, int row) const;
      QString getDeviceName(int platformRow, int row) const;
      StringPointer getPlatformInfo(cl_platform_id id, cl_platform_info what) const;
      template<class T> T* getDeviceInfo(cl_device_id id, cl_device_info what) const;
      QString getDetailedPlatformInfo(cl_platform_id id) const;
      QString getDetailedDeviceInfo(cl_device_id id) const;
   };
}



#endif
