#ifndef OPENCLPROGRAM_H
#define OPENCLPROGRAM_H
#include <QtCore>
#include <CL/cl.h>
#include <memory>



class OpenCLKernel;



class OpenCLProgram
{
public:
   OpenCLProgram(cl_device_id deviceID, cl_context contextID, cl_command_queue commandQueueID);
   ~OpenCLProgram();
   OpenCLProgram(const OpenCLProgram&) = delete;
   OpenCLProgram& operator=(const OpenCLProgram&) = delete;
   OpenCLProgram(OpenCLProgram&&) = delete;
   OpenCLProgram& operator=(OpenCLProgram&&) = delete;
   void addSource(const QString& source);
   void addFile(const QString& filePath);
   bool compile(const QString& options);
   std::unique_ptr<OpenCLKernel> makeKernel(const QString& name);
private:
   cl_device_id _deviceID;
   cl_context _contextID;
   cl_command_queue _commandQueueID;
   cl_program* _id {nullptr};
   QList<QByteArray> _sources;
};



#endif
