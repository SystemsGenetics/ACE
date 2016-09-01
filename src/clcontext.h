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



class CLContext: public CLProgram, public CLCommandQueue
{
public:
   struct CannotCreate : public Exception { using Exception::Exception; };
   struct NotInitialized : public Exception { using Exception::Exception; };
   friend class Console;
   CLContext() = default;
   ~CLContext();
   bool is_initd();
protected:
   template<class T> CLBuffer<T> buffer(int);
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
