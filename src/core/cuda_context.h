#ifndef CUDA_CONTEXT_H
#define CUDA_CONTEXT_H
#include <QObject>
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This contains a CUDA context. Unlike OpenCL, in which the context is
    * the top-level object, a CUDA context is created per device per process.
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
