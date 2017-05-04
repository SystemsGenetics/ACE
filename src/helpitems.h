#ifndef HELPITEMS_H
#define HELPITEMS_H
#include "abstracthelpitem.h"



class CLHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "cl";
   }
   std::string getDescription() const override final
   {
      return "cl <list|info|set|clear>\n"
             "\n"
             "list - list all available opencl devices\n"
             "info n:m - get detailed information about nth platform's mth device\n"
             "set n:m - set active opencl device to nth platform's mth device\n"
             "clear - clear selected opencl device\n";
   }
};

class OpenHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "open";
   }
   std::string getDescription() const override final
   {
      return "open [file] (--select)\n"
             "\n"
             "Open a new or existing data file location at [file]. --select will make the"
             " data object selected once opened.\n";
   }
};

class CloseHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "close";
   }
   std::string getDescription() const override final
   {
      return "close [name]\n"
             "\n"
             "Closes open data object with name [name].";
   }
};

class SelectHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "select";
   }
   std::string getDescription() const override final
   {
      return "select [name]\n"
             "\n"
             "Select data object with name [name].\n";
   }
};

class ClearHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "clear";
   }
   std::string getDescription() const override final
   {
      return "clear\n"
             "\n"
             "Clears any currently selected data object.\n";
   }
};

class ListHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "list";
   }
   std::string getDescription() const override final
   {
      return "list\n"
             "\n"
             "Lists names of all currently open data objects.\n";
   }
};

class HistoryHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "history";
   }
   std::string getDescription() const override final
   {
      return "history [name]\n"
             "\n"
             "Print detailed history of data object with name [name].\n";
   }
};

class LoadHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "load";
   }
   std::string getDescription() const override final
   {
      return "load [file] (--force)\n"
             "\n"
             "Load flat text file located at [file] into currently selected data object. The data"
             " object must be empty unless the option --force is used to clear it. Additional"
             " arguments can be given for specific data types.\n";
   }
};

class ImportHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "import";
   }
   std::string getDescription() const override final
   {
      return "import [datafile] [flatfile] (--force)\n"
             "\n"
             "Open a new or existing data object located at [datafile] and load new data from flat"
             " text file located at [flatfile]. The data object must be empty unless --force is"
             " used. Additional arguments can be given for specific data types.\n";
   }
};

class DumpHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "dump";
   }
   std::string getDescription() const override final
   {
      return "dump\n"
             "\n"
             "dump currently selected data object's data to flat text file. Additional arguments"
             " can be given for specific data types.\n";
   }
};

class QueryHelpItem : public AbstractHelpItem
{
   std::string getName() const override final
   {
      return "query";
   }
   std::string getDescription() const override final
   {
      return "query\n"
             "\n"
             "query currently selected data object's data to terminal. Additional arguments"
             " can be given for specific data types.\n";
   }
};



#endif
