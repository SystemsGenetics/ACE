#ifndef UNIT_H
#define UNIT_H
#include <iostream>
#include "../src/ace.h"
#include "utests.h"



namespace ace = AccelCompEng;



void add_getopts(UTests&);
void add_nvmemory(UTests&);
void add_fstring(UTests&);
void add_history(UTests&);
void add_file(UTests&);
void add_datamap(UTests&);



class FakeData : public ace::Data
{
public:
   FakeData() = default;
   void init() override final {}
   void load(ace::GetOpts&,ace::Terminal&) override final
   {
      if (touched)
      {
         throw int(0);
      }
      touched = true;
   }
   void dump(ace::GetOpts&,ace::Terminal&) override final
   {
      if (touched)
      {
         throw int(1);
      }
      touched = true;
   }
   void query(ace::GetOpts&,ace::Terminal&) override final
   {
      if (touched)
      {
         throw int(2);
      }
      touched = true;
   }
   bool empty() override final { return true; }
   bool touched {false};
};



class AceTestFactory : public ace::Factory
{
   ace::Analytic* build_analytic(const std::string&) override final { return nullptr; }
   ace::Data* build_data(const std::string& type) override final {
      ace::Data* ret {nullptr};
      if (type==std::string("FakeData"))
      {
         ret = new FakeData;
      }
      return ret;
   }
};



#endif
