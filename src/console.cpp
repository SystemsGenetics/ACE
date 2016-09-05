/*
 * See GPL.txt for copyright information.
 *
 * Author: Joshua Burns
 *
 */
#include <vector>
#include <sstream>
#include <forward_list>
#include <memory>
#include <time.h>
#include "console.h"
#include "cldevice.h"
#include "data.h"
#include "factory.h"
namespace AccelCompEng
{



bool Console::_lock {false};



Console::Console(int argc, char* argv[], Terminal& tm, Factory& factory, DataMap& dmap,
                 const char* header):
   _tm {tm},
   _factory(factory),
   _dataMap {dmap},
   _device {nullptr},
   _header(header)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(!_lock,f,__LINE__);
   _lock = true;
   try
   {
      _devList = new CLDevList(true);
   }
   catch (CLDevList::PlatformErr)
   {
      tm << Terminal::warning;
      tm << "OpenCL error while attempting to enumerate platforms, OpenCL is now DISABLED.\n";
      tm << Terminal::general;
      _devList = new CLDevList;
   }
   catch (CLDevList::DeviceErr)
   {
      tm << Terminal::warning;
      tm << "OpenCL error while attempting to enumerate devices, OpenCL is now DISABLED.\n";
      tm << Terminal::general;
      _devList = new CLDevList;
   }
}



/// Deletes OpenCL device if one is set and unsets lock to console.
Console::~Console()
{
   _lock = false;
   if (_device)
   {
      delete _device;
   }
   if (_devList)
   {
      delete _devList;
   }
}



/// Prints welcome message and goes directly to terminal loop.
void Console::run()
{
   terminal_loop();
}



/// @brief Main program terminal loop.
///
/// This is the main function loop of the program. This loop will continue to
/// grab one input from the Terminal interface until the quit command has been
/// processed. Each user line is processed by calling process. Any exception
/// thrown is also caught here and caught to prevent the program from crashing;
/// printing output to the user about what type of exception.
void Console::terminal_loop()
{
   bool alive = true;
   while (alive)
   {
      string header {_header};
      string line;
      if (_dataMap.selected()!=_dataMap.end())
      {
         header += "[";
         header += _dataMap.selected().file();
         header += "]";
      }
      header += ":>";
      _tm.header(header);
      _tm >> line;
      _tm << "\n";
      if (!line.empty())
      {
         GetOpts ops(line);
         try
         {
            process(ops);
         }
         catch (CommandError e)
         {
            _tm << Terminal::warning;
            e.print(_tm);
            _tm << Terminal::general;
         }
         catch (CommandQuit)
         {
            alive = false;
         }
         catch (Exception e)
         {
            _tm << Terminal::error;
            _tm << "ACE Exception Caught!\n";
            _tm << "What: " << e.what() << "\n";
            _tm << "Location: " << e.function() << ":" << e.line() << "\n";
            if (e.detail())
            {
               _tm << "Details:" << e.detail() << "\n";
            }
            _tm << Terminal::general;
         }
         catch (std::exception& stde)
         {
            _tm << Terminal::error;
            _tm << "Standard Library Exception Caught!\n";
            _tm << "What: " << stde.what() << "\n";
            _tm << "It is HIGHLY recommended you immediately close this program.\n";
            _tm << Terminal::general;
         }
         catch (...)
         {
            _tm << Terminal::error;
            _tm << "UNKNOWN EXCEPTION CAUGHT!\n";
            _tm << "It is HIGHLY recommended you immediately close this program.\n";
            _tm << Terminal::general;
         }
      }
      _tm << Terminal::flush;
   }
}



