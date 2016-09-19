#ifndef ACCELCOMPENG_LINUXTERM_H
#define ACCELCOMPENG_LINUXTERM_H
#include <string>
#include <list>
#include "terminal.h"
namespace AccelCompEng
{



/// @brief Linux OS Terminal implementation.
///
/// Implements terminal interface for user in linux terminal environment. Uses
/// system calls to get the width of the terminal and VT100 and ANSI escape
/// codes to control the terminal in raw mode to create an interactive terminal
/// environment. See https://en.wikipedia.org/wiki/ANSI_escape_code for more
/// information about control characters used.
class LinuxTerm : public Terminal
{
public:
   LinuxTerm(const LinuxTerm&) = delete;
   LinuxTerm(LinuxTerm&&) = delete;
   LinuxTerm& operator=(const LinuxTerm&) = delete;
   LinuxTerm& operator=(LinuxTerm&&) = delete;
   /// Grabs the width of the program's terminal in characters and locks terminal for exclusive use.
   LinuxTerm();
   ~LinuxTerm() override final;
   /// Grabs the terminal attributes and sets them to not echo and not be canonical. Changes
   /// terminal status to raw mode.
   static void stty_raw();
   /// Grabs the terminal attributes and sets them to echo and canoncial. Changes terminal status to
   /// cooked mode.
   static void stty_cooked();
   /// Set new header for terminal input.
   void header(const std::string&) override final;
   /// Set floating point precision.
   /// @todo Need to finish this function.
   void precision(int) override final;
   LinuxTerm& operator<<(short) override final;
   LinuxTerm& operator<<(unsigned short) override final;
   LinuxTerm& operator<<(int) override final;
   LinuxTerm& operator<<(unsigned int) override final;
   LinuxTerm& operator<<(long) override final;
   LinuxTerm& operator<<(unsigned long) override final;
   LinuxTerm& operator<<(float) override final;
   LinuxTerm& operator<<(double) override final;
   LinuxTerm& operator<<(const char*) override final;
   LinuxTerm& operator<<(const std::string&) override final;
   void operator>>(std::string&) override final;
private:
   void set_ops(Ops) override final;
   void reset_cursor(int);
   void reprint(bool);
   static constexpr char _backspaceCh {'\x7f'};
   static constexpr char _escapeCh {'\x1b'};
   static constexpr char _arrowRightCh {'C'};
   static constexpr char _arrowLeftCh {'D'};
   constexpr static const char* _cursorUpStr {"\x1b[A"};
   constexpr static const char* _boldTextStr {"\x1b[1m"};
   constexpr static const char* _normTextStr {"\x1b[0m"};
   static bool _lock;
   static bool _cooked;
   int _cols;
   int _chCount;
   std::string _header;
   std::list<char> _line;
   std::list<char>::iterator _i;
};



}
#endif
