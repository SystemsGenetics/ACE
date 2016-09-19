#ifndef ACCELCOMPENG_CLCONTEXT_H
#define ACCELCOMPENG_CLCONTEXT_H
#include <CL/cl.h>
#include "cldevice.h"
#include "clprogram.h"
#include "clcommandqueue.h"
#include "clkernel.h"
#include "clbuffer.h"
#include "clevent.h"
namespace AccelCompEng
{



/// @brief ACE OpenCL system.
///
/// Wrapper for OpenCL context. Also contains wrappers for OpenCL program and commandqueue.
class CLContext: public CLProgram, public CLCommandQueue
{
public:
   struct CannotCreate : public Exception { using Exception::Exception; };
   struct NotInitialized : public Exception { using Exception::Exception; };
   friend class Console;
   /// Creates an empty wrapper with no OpenCL context.
   CLContext() = default;
   ~CLContext();
   /// Tests if object is initialized and contains a valid OpenCL context.
   /// @return Does this object have an OpenCL context?
   bool is_initd();
protected:
   /// Creates a new OpenCL buffer object.
   /// @tparam T The basic type of the new buffer object.
   /// @param size Size of array for the new buffer object.
   template<class T> CLBuffer<T> buffer(int size);
private:
   void init_cl(CLDevice& dev);
   bool _initd {false};
   cl_context _cid;
};



template<class T> CLBuffer<T> CLContext::buffer(int size)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotInitialized>(_initd,f,__LINE__);
   return CLBuffer<T>(_cid,size);
}



}
#endif