/// Processes user command and routes to specific command.
///
/// @param ops Command line object with options.
///
/// @exception CommandQuit Signals the user has inputed the quit command to exit
/// the program.
void Console::process(GetOpts& ops)
{
   enum {Analytic=0,GPU,Open,Load,Select,Dump,Query,Close,List,Clear,History,
         Quit};
   switch (ops.com_get({"gpu","open","load","select","dump","query","close",
                        "list","clear","history","quit"}))
   {
   case GPU:
      ops.com_pop();
      gpu_process(ops);
      break;
   case Open:
      ops.com_pop();
      data_open(ops);
      break;
   case Close:
      ops.com_pop();
      data_close(ops);
      break;
   case Select:
      ops.com_pop();
      data_select(ops);
      break;
   case Clear:
      data_clear();
      break;
   case List:
      data_list();
      break;
   case History:
      ops.com_pop();
      data_history(ops);
      break;
   case Load:
      ops.com_pop();
      data_load(ops);
      break;
   case Dump:
      ops.com_pop();
      data_dump(ops);
      break;
   case Query:
      ops.com_pop();
      data_query(ops);
      break;
   case Quit:
      throw CommandQuit();
   case Analytic:
      analytic(ops);
      break;
   }
}



/// Processes OpenCL command and routes to specific command given.
///
/// @param ops Command line object with options.
///
/// @exception CommandError OpenCL subcommand given not found.
void Console::gpu_process(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("gpu","subcommand not provided.");
   }
   enum {Error=0,List,Info,Set,Clear};
   switch (ops.com_get({"list","info","set","clear"}))
   {
   case List:
      gpu_list();
      break;
   case Info:
      ops.com_pop();
      gpu_info(ops);
      break;
   case Set:
      ops.com_pop();
      gpu_set(ops);
      break;
   case Clear:
      gpu_clear();
      break;
   case Error:
      std::ostringstream buffer;
      buffer << "command " << ops.com_front() << " not recognized.";
      throw CommandError("gpu",buffer.str());
      break;
   }
}



/// Executes command to list all available OpenCL devices.
void Console::gpu_list()
{
   for (auto d:*_devList)
   {
      _tm << d.info(CLDevice::ident) << " ";
      _tm << d.info(CLDevice::name);
      if (_device&&d==*_device)
      {
         _tm << " ***";
      }
      _tm << "\n";
   }
   _tm << Terminal::flush;
}



/// Executes command to print basic info of of given OpenCL device.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected.
void Console::gpu_info(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("gpu info","command requires 1 argument.");
   }
   int p,d;
   char sep;
   std::istringstream str(ops.com_front());
   if ((str >> p >> sep >> d)&&sep==':'&&_devList->exist(p,d))
   {
      CLDevice& dev {_devList->at(p,d)};
      _tm << "===== " << dev.info(CLDevice::name) << " ("
          << dev.info(CLDevice::type) << ") =====\n";
      _tm << "Online: " << dev.info(CLDevice::online) << ".\n";
      _tm << "Unified Memory: " << dev.info(CLDevice::unified_mem) << ".\n";
      _tm << dev.info(CLDevice::addr_space) << " bit address space.\n";
      _tm << dev.info(CLDevice::clock) << "Mhz max clock frequency.\n";
      _tm << dev.info(CLDevice::compute_units) << " compute unit(s), "
          << dev.info(CLDevice::work_size) << " work-item(s) per unit.\n";
      _tm << dev.info(CLDevice::global_mem) << " global memory, "
          << dev.info(CLDevice::local_mem) << " local memory.\n";
   }
   else
   {
      std::ostringstream buffer;
      buffer << "cannot find OpenCL device \"" << ops.com_front() << "\".";
      throw CommandError("gpu info",buffer.str());
   }
}



/// Executes command that sets OpenCL device for analytic computation.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected.
void Console::gpu_set(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("gpu info","command requires 1 argument.");
   }
   int p,d;
   char sep;
   std::istringstream str(ops.com_front());
   if ((str >> p >> sep >> d)&&sep==':'&&_devList->exist(p,d))
   {
      if (_device)
      {
         delete _device;
      }
      _device = new CLDevice {_devList->at(p,d)};
      _tm << "OpenCL device set to \"" << _device->info(CLDevice::CLInfo::name)
          << "\".\n";
   }
   else
   {
      std::ostringstream buffer;
      buffer << "cannot find OpenCL device \"" << ops.com_front() << "\".";
      throw CommandError("gpu info",buffer.str());
   }
}



