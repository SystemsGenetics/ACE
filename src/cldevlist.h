/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#ifndef ACCELCOMPENG_CLDEVLIST_H
#define ACCELCOMPENG_CLDEVLIST_H
#include <vector>
#include <memory>
#include "exception.h"
namespace AccelCompEng
{



class CLDevice;



/// @brief Container list of all OpenCL devices.
///
/// List of all available OpenCL devices. The list is constructed upon instantiation of the object
/// and can be recompiled by called the refresh() function.
class CLDevList
{
public:
   struct PlatformErr : public Exception { using Exception::Exception; };
   struct DeviceErr : public Exception { using Exception::Exception; };
   class Iterator;
   /// Compile list of available OpenCL devices unless told not to.
   /// @param isBuild Should the list be compiled or left empty?
   CLDevList(bool isBuild = false);
   CLDevList(const CLDevList&) = default;
   CLDevList(CLDevList&&) = default;
   CLDevList& operator=(const CLDevList&) = default;
   CLDevList& operator=(CLDevList&&) = default;
   Iterator begin();
   Iterator end();
   /// Clears vector list of previously built devices and builds new list of OpenCL devices.
   void refresh();
   /// Returns if specified OpenCL device exists.
   /// @param p The increment into list of platforms.
   /// @param d The increment into list of devices of given platform.
   /// @return True if given OpenCL device exists.
   bool exist(int,int);
   CLDevice& at(int,int);
private:
   void build();
   std::vector<std::vector<std::unique_ptr<CLDevice>>> _list;
};



/// @brief Device iterator.
///
/// Iterater for OpenCL device list.
class CLDevList::Iterator
{
public:
   friend class CLDevList;
   Iterator(const Iterator&) = default;
   Iterator(Iterator&&) = default;
   Iterator& operator=(const Iterator&) = default;
   Iterator& operator=(Iterator&&) = default;
   CLDevice& operator*();
   void operator++();
   void operator--();
   bool operator!=(const Iterator&);
private:
   Iterator(int,int,CLDevList*);
   CLDevList* _devList;
   int _pi;
   int _di;
};



}
#endif
