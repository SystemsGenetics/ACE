#include "ace_settings.h"
#include "opencl_platform.h"



using namespace Ace;
//



/*!
 */
Settings* Settings::_instance {nullptr};






/*!
 */
Settings& Settings::instance()
{
   if ( !_instance )
   {
      _instance = new Settings;
   }
   return *_instance;
}






/*!
 */
OpenCL::Device* Settings::device() const
{
   return _device;
}






/*!
 *
 * @param device  
 */
void Settings::setDevice(OpenCL::Device* device)
{
   _device = device;
}






/*!
 */
Settings::Settings()
{
   for (int i = 0; i < OpenCL::Platform::size() ;++i)
   {
      if ( OpenCL::Platform::get(i)->deviceSize() > 0 )
      {
         _device = OpenCL::Platform::get(i)->device(0);
         break;
      }
   }
}
