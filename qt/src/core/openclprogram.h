#ifndef OPENCLPROGRAM_H
#define OPENCLPROGRAM_H
#include <QtCore>
#include <CL/cl.h>
#include <memory>

#include "opencl.h"



class EOpenCLKernel;



class EOpenCLProgram : public EOpenCL
{
public:
   EOpenCLProgram(cl_device_id deviceID, cl_context contextID, cl_command_queue commandQueueID);
   ~EOpenCLProgram();
   ACE_DISBALE_COPY_AND_MOVE(EOpenCLProgram)
   void addSource(const QString& source);
   bool addFile(const QString& filePath);
   bool compile(const QString& options);
   std::unique_ptr<EOpenCLKernel> makeKernel(const QString& name);
   bool hasBuildError() const;
   QString getBuildError() const;
private:
   cl_device_id _deviceID;
   cl_context _contextID;
   cl_command_queue _commandQueueID;
   std::unique_ptr<cl_program> _id {nullptr};
   QList<QByteArray> _sources;
   QString _buildError;
};



#endif
