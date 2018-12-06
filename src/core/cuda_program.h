#ifndef CUDA_PROGRAM_H
#define CUDA_PROGRAM_H
#include <QObject>
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This contains a CUDA program. The purpose of this class is to build a
    * given list of files assumed to be CUDA kernel source code, and to provide
    * CUDA kernel objects from compiled source code.
    */
   class Program : public QObject
   {
      Q_OBJECT
   public:
      explicit Program(const QStringList& paths, QObject* parent = nullptr);
      virtual ~Program() override final;
      CUmodule id() const { return _module; }
   private:
      QString readSourceFile(const QString& path);
      void build();
      QString getBuildLog() const;
      nvrtcProgram _program {nullptr};
      CUmodule _module {nullptr};
      QString _source;
   };
}



#endif
