#ifndef OPENCL_H
#define OPENCL_H
#include <QtCore>
#include <CL/cl.h>

#include "utilities.h"
#include "eexception.h"



/// Base opencl class that all other opencl classes inherit. Used for error handling.
class EOpenCL
{
public:
   /// Status of opencl object.
   enum Status
   {
      Ok = 0 ///< Ok
      ,NoOpenCL ///< There is no OpenCL support on current system
      ,OpenCLError ///< An OpenCL error occured
   };
   /// Get status of opencl object.
   ///
   /// @return Status.
   Status getStatus() const;
   /// Get name of OpenCL function that failed, if any.
   ///
   /// @return Function name.
   QString getErrorFunction() const;
   /// Get error code description, if any.
   ///
   /// @return Error code description.
   QString getErrorCode() const;
   /// Write description of error in given exception.
   ///
   /// @param e Exception to have error description written to.
   void fillException(EException& e) const;
   /// Determine if object is in an OK state.
   ///
   /// @return True if object is in the Ok state.
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
