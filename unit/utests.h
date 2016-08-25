#ifndef UTESTS_H
#define UTESTS_H
#include <string>
#include <memory>
#include <vector>



class UTests
{
public:
   class Run;
   struct Fail {};
   UTests() = default;
   void attach(const std::shared_ptr<Run>& run);
   bool execute();
private:
   static int _count;
   std::vector<std::shared_ptr<Run>> _runs;
};



class UTests::Run
{
public:
   Run(const std::string& name, void (*in)(void), void (*out)(void));
   void add_test(const std::string& name, void (*func)(void));
   bool execute() const;
private:
   struct Test
   {
      std::string _name;
      void (*_func)(void);
   };
   std::string _name;
   void (*_in)(void);
   void (*_out)(void);
   std::vector<Test> _tests;
};



#endif