/// Executes command that clears any OpenCL device set for computation.
void Console::gpu_clear()
{
   if (_device)
   {
      delete _device;
      _device = nullptr;
      _tm << "OpenCL device cleared.\n";
   }
   else
   {
      _tm << "no OpenCL devie set.\n";
   }
}



/// Executes command to open a data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected, data object is
/// already loaded, or data type given in invalid. Exception specifying which
/// error occured.
void Console::data_open(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("open","command requires 1 argument.");
   }
   string file;
   string type;
   seperate("open",ops.com_front(),file,type);
   if (file.empty()||type.empty())
   {
      throw CommandError("open","syntax error detected in first argument.");
   }
   bool willSelect {ops.has_opt("select")};
   File* np;
   try
   {
      np = dynamic_cast<File*>(_dataMap.open(file,type,willSelect));
   }
   catch (DataMap::AlreadyExists)
   {
      std::ostringstream buffer;
      buffer << "error: " << file << " already exists.";
      throw CommandError("open",buffer.str());
   }
   catch (DataMap::InvalidType)
   {
      std::ostringstream buffer;
      buffer << "error: '" << type << "' is not valid data type.";
      throw CommandError("open",buffer.str());
   }
   if (np->is_new())
   {
      np->init_history();
      np->history().init(file,"__NEW__",ops.orig());
      np->write_history();
      _tm << "New file " << file << " opened.\n";
   }
   else
   {
      _tm << "Old file " << file << " opened.";
   }
}



/// Executes command to close data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected or data object
/// cannot be find, exception specifying which one.
void Console::data_close(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("close","command requires 1 argument.");
   }
   bool wasSelected;
   try
   {
      wasSelected = _dataMap.close(ops.com_front());
   }
   catch (DataMap::DoesNotExist)
   {
      std::ostringstream buffer;
      buffer << ops.com_front() << " cannot be found.";
      throw CommandError("close",buffer.str());
   }
   _tm << ops.com_front() << " data file closed.\n";
}



/// Executes command to select a loaded data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected or data object
/// cannot be find, exception specifying which one.
void Console::data_select(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("select","command requires 1 argument.");
   }
   try
   {
      _dataMap.select(ops.com_front());
   }
   catch (DataMap::DoesNotExist)
   {
      std::ostringstream buffer;
      buffer << ops.com_front() << " cannot be found.";
      throw CommandError("select",buffer.str());
   }
   _tm << ops.com_front() << " data file selected.\n";
}



/// Executes command to clear any selected data object.
void Console::data_clear()
{
   if (_dataMap.unselect())
   {
      _tm << "data selection cleared.\n";
   }
   else
   {
      _tm << "no data object selected.\n";
   }
}



/// Executes command to list all loaded data objects.
void Console::data_list()
{
   for (auto i=_dataMap.begin();i!=_dataMap.end();++i)
   {
      _tm << i.file() << " [" << i.type() << "]";
      if (i==_dataMap.selected())
      {
         _tm << " ***";
      }
      _tm << "\n";
   }
}



/// Executes command to print the history of a data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Syntax error in command detected or data object
/// cannot be find, exception specifying which one.
void Console::data_history(GetOpts& ops)
{
   if (ops.com_empty())
   {
      throw CommandError("history","command requires 1 argument.");
   }
   File* kp = dynamic_cast<File*>(_dataMap.find(ops.com_front()));
   if (!kp)
   {
      std::ostringstream buffer;
      buffer << ops.com_front() << " cannot be found.";
      throw CommandError("history",buffer.str());
   }
   History& h = kp->history();
   time_t t = h.time_stamp();
   struct tm* bt = localtime(&t);
   _tm << "Time Stamp: ";
   _tm << bt->tm_mday << "-" << (bt->tm_mon+1) << "-" << (bt->tm_year+1900)
       << " " << bt->tm_hour << ":" << bt->tm_min << "\n";
   _tm << "File Name: " << h.file_name() << "\n";
   _tm << "Object: " << h.object() << "\n";
   _tm << "Command: " << h.command() << "\n";
   if (h.has_children())
   {
      _tm << "{\n";
      rec_history(h.begin(),h.end(),1);
      _tm << "{\n";
   }
}



