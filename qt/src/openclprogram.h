#ifndef OPENCLPROGRAM_H
#define OPENCLPROGRAM_H
#include <QtCore>
#include <CL/cl.h>
#include <memory>

#include "utilities.h"



class EOpenCLKernel;



class EOpenCLProgram
{
public:
   EOpenCLProgram(cl_device_id deviceID, cl_context contextID, cl_command_queue commandQueueID);
   ~EOpenCLProgram();
   ACE_MAKE_NO_COPY_OR_MOVE(EOpenCLProgram);
   void addSource(const QString& source);
   void addFile(const QString& filePath);
   bool compile(const QString& options);
   std::unique_ptr<EOpenCLKernel> makeKernel(const QString& name);
private:
   cl_device_id _deviceID;
   cl_context _contextID;
   cl_command_queue _commandQueueID;
   cl_program* _id {nullptr};
   QList<QByteArray> _sources;
};



#endif
