#ifndef OPENCL_CONTEXT_H
#define OPENCL_CONTEXT_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    */
   class Context : public QObject
   {
      Q_OBJECT
   public:
      explicit Context(Platform* platform, const QList<Device*>& devices, QObject* parent = nullptr);
      virtual ~Context() override final;
      cl_context id() const;
      const QList<Device*>& devices() const;
   private:
      /*!
       */
      cl_context _id;
      /*!
       */
      QList<Device*> _devices;
   };
}



#endif
