#ifndef OPENCL_CONTEXT_H
#define OPENCL_CONTEXT_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    * This contains an OpenCL context. This is a very basic class that simply 
    * maintains ownership over an underlying context along with providing basic 
    * information about it such as the list of devices associated with it. This class 
    * in turn is used by other OpenCL classes that derive from an OpenCL context. 
    */
   class Context : public QObject
   {
      Q_OBJECT
   public:
      explicit Context(const QList<Device*>& devices, QObject* parent = nullptr);
      virtual ~Context() override final;
      cl_context id() const;
      const QList<Device*>& devices() const;
   private:
      /*!
       * The OpenCL context ID of this object. 
       */
      cl_context _id;
      /*!
       * Pointer list of devices that are part of this context. 
       */
      QList<Device*> _devices;
   };
}



#endif
