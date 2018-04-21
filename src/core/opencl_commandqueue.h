#ifndef OPENCL_COMMANDQUEUE_H
#define OPENCL_COMMANDQUEUE_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    */
   class CommandQueue : public QObject
   {
      Q_OBJECT
   public:
      explicit CommandQueue(Context* context, Device* device, QObject* parent = nullptr);
      virtual ~CommandQueue() override final;
      cl_command_queue id() const;
   private:
      /*!
       */
      cl_command_queue _id;
   };
}



#endif
