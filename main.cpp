#include "lineedit.hpp"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Error requires 3 arguments EDIT filein fileout\n";
    return 0;
  }

  if (strncmp(argv[1], "EDIT", 4) != 0) {
    std::cout << "Error syntax is ./prog EDIT filein fileout\n";
    return 0;
  }

  TextEdit edit;
  bool check = edit.readFile(argv[2]);
  memcpy(edit.fileOut, argv[3], 100);  // assume max file name = 100 chars

  if (!check) {
    std::cout << "Error invalid file\n";
    return 0;
  }

  char buffer[100];

  while (!edit.quit) {
    fgets(buffer, 100, stdin);
    if (!edit.command(buffer)) {
      std::cout << "Invalid command try again!\n";
      continue;
    }
    if (!edit.quit) {
      std::cout << "Current Line: " << edit.currentLine << std::endl;
    }
  }

  return 0;
}
