#ifndef OPENCLPROGRAM_H
#define OPENCLPROGRAM_H
#include <QtCore>
#include <CL/cl.h>
#include <memory>

#include "opencl.h"



class EOpenCLKernel;



/// Holds an OpenCL program.
class EOpenCLProgram : public EOpenCL
{
public:
   /// Internal command DO NOT USE.
   EOpenCLProgram(cl_device_id deviceID, cl_context contextID, cl_command_queue commandQueueID);
   ~EOpenCLProgram();
   ACE_DISBALE_COPY_AND_MOVE(EOpenCLProgram)
   /// Add OpenCL C source code.
   ///
   /// @param source Source code.
   void addSource(const QString& source);
   /// Add OpenCL C source code from file.
   ///
   /// @param filePath Path to OpenCL C source file.
   ///
   /// @return True on success.
   bool addFile(const QString& filePath);
   /// Compile all added OpenCL C source code.
   ///
   /// @param options Optional parameters passed to compiler.
   ///
   /// @return True on success.
   bool compile(const QString& options = "");
   /// Make a new kernel from compiled source code. WARNING compile method must be called before
   /// using this method.
   ///
   /// @param name Name of kernel.
   ///
   /// @return Pointer to new kernel object on success or null pointer on failure.
   std::unique_ptr<EOpenCLKernel> makeKernel(const QString& name);
   /// Determine if a compile error has occured when calling compile method.
   ///
   /// @return True if error has occured.
   bool hasBuildError() const;
   /// Get compile error.
   ///
   /// @return Compile error.
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
