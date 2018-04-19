#ifndef OPENCL_PROGRAM_H
#define OPENCL_PROGRAM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
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
      void compile(Device* device);
      QString getBuildLog(Device* device) const;
      /*!
       */
      cl_program _id;
   };
}



#endif
