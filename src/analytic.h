#ifndef ACCELCOMPENG_ANALYTIC_H
#define ACCELCOMPENG_ANALYTIC_H
#include <CL/cl.hpp>
#include <string>
#include <memory>
#include "data.h"
#include "terminal.h"
#include "clcontext.h"
namespace AccelCompEng
{



/// @brief Analytic interface class
///
/// Defines the interface between the program and any analytic plugin class.
/// This virtual class is instantiated for running a specific analytic command
/// and destroyed once the command has executed.
class Analytic : public CLContext
{
public:
   virtual ~Analytic() = default;
   /// @brief Execute analytic.
   ///
   /// Begin execution of the analytic.
   ///
   /// @param ops Arguments and options from the user.
   /// @param tm Terminal interface for the user console.
   void execute(GetOpts& ops, Terminal& tm);
   /// @brief Add data input.
   ///
   /// Adds a single input data object that will be used in execution.
   ///
   /// @param in Pointer to the data object to be used as input.
   virtual void input(Data* in) = 0;
   /// @brief Add data output.
   ///
   /// Adds a single output data object that will be used in execution.
   ///
   /// @param out Pointer to the data object to be used as output.
   virtual void output(Data* out) = 0;
protected:
   /// @brief OpenCL execution.
   ///
   /// Runs OpenCL accelerated execution of analytic on all input and output data object.
   ///
   /// @param ops Arguments and options from the user.
   /// @param tm Terminal interface for the user console.
   virtual void execute_cl(GetOpts& ops, Terminal& tm) = 0;
   /// @brief Serial execution.
   ///
   /// Runs non-accelerated execution of analytic on all input and output data object.
   ///
   /// @param ops Arguments and options from the user.
   /// @param tm Terminal interface for the user console.
   virtual void execute_pn(GetOpts& ops, Terminal& tm) = 0;
};



}
#endif
