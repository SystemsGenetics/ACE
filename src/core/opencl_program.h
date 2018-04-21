#ifndef OPENCL_PROGRAM_H
#define OPENCL_PROGRAM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    * This contains an OpenCL program. The primary purpose of this class is to build a 
    * given list of files assumed to be OpenCL kernel source code. All devices part of 
    * the context this program is derived from compiles all the source files given. 
    * The only other purpose of this class is to provide its OpenCL program ID to 
    * create kernels from the built program kernel code. 
    */
   class Program : public QObject
   {
      Q_OBJECT
   public:
      explicit Program(Context* context, const QStringList& paths, QObject* parent);
      virtual ~Program() override final;
      cl_program id() const;
   private:
      char* readSourceFile(const QString& path, size_t* size);
      void deleteStrings(const char** sources, int size);
      void build(Device* device);
      QString getBuildLog(Device* device) const;
      /*!
       * The OpenCL program ID of this object. 
       */
      cl_program _id;
   };
}



#endif