/// Executes command to call the load function of a data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError No data object is currently selected or attempting
/// to overwrite existing data object with force option, exception specifying
/// which one.
void Console::data_load(GetOpts& ops)
{
   try
   {
      Data& k = *(_dataMap.current());
      bool willForce {ops.has_opt("force",true)};
      if (!k.empty()&&!willForce)
      {
         throw CommandError("load","cannot overwrite non-empty data object.");
      }
      k.clear();
      k.init_history();
      k.history().init(_dataMap.selected().file(),"__LOAD__",ops.orig());
      k.write_history();
      _dataMap.load(ops,_tm);
   }
   catch (DataMap::NoSelect)
   {
      throw CommandError("load","no data object selected.");
   }
}



/// Executes command to call the dump function of a data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError No data object is currently selected.
void Console::data_dump(GetOpts& ops)
{
   try
   {
      _dataMap.dump(ops,_tm);
   }
   catch (DataMap::NoSelect)
   {
      throw CommandError("dump","no data object selected.");
   }
}



/// Executes command to call the query function of a data object.
///
/// @param ops Command line object with options.
///
/// @exception CommandError No data object is currently selected.
void Console::data_query(GetOpts& ops)
{
   try
   {
      _dataMap.query(ops,_tm);
   }
   catch (DataMap::NoSelect)
   {
      throw CommandError("query","no data object selected.");
   }
}



/// @brief Executes analytic.
///
/// Executes the catch all analytic command, using the first argument as the
/// name of the analytic.
///
/// @param ops Command line object with options.
///
/// @exception CommandError Something has happened which prevents completion of
/// the analytic, such as a parsing error. Exception specifies error detected.
void Console::analytic(GetOpts& ops)
{
   using aptr = std::unique_ptr<Analytic>;
   using ilist = std::forward_list<Data*>;
   aptr a(_factory.build_analytic(ops.com_front()));
   if (!a)
   {
      std::ostringstream buffer;
      buffer << "cannot find analytic type '" << ops.com_front() << "'.";
      throw CommandError("analytic",buffer.str());
   }
   bool willForce {ops.has_opt("force",true)};
   ilist ins;
   for (auto i = ops.begin();i!=ops.end();)
   {
      if (i.is_key("in"))
      {
         string raw;
         try
         {
            raw = i.value<string>();
         }
         catch (GetOpts::InvalidType)
         {
            throw CommandError("analytic","syntax error in --in parameter.");
         }
         string file;
         string type;
         seperate("analytic",raw,file,type);
         Data* d = _dataMap.find(file);
         if (d==nullptr)
         {
            try
            {
               d = _dataMap.open(file,type);
            }
            catch (DataMap::InvalidType)
            {
               std::ostringstream buffer;
               buffer << "error: '" << type << "' is not valid data type.";
               throw CommandError("analytic",buffer.str());
            }
            if (d->is_new())
            {
               CommandError("analytic","cannot set new data object as input.");
            }
         }
         ins.push_front(d);
         a->input(d);
         i = ops.erase(i);
      }
      else
      {
         ++i;
      }
   }
   for (auto i = ops.begin();i!=ops.end();)
   {
      if (i.is_key("out"))
      {
         string raw;
         try
         {
            raw = i.value<string>();
         }
         catch (GetOpts::InvalidType)
         {
            throw CommandError("analytic","syntax error in --out parameter.");
         }
         string file;
         string type;
         seperate("analytic",raw,file,type);
         Data* d = _dataMap.find(file);
         if (d==nullptr)
         {
            try
            {
               d = _dataMap.open(file,type);
            }
            catch (DataMap::InvalidType)
            {
               std::ostringstream buffer;
               buffer << "error: '" << type << "' is not valid data type.";
               throw CommandError("analytic",buffer.str());
            }
         }
         if (!d->empty()&&!willForce)
         {
            throw CommandError("analytic",
                               "cannot overwrite non-empty data object.");
         }
         File& k = *dynamic_cast<File*>(d);
         k.clear();
         k.init_history();
         k.history().init(file,ops.com_front(),ops.orig());
         for (auto i:ins)
         {
            k.history().add_child(i->history());
         }
         k.write_history();
         a->output(d);
         i = ops.erase(i);
      }
      else
      {
         ++i;
      }
   }
   if (_device)
   {
      a->init_cl(*_device);
   }
   ops.com_pop();
   a->execute(ops,_tm);
}



