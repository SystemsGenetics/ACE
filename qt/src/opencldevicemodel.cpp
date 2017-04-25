#include "opencldevicemodel.h"
#include "utilities.h"
#include "opencldevice.h"
#include "exception.h"






OpenCLDeviceModel::OpenCLDeviceModel(QObject* parent):
   QAbstractItemModel(parent)
{
}






int OpenCLDeviceModel::columnCount(const QModelIndex& /*parent*/) const
{
   return 1;
}






QVariant OpenCLDeviceModel::data(const QModelIndex &index, int role) const
{
   try
   {
      switch (role)
      {
      case Qt::DisplayRole:
         if ( !index.parent().isValid() )
         {
            return getPlatformName(index.row());
         }
         else
         {
            return getDeviceName(index.parent().row(),index.row());
         }
      default:
         return QVariant();
      }
   }
   catch (Exception e)
   {
      e.display();
      exit(1);
   }
}






QModelIndex OpenCLDeviceModel::index(int row, int column, const QModelIndex& parent) const
{
   try
   {
      if ( !parent.isValid() )
      {
         return createIndex(row,column,static_cast<quintptr>(0));
      }
      else
      {
         return createIndex(row,column,parent.row()+1);
      }
   }
   catch (Exception e)
   {
      e.display();
      exit(1);
   }
}






QModelIndex OpenCLDeviceModel::parent(const QModelIndex &index) const
{
   try
   {
      if ( index.internalId() == 0 )
      {
         return QModelIndex();
      }
      else
      {
         return createIndex(index.internalId()-1,0);
      }
   }
   catch (Exception e)
   {
      e.display();
      exit(1);
   }
}






int OpenCLDeviceModel::rowCount(const QModelIndex &parent) const
{
   try
   {
      if ( !parent.isValid() )
      {
         return getPlatformCount();
      }
      else if ( !parent.parent().isValid() )
      {
         return getDeviceCount(parent.row());
      }
      else
      {
         return 0;
      }
   }
   catch (Exception e)
   {
      e.display();
      exit(1);
   }
}






QString OpenCLDeviceModel::getDetailedInfo(const QModelIndex &index) const
{
   if ( index.internalId() == 0 )
   {
      return getDetailedPlatformInfo(getPlatformID(index.row()));
   }
   else
   {
      return getDetailedDeviceInfo(getDeviceID(index.parent().row(),index.row()));
   }
}






bool OpenCLDeviceModel::isDevice(const QModelIndex &index) const
{
   return ( index.internalId() != 0 );
}






void OpenCLDeviceModel::setDevice(const QModelIndex &index) const
{
   if ( index.internalId() != 0 )
   {
      cl_platform_id platformID = getPlatformID(index.parent().row());
      cl_device_id deviceID = getDeviceID(index.parent().row(),index.row());
      OpenCLDevice::getInstance().setDevice(platformID,deviceID);
   }
}






void OpenCLDeviceModel::reset()
{
   beginResetModel();
   endResetModel();
}






int OpenCLDeviceModel::getPlatformCount() const
{
   cl_uint total;
   cl_int code = clGetPlatformIDs(0,nullptr,&total);
   if ( code != CL_SUCCESS )
   {
      total = 0;
   }
   return total;
}






cl_platform_id OpenCLDeviceModel::getPlatformID(int row) const
{
   // get total number of platforms
   int total = getPlatformCount();
   if ( row >= total || row < 0 )
   {
      MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid OpenCL Platform"));
      e.out() << tr("An invalid row number was given that is out of range of the available "
                    "platforms. The number must be between 1 and 2 while the given number is 3.")
                 .arg(0).arg(total-1).arg(row);
      throw e;
   }

   // get platform ID for specific row
   cl_platform_id* platforms = new cl_platform_id[total];
   cl_int code = clGetPlatformIDs(total,platforms,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetPlatformIDs",code);
   }
   cl_platform_id id = platforms[row];
   delete[] platforms;
   return id;
}






QString OpenCLDeviceModel::getPlatformName(int row) const
{
   // get platform ID
   cl_platform_id id = getPlatformID(row);

   // get platform name
   StringPointer cName(getPlatformInfo(id,CL_PLATFORM_NAME));
   if ( !cName )
   {
      QString();
   }
   QString name(cName.get());
   return name;
}






int OpenCLDeviceModel::getDeviceCount(int platformRow) const
{
   cl_platform_id id = getPlatformID(platformRow);
   cl_uint total;
   cl_int code = clGetDeviceIDs(id,CL_DEVICE_TYPE_ALL,0,nullptr,&total);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetDeviceIDs",code);
   }
   return total;
}






cl_device_id OpenCLDeviceModel::getDeviceID(int platformRow, int row) const
{
   // get total number of devices and platform ID
   int total = getDeviceCount(platformRow);
   if ( row >= total )
   {
      return 0;
   }
   cl_platform_id platformID = getPlatformID(platformRow);

   // get device ID for specific row
   cl_device_id* devices = new cl_device_id[total];
   cl_int code = clGetDeviceIDs(platformID,CL_DEVICE_TYPE_ALL,total,devices,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetDeviceIDs",code);
   }
   cl_device_id id = devices[row];
   delete[] devices;
   return id;
}






QString OpenCLDeviceModel::getDeviceName(int platformRow, int row) const
{
   // get device ID
   cl_device_id id = getDeviceID(platformRow,row);

   // get device name
   unique_ptr<char> cName(OpenCL::getDeviceInfo<char>(id,CL_DEVICE_NAME));
   if ( !cName )
   {
      return QString();
   }
   QString name(cName.get());
   return name;
}






