#ifndef OPENCL_COMMANDQUEUE_H
#define OPENCL_COMMANDQUEUE_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    * This contains an OpenCL command queue. This is a very basic class that simply 
    * maintains ownership of an underlying command queue. The main purpose of this 
    * class is to be used by other classes that add commands to its underlying command 
    * queue. 
    */
   class CommandQueue : public QObject
   {
      Q_OBJECT
   public:
      explicit CommandQueue(Context* context, Device* device, QObject* parent = nullptr);
      virtual ~CommandQueue() override final;
      cl_command_queue id() const;
      Device* device() const;
   private:
      /*!
       * The OpenCL command queue ID of this object. 
       */
      cl_command_queue _id;
      /*!
       * Pointer to the device this command queue uses. 
       */
      Device* _device;
   };
}



#endif
