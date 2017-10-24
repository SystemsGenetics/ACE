#include "opencl.h"






EOpenCL::Status EOpenCL::getStatus() const
{
   return _status;
}






QString EOpenCL::getErrorFunction() const
{
   return _errorFunction;
}






QString EOpenCL::getErrorCode() const
{
   return Ace::OpenCL::generateErrorString(_errorCode);
}






void EOpenCL::fillException(EException& e) const
{
   e.setTitle(QObject::tr("OpenCL Error"));
   e.setDetails(QString("%1: %2").arg(getErrorFunction()).arg(getErrorCode()));
}






void EOpenCL::setNoOpenCL()
{
   if ( _status == Ok )
   {
      _status = NoOpenCL;
   }
}






void EOpenCL::reportError(const QString &function, cl_int code)
{
   // only report error if none has been set
   if ( _status == Ok )
   {
      // set new error
      _errorFunction = function;
      _errorCode = code;
      _status = OpenCLError;
   }
}






EOpenCL::operator bool() const
{
   return _status == Ok;
}
