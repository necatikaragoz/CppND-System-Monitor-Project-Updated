#include "ncurses_display.h"
#include "system.h"
#include "iostream"
#include "linux_parser.h"
#include "TestCls.h"

int main() {
  //TestCls testObj;
  System system;
  NCursesDisplay::Display(system);
}