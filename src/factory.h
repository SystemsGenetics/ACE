#ifndef ACCELCOMPENG_FACTORY_H
#define ACCELCOMPENG_FACTORY_H
#include <string>
#include "analytic.h"
#include "data.h"
namespace AccelCompEng
{



/// @brief ACE plugin factory.
///
/// Abstract factory class that produces data and analytic plugin objects.
class Factory
{
public:
   /// Builds new data objects from the given type.
   /// @param Data type to produce.
   virtual Data* build_data(const std::string& type) = 0;
   /// Builds new analytic objects from the given type.
   /// @param Analytic type to produce.
   virtual Analytic* build_analytic(const std::string& type) = 0;
};



}
#endif
