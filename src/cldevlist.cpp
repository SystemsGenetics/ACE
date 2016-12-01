#include <CL/cl.h>
#include "cldevlist.h"
#include "cldevice.h"
namespace AccelCompEng
{



CLDevList::CLDevList(bool isBuild)
{
   if ( isBuild )
   {
      build();
   }
}



void CLDevList::build()
{
   static const char* f = __PRETTY_FUNCTION__;
   cl_platform_id* platforms {nullptr};
   cl_device_id* devices {nullptr};
   try
   {
      cl_uint ptotal;
      cl_int err = clGetPlatformIDs(0,NULL,&ptotal);
      classert<PlatformErr>(err,f,__LINE__);
      platforms = new cl_platform_id[ptotal];
      err = clGetPlatformIDs(ptotal,platforms,NULL);
      classert<PlatformErr>(err,f,__LINE__);
      for (int i = 0; i < ptotal ;++i)
      {
         cl_uint dtotal;
         err = clGetDeviceIDs(platforms[i],CL_DEVICE_TYPE_ALL,0,NULL,&dtotal);
         classert<DeviceErr>(err,f,__LINE__);
         devices = new cl_device_id[dtotal];
         err = clGetDeviceIDs(platforms[i],CL_DEVICE_TYPE_ALL,dtotal,devices,NULL);
         classert<DeviceErr>(err,f,__LINE__);
         _list.push_back({});
         for (int j = 0; j < dtotal ;++j)
         {
            _list.back().push_back(std::unique_ptr<CLDevice>(new CLDevice(i,j,platforms[i],
                                                                          devices[j])));
         }
         delete[] devices;
         devices = nullptr;
      }
      delete[] platforms;
   }
   catch (...)
   {
      _list.clear();
      if (platforms) delete[] platforms;
      if (devices) delete[] devices;
      throw;
   }
}



CLDevList::Iterator CLDevList::begin()
{
   return {0,0,this};
}



CLDevList::Iterator CLDevList::end()
{
   int pi = 0;
   int di = 0;
   if ( _list.size() > 0 )
   {
      pi = static_cast<int>(_list.size()-1);
      di = static_cast<int>(_list.back().size());
   }
   return {pi,di,this};
}



void CLDevList::refresh()
{
   _list.clear();
   build();
}



bool CLDevList::exist(int p, int d)
{
   return ( p < _list.size() && d < _list[p].size() );
}



CLDevice& CLDevList::at(int p, int d)
{
   return *(_list.at(p).at(d).get());
}



CLDevice& CLDevList::Iterator::operator*()
{
   return *(_devList->_list[_pi][_di].get());
}



void CLDevList::Iterator::operator++()
{
   if ( _devList->_list.size() > 0 )
   {
      if ( ++_di >= _devList->_list[_pi].size() )
      {
         _di = 0;
         if ( ++_pi >= _devList->_list.size() )
         {
            _pi--;
            _di = _devList->_list.back().size();
         }
      }
   }
}



void CLDevList::Iterator::operator--()
{
   if (--_di==-1)
   {
      _di = 0;
      if (--_pi==-1)
      {
         _pi = 0;
      }
   }
}



bool CLDevList::Iterator::operator!=(const Iterator& cmp)
{
   return ( _pi != cmp._pi || _di != cmp._di );
}



CLDevList::Iterator::Iterator(int p, int d, CLDevList* devList):
   _pi(p),
   _di(d),
   _devList(devList)
{}



}
