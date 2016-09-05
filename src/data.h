#ifndef ACCELCOMPENG_DATA_H
#define ACCELCOMPENG_DATA_H
#include "terminal.h"
#include "getopts.h"
#include "file.h"
namespace AccelCompEng
{



class Data : public File
{
public:
   virtual ~Data() = default;
   const std::string type() const;
   void type(const std::string& type);
   virtual void load(GetOpts& ops, Terminal& tm) = 0;
   virtual void dump(GetOpts& ops, Terminal& tm) = 0;
   virtual void query(GetOpts& ops, Terminal& tm) = 0;
   virtual bool empty() = 0;
protected:
   Data() = default;
private:
   std::string _type;
};



}
#endif
