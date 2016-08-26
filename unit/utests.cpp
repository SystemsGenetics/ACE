#include "utests.h"
#include <iostream>



int UTests::_count {0};



void UTests::attach(const std::shared_ptr<Run>& run)
{
   _runs.push_back(run);
}



bool UTests::execute()
{
   bool ret {true};
   std::cout << "Running unit tests" << std::flush;
   for (auto i = _runs.begin();i!=_runs.end();++i)
   {
      if (!(*i)->execute())
      {
         ret = false;
         break;
      }
   }
   if (ret)
   {
      std::cout << "\n" << _count << " unit test(s) passed.\n";
   }
   return ret;
}



UTests::Run::Run(const std::string& name, void (*in)(void), void (*out)(void)):
   _name(name),
   _in(in),
   _out(out)
{}



void UTests::Run::add_test(const std::string& name, void (*func)(void))
{
   _tests.push_back({name,func});
}



bool UTests::Run::execute() const
{
   bool ret {true};
   if (_in)
   {
      _in();
   }
   try
   {
      for (auto i = _tests.begin();i!=_tests.end();++i)
      {
         std::cout << "." << std::flush;
         ++UTests::_count;
         try
         {
            i->_func();
         }
         catch (Fail)
         {
            std::cout << "\n" << _name << "::" << i->_name << " FAILED!" << std::endl;
            ret = false;
            break;
         }
      }
   }
   catch (...)
   {
      if (_out)
      {
         _out();
      }
      throw;
   }
   if (_out)
   {
      _out();
   }
   return ret;
}
