#ifndef LINEEDIT_HPP
#define LINEEDIT_HPP

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

#define MAX 1000

typedef struct LinkedList {
  char data[MAX] = {0};
  LinkedList *next = NULL;
} LinkedList;

class TextEdit {
 public:
  LinkedList *head;
  int currentLine, totalLines;
  char fileOut[100];
  bool quit;

  TextEdit();
  ~TextEdit();
  bool readFile(const char *name);
  void printFile();
  void insert(char *text, int line);
  void replace(char *text, int line);
  void del(int n, int m);
  void printLine(int n, int m);
  bool command(char *cmd);
};

#endif  // LINEEDIT_HPP
