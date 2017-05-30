#ifndef OPENCL_H
#define OPENCL_H
#include <QtCore>
#include <CL/cl.h>

#include "utilities.h"



class EOpenCL
{
public:
   enum Status
   {
      Ok = 0
      ,NoOpenCL
      ,OpenCLError
   };
   Status getStatus() const;
   QString getErrorFunction() const;
   QString getErrorCode() const;
   operator bool() const;
protected:
   EOpenCL() = default;
   void setNoOpenCL();
   void reportError(const QString& function, cl_int code);
private:
   Status _status {Ok};
   QString _errorFunction;
   cl_int _errorCode {Ace::OpenCL::ReturnCodes::Success};
};



#endif
