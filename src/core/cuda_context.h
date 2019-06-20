#ifndef CUDA_CONTEXT_H
#define CUDA_CONTEXT_H
#include <QObject>
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This class contains a CUDA context. There are two main differences between
    * an OpenCL context and a CUDA context: whereas an OpenCL context is the
    * top-level object and is used explicitly to manage OpenCL resources, a CUDA
    * context is created per device per process and is used implicitly by being
    * "bound" to the calling thread using setCurrent(). Threads within the same
    * process may share the same CUDA context, but each thread must bind the CUDA
    * context when it (the thread) is created. This way, the CUDA Driver API uses
    * whatever CUDA context is bound to the calling thread to manage CUDA resources.
    * If no CUDA context is bound, the CUDA Driver API will throw an error.
    */
   class Context : public QObject
   {
      Q_OBJECT
   public:
      explicit Context(Device* device, QObject* parent = nullptr);
      virtual ~Context() override final;
      CUcontext id() const { return _id; }
      void setCurrent() const;
   private:
      CUcontext _id;
   };
}



#endif