OpenCLDeviceModel::StringPointer OpenCLDeviceModel::getPlatformInfo(cl_platform_id id
                                                                    , cl_platform_info what) const
{
   // query the size of the c string
   size_t size;
   cl_int code = clGetPlatformInfo(id,what,0,nullptr,&size);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetPlatformInfo",code);
   }

   // allocate memory for c string and get information
   char* info = new char[size];
   code = clGetPlatformInfo(id,what,size,info,nullptr);
   if ( code != CL_SUCCESS )
   {
      delete[] info;
      OpenCL::throwError("clGetPlatformInfo",code);
   }
   return StringPointer(info);
}






QString OpenCLDeviceModel::getDetailedPlatformInfo(cl_platform_id id) const
{
   // get all platform information
   StringPointer name(getPlatformInfo(id,CL_PLATFORM_NAME));
   StringPointer vendor(getPlatformInfo(id,CL_PLATFORM_VENDOR));
   StringPointer version(getPlatformInfo(id,CL_PLATFORM_VERSION));
   StringPointer profile(getPlatformInfo(id,CL_PLATFORM_PROFILE));
   StringPointer extensions(getPlatformInfo(id,CL_PLATFORM_EXTENSIONS));

   // if any information failed delete any allocated memory and return
   if ( !name || !vendor || !version || !profile || !extensions )
   {
      return QString();
   }

   // format all information into HTML for label
   QString detailedInfo;
   QTextStream stream(&detailedInfo);
   stream << "<h3>" << name.get() << "</h3>";
   stream << "<b>Vendor:</b> " << vendor.get() << "<br/>";
   stream << "<b>Version:</b> " << version.get() << "<br/>";
   stream << "<b>Profile:</b> " << profile.get() << "<br/>";
   stream << "<b>Extensions:</b> " << extensions.get() << "<br/>";
   return detailedInfo;
}






QString OpenCLDeviceModel::getDetailedDeviceInfo(cl_device_id id) const
{
   // get all device information
   unique_ptr<char> a(OpenCL::getDeviceInfo<char>(id,CL_DEVICE_NAME));
   StringPointer name(a.release());
   unique_ptr<cl_device_type> type(OpenCL::getDeviceInfo<cl_device_type>(id,CL_DEVICE_TYPE));
   unique_ptr<cl_bool> available(OpenCL::getDeviceInfo<cl_bool>(id,CL_DEVICE_AVAILABLE));
   unique_ptr<cl_bool> compile(OpenCL::getDeviceInfo<cl_bool>(id,CL_DEVICE_COMPILER_AVAILABLE));
   unique_ptr<cl_bool> unifiedMemory(OpenCL::getDeviceInfo<cl_bool>(id,CL_DEVICE_HOST_UNIFIED_MEMORY));
   unique_ptr<cl_uint> clockSpeedMhz(OpenCL::getDeviceInfo<cl_uint>(id,CL_DEVICE_MAX_CLOCK_FREQUENCY));
   unique_ptr<cl_uint> computeUnits(OpenCL::getDeviceInfo<cl_uint>(id,CL_DEVICE_MAX_COMPUTE_UNITS));
   unique_ptr<size_t> workSize(OpenCL::getDeviceInfo<size_t>(id,CL_DEVICE_MAX_WORK_GROUP_SIZE));
   unique_ptr<cl_ulong> globalMemorySize(OpenCL::getDeviceInfo<cl_ulong>(id,CL_DEVICE_GLOBAL_MEM_SIZE));
   unique_ptr<cl_ulong> localMemorySize(OpenCL::getDeviceInfo<cl_ulong>(id,CL_DEVICE_LOCAL_MEM_SIZE));

   // if any information failed delete any allocated memory and return
   if ( !name || !type || !available || !compile || !unifiedMemory || !clockSpeedMhz
        || !computeUnits || !workSize || !globalMemorySize || !localMemorySize )
   {
      return QString();
   }

   // format all information into HTML for label
   QString detailedInfo;
   QTextStream stream(&detailedInfo);
   stream << "<h3>" << name.get() << "</h3>";
   stream << "<b>Type:</b> ";
   switch (*type)
   {
   case CL_DEVICE_TYPE_CPU:
      stream << "CPU";
      break;
   case CL_DEVICE_TYPE_GPU:
      stream << "GPU";
      break;
   case CL_DEVICE_TYPE_ACCELERATOR:
      stream << "Accelerator";
      break;
   case CL_DEVICE_TYPE_DEFAULT:
      stream << "Default";
      break;
   case CL_DEVICE_TYPE_ALL:
      stream << "All";
      break;
   }
   stream << "<br/>";
   stream << "<b>Available:</b> ";
   (*available)? stream << "Yes" : stream << "No";
   stream << "<br/>";
   stream << "<b>Compiler Available:</b> ";
   (*compile)? stream << "Yes" : stream << "No";
   stream << "<br/>";
   stream << "<b>Unified Memory:</b> ";
   (*unifiedMemory)? stream << "Yes" : stream << "No";
   stream << "<br/>";
   stream << "<b>Clock Speed:</b> " << *clockSpeedMhz << " Mhz<br/>";
   stream << "<b>Compute Units:</b> " << *computeUnits << "<br/>";
   stream << "<b>Maximum Workgroup Size:</b> " << *workSize << "<br/>";
   stream << "<b>Global Memory:</b> " << formatMemorySize(*globalMemorySize) << "<br/>";
   stream << "<b>Local Memory:</b> " << formatMemorySize(*localMemorySize) << "<br/>";

   // delete temporarily used local memory
   return detailedInfo;
}
