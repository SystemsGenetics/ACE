/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#ifndef ACCELCOMPENG_CONSOLE_H
#define ACCELCOMPENG_CONSOLE_H
#include <string>
#include <list>
#include "terminal.h"
#include "cldevlist.h"
#include "datamap.h"
#include "exception.h"
#include "getopts.h"
namespace AccelCompEng
{



class CLDevice;



/// @brief Main program Console.
///
/// Designed to take over execution of the program and manage commands supplied
/// by the user for all data and analytic objects. Also manages and displays
/// information about all OpenCL devices attached to the program's machine.
class Console
{
public:
   struct InvalidUse : public Exception { using Exception::Exception; };
   class CommandError;
   /// Initialize the console object, keeping in mind there can be only one console object.
   /// @param argc Argument from main.
   /// @param argv Argument from main.
   /// @param tm Terminal object for ACE.
   /// @param factory Plugin factory for ACE.
   /// @param dmap Data manager for ACE.
   /// @param header String that will be printed with command prompt.
   Console(int argc, char* argv[], Terminal& tm, Factory& factory, DataMap& dmap,
           const char* header);
   ~Console();
   Console(const Console&) = delete;
   Console& operator=(const Console&) = delete;
   Console(Console&&) = delete;
   Console& operator=(Console&&) = delete;
   /// Give control to console and initiate command prompt for user.
   void run();
   /// Run a single command in the console that is non-blocking and returns control once the command
   /// is complete.
   bool command(const std::string& line);
private:
   struct CommandQuit {};
   void process(GetOpts&);
   void gpu_process(GetOpts&);
   void gpu_list();
   void gpu_info(GetOpts&);
   void gpu_set(GetOpts&);
   void gpu_clear();
   void data_open(GetOpts&);
   void data_close(GetOpts&);
   void data_select(GetOpts&);
   void data_clear();
   void data_list();
   void data_history(GetOpts&);
   void data_load(GetOpts&);
   void data_dump(GetOpts&);
   void data_query(GetOpts&);
   void analytic(GetOpts&);
   void seperate(const std::string&,const std::string&,std::string&,std::string&);
   void rec_history(History::Iterator,History::Iterator,int);
   void print_pad(int);
   static bool _lock;
   Terminal& _tm;
   DataMap& _dataMap;
   CLDevice* _device;
   CLDevList* _devList;
   std::string _header;
   Factory& _factory;
};



/// @brief Single console error.
///
/// This holds a single error that occured and is thrown from somewhere outside
/// of the console class which the console class is designed to catch and
/// report to the user.
class Console::CommandError
{
public:
   /// Initializes a new command error and sets who threw it and its message.
   /// @param who Identifies who threw the error message.
   /// @param msg The message describing the error that occured.
   CommandError(const std::string&,const std::string&);
   /// Prints the error message to the terminal.
   /// @param tm The program's terminal that will be printed to.
   void print(Terminal&);
private:
   std::string _who;
   std::string _msg;
};



}
#endif