/// Takes a single input string and seperates it into two substrings using :
/// as the delimiter. Will also work with no delimiter, setting the first
/// substring as the whole input string and the second as empty.
///
/// @param who Name of what is using this function for if it throws a command
/// error.
/// @param raw Input string that will be seperated.
/// @param file Output string that will have first substring set to it.
/// @param type Output string that will have second substring set to it.
///
/// @exception CommandError There is more than one delimiter character in the
/// raw string.
void Console::seperate(const string& who, const string& raw, string& file,
                       string& type)
{
   auto n = raw.find(':');
   if (n!=raw.rfind(':'))
   {
      throw CommandError(who.c_str(),"syntax error detected.");
   }
   file = raw.substr(0,n);
   type.clear();
   if (n!=string::npos)
   {
      type = raw.substr(++n);
   }
}



/// @brief Recursively prints history list.
///
/// Recursively lists the history of the given history list iterators, starting
/// with begin and ending with end. Will also recusively call itself for each
/// iterator if that iterator has a child iterator.
///
/// @param begin First iterator that will be listed.
/// @param end End of list iterator that marks end of iterator list.
/// @param d Recursive depth of current list being printed.
inline void Console::rec_history(hiter begin, hiter end, int d)
{
   for (auto i = begin;i!=end;++i)
   {
      const History& h {*i};
      time_t t = h.time_stamp();
      struct tm* bt = localtime(&t);
      if (i!=begin)
      {
         print_pad(d);
         _tm << "\n";
      }
      print_pad(d);
      _tm << "Time Stamp: ";
      _tm << bt->tm_mday << "-" << (bt->tm_mon+1) << "-" << (bt->tm_year+1900)
          << " " << bt->tm_hour << ":" << bt->tm_min << "\n";
      print_pad(d);
      _tm << "File Name: " << h.file_name() << "\n";
      print_pad(d);
      _tm << "Object: " << h.object() << "\n";
      print_pad(d);
      _tm << "Command: " << h.command() << "\n";
      if (h.has_children())
      {
         print_pad(d);
         _tm << "{\n";
         rec_history(h.begin(),h.end(),d+1);
         print_pad(d);
         _tm << "{\n";
      }
   }
}



/// Prints a certain number of spaces as padding for a line of text.
///
/// @param d How many levels of padding that will be printed.
inline void Console::print_pad(int d)
{
   while (d--)
   {
      _tm << "  ";
   }
}



/// Initializes a new command error and sets who threw it and its message.
///
/// @param who Identifies who threw the error message.
/// @param msg The message describing the error that occured.
Console::CommandError::CommandError(const string& who, const string& msg):
   _who(who),
   _msg(msg)
{}



/// Prints the error message to the terminal.
///
/// @param tm The program's terminal that will be printed to.
void Console::CommandError::print(Terminal& tm)
{
   tm << _who << ": " << _msg << "\n";
}



}
