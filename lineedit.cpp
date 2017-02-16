#include "lineedit.hpp"

TextEdit::TextEdit() : currentLine(0), totalLines(0), quit(false) {
  head = new LinkedList;
}

TextEdit::~TextEdit() {
  LinkedList *next = head, *del;
  while (next) {
    del = next;
    next = next->next;
    delete (del);
  }
}

bool TextEdit::readFile(const char *name) {
  FILE *fp;
  char buffer[MAX];
  fp = fopen(name, "r");

  if (!fp) {
    std::cout << "Invalid File\n";
    return false;
  }

  LinkedList *cur = head;
  while (fgets(buffer, 1000, fp) != NULL) {
    LinkedList *newnode = new LinkedList;
    memcpy(newnode->data, buffer, MAX);
    newnode->next = NULL;
    cur->next = newnode;
    cur = newnode;
    totalLines++;
  }
  printFile();
  currentLine = totalLines;
  std::cout << "Current Line: " << currentLine << std::endl;
  fclose(fp);
  return true;
}

void TextEdit::printFile() {
  int counter = 1;
  LinkedList *cur = head->next;
  while (cur) {
    std::cout << counter << ":";
    if (strncmp(cur->data, "", MAX) == 0) {
      std::cout << cur->data << std::endl;
    } else {
      std::cout << cur->data;
    }
    cur = cur->next;
    counter++;
  }
}

void TextEdit::insert(char *text, int line) {
  LinkedList *cur = head, *next = head->next;

  if (line <= totalLines) {
    std::cout << "<=\n";
    for (int i = 1; i < line; i++) {
      cur = next;
      next = next->next;
    }
    totalLines++;
    currentLine = line;
  }

  else {
    std::cout << ">\n";
    for (int i = 1; i < totalLines; i++) {
      cur = next;
      next = next->next;
    }

    std::cout << "need to add: " << line - totalLines << std::endl;
    for (int i = 0; i < line - totalLines - 1; ++i) {
      LinkedList *newnode = new LinkedList;
      cur->next = newnode;
      newnode->next = NULL;
      cur = newnode;
    }
    next = cur->next;
    currentLine = line;
    totalLines += line - totalLines;
  }

  LinkedList *newnode = new LinkedList;
  memcpy(newnode->data, text, MAX);
  newnode->next = next;
  cur->next = newnode;

  printFile();
  return;
}

void TextEdit::replace(char *text, int line) {
  std::cout << "Replacing line: " << line << " with: " << text << std::endl;
  LinkedList *cur = head->next;
  for (int i = 1; i < line; i++) {
    cur = cur->next;
  }
  memcpy(cur->data, text, MAX);
  printFile();
}

void TextEdit::del(int n, int m) {
  LinkedList *cur = head->next, *next, *prev = head;
  // do some garb cleanup maybe lol

  LinkedList *singleDel;
  if (m == 0) {
    if (n == 1) {  // special case adjust head
      singleDel = cur;
      head->next = cur->next;
      delete singleDel;
    }
    // go to n then delete
    else {
      for (int i = 1; i < n; i++) {
        prev = prev->next;
        cur = cur->next;
        next = cur->next;
      }
      prev->next = next;
      singleDel = cur;
      delete singleDel;
    }
    totalLines--;
    currentLine--;
  } else {  // del n to m inclusive
    LinkedList *delPtr[m - n + 1];
    int ctr = 0;

    for (int i = 1; i < n; i++) {
      prev = prev->next;
      cur = cur->next;
      next = cur->next;
    }

    for (int j = 0; j < m - n + 1; j++) {
      delPtr[ctr] = cur;
      cur = cur->next;
      ctr++;
    }

    prev->next = cur;  // points to before delete -> after;

    for (int k = 0; k < m - n + 1; k++) {
      delete delPtr[k];
    }
    totalLines -= m - n + 1;
    currentLine -= m - n + 1;
  }
}

