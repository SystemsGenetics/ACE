/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#ifndef ACCELCOMPENG_CLDEVICE_H
#define ACCELCOMPENG_CLDEVICE_H
#include <CL/cl.h>
#include <string>
#include "exception.h"
namespace AccelCompEng
{



/// @brief Represents a single OpenCL device.
///
/// Represents a single OpenCL device that can be queried for information about the device along
/// with pointers to the OpenCL platform and device interface. The "address" of these devices are
/// represented by two numbers, the first being the platform the device is part of and the second
/// being which device in the platform.
class CLDevice
{
public:
   struct InfoErr : public Exception { using Exception::Exception; };
   friend class CLDevList;
   /// Defines all possible types of information that can be queried about an OpenCL device.
   enum CLInfo
   {
      ident, ///< The two identifying numbers of device.
      name, ///< The name of the device.
      pname, ///< The name of the platform the device is on.
      type, ///< The OpenCL device type.
      online, ///< Is the device online or not.
      unified_mem, ///< Does the device have unified memory.
      addr_space, ///< The address space in bits.
      clock, ///< The maximum clock frequency.
      compute_units, ///< The number of compute units.
      work_size, ///< The maximum workgroup size.
      global_mem, ///< Size of global memory.
      local_mem, ///< Size of local memory.
      extensions, ///< Extensions this device supports.
      float_hp, ///< Floating point operations supported for half precision.
      float_sp, ///< Floating point operations supported for single precision.
      float_dp ///< Floating point operations supported for double precision.
   };
   CLDevice(const CLDevice&) = default;
   CLDevice(CLDevice&&) = default;
   CLDevice& operator=(const CLDevice&) = default;
   CLDevice& operator=(CLDevice&&) = default;
   /// Return OpenCL platform id of device.
   /// @return OpenCL platform id.
   cl_platform_id platform();
   /// Return OpenCL device id of CLDevice object.
   /// @return OpenCL device id.
   cl_device_id device();
   /// Returns string with requested information about OpenCL device. The different types of
   /// information to be queired is defined in clinfo.
   /// @param which Enumerated type specifying what type of information to query.
   /// @return Requested information.
   std::string info(CLInfo) const;
   bool operator==(const CLDevice&);
private:
   /// This is the only constructor for this class which is private and should only be used by the
   /// CLDevList class. The parameters given for the device are NOT checked for validity.
   /// @param pinc The increment into list of platforms.
   /// @param dinc The increment into list of devices for given platform.
   /// @param pid The OpenCL platform id for the device.
   /// @param did The OpenCL device id for the device.
   CLDevice(int pinc, int dinc, cl_platform_id pid, cl_device_id did);
   void yes_no(std::ostringstream&,bool) const;
   void size_it(std::ostringstream&,long) const;
   template<class T> T get_info(cl_device_info) const;
   static constexpr int _fPrecision {2};
   int _pinc;
   int _dinc;
   cl_platform_id _pid;
   cl_device_id _did;
};



template<class T> T CLDevice::get_info(cl_device_info infoType) const
{
   static const char* f = __PRETTY_FUNCTION__;
   T ret;
   cl_int err;
   err = clGetDeviceInfo(_did,infoType,sizeof(T),&ret,NULL);
   classert<InfoErr>(err,f,__LINE__);
   return ret;
}



template<> inline std::string CLDevice::get_info(cl_device_info infoType) const
{
   static const char* f = __PRETTY_FUNCTION__;
   size_t strSize;
   cl_int err;
   err = clGetDeviceInfo(_did,infoType,0,NULL,&strSize);
   classert<InfoErr>(err,f,__LINE__);
   char buffer[strSize];
   err = clGetDeviceInfo(_did,infoType,strSize,buffer,NULL);
   classert<InfoErr>(err,f,__LINE__);
   return buffer;
}



}
#endif
