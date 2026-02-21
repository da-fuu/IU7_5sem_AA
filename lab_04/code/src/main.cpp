#include "ui.h"
#include <iostream>

int main(int argc, char *argv[]) {
#if defined(MODE_TUI)
  runTUI();
#elif defined(MODE_CLI)
  runCLI(argc, argv);
#else
  std::cerr
      << "Error: Mode not selected. Compile with -D MODE_TUI or -D MODE_CLI.\n";
  return 1;
#endif
  return 0;
}