void TextEdit::printLine(int n, int m) {
  std::cout << "afdssdf: " << n << ":" << m << std::endl;
  LinkedList *cur = head->next;
  for (int i = 1; i < n; ++i) {
    cur = cur->next;
  }
  if (m == 0) {  // single line
    std::cout << cur->data;
  } else {
    std::cout << "fdskfsdkfs\n";
    for (int i = n; i < m; ++i) {
      if (strncmp(cur->data, "", MAX) == 0) {
        std::cout << n << ":" << cur->data << std::endl;
      } else {
        std::cout << n << ":" << cur->data;
      }
      cur = cur->next;
    }
  }
}

bool TextEdit::command(char *cmd) {
  char *token;

  token = strtok(cmd, " ");

  if (strncmp(token, "I", 1) == 0) {
    char *token1, *token2;
    token1 = strtok(NULL, " ");
    token2 = strtok(NULL, " ");

    if (token2 == NULL) {  // only one param insert before curr line
      insert(token1, currentLine);
      return true;
    } else {  // go to line then insert
      insert(token2, atoi(token1));
      return true;
    }
  }

  else if (strncmp(token, "D", 1) == 0) {
    int n, m;
    token = strtok(NULL, " ");
    if (token == NULL) {  // delete current line
      del(currentLine, 0);
      return true;
    }
    n = atoi(token);
    if (n > totalLines) {
      std::cout << "Error: n > totalLines\n";
      return false;
    }
    token = strtok(NULL, " ");
    if (token == NULL) {  // delete line n
      del(n, 0);
      return true;
    }
    m = atoi(token);
    if (n > m || m == 0 || n == 0) {
      std::cout << "Error: n>m || m == 0 || n == 0\n";
      return false;
    }
    // delete line n to m
    del(n, m);
    return true;
  } else if (strncmp(token, "V", 1) == 0) {
    printFile();
    return true;
  } else if (strncmp(token, "G", 1) == 0) {
    token = strtok(NULL, " ");
    if (token == NULL) {  // go to first line
      currentLine = 1;
      return true;
    } else {  // go to line n
      currentLine = atoi(token);
      return true;
    }
  } else if (strncmp(token, "L", 1) == 0) {
    int n, m;
    token = strtok(NULL, " ");
    if (token == NULL) {  // display content of current line
      printLine(currentLine, 0);
      return true;
    }
    n = atoi(token);
    if (n > totalLines) {
      std::cout << "Value > TotalLines\n";
      return false;
    }
    token = strtok(NULL, " ");
    if (token == NULL) {  // display line n
      printLine(n, 0);
      return true;
    }
    m = atoi(token);
    if (n > m || m == 0 || n == 0) {
      std::cout << "Error: n>m || m == 0 || n == 0\n";
      return false;
    }
    // display line n to m
    printLine(n, m);
    return true;
  } else if (strncmp(token, "S", 1) == 0) {
    token = strtok(NULL, " ");
    // prompt for line

    if (token != NULL && atoi(token) > totalLines) {
      return false;
    }
    char buffer[MAX];
    std::cout << "Enter replacement string:";
    fgets(buffer, MAX, stdin);
    std::cout << "You Entered: " << buffer << std::endl;

    if (token == NULL) {
      replace(buffer, currentLine);
      // replace current line with this
      // call V w/ current line #
      return true;
    }

    else {
      replace(buffer, atoi(token));
      // replace  line n with this
      // call v with line n
      return true;
    }
  } else if (strncmp(token, "E", 1) == 0) {
    // save and exit write line by line to fileOut;
    FILE *fp;
    fp = fopen(fileOut, "w");
    std::cout << "Saving to:" << fileOut << "===" << std::endl;
    if (!fp) {
      std::cout << "Error making output file!\n";
      return false;
    }
    LinkedList *cur = head->next;

    while (cur) {
      if (strncmp(cur->data, "", 1000) != 0) {
        fputs(cur->data, fp);
      } else {
        fputs("\n", fp);
      }
      cur = cur->next;
    }
    // iterate thru linked list via head and insert line
    fclose(fp);
    quit = true;
    std::cout << "Closing\n";
    return true;
  } else if (strncmp(token, "Q", 1) == 0) {
    quit = true;
    return true;
  } else {
    return false;
  }

  return false;
}
