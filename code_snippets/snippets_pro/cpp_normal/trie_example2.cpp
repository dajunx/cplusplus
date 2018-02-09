// url:http://blog.csdn.net/hguisu/article/details/8131559

#include "stdlib.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define NULL 0

const int num_chars = 26;
class Trie {
public:
  Trie() : root(new Trie_node) {}
  virtual ~Trie() {}
  int trie_search(const char *word, char *entry) const {
    char char_code;

    Trie_node *location = root; //从根结点开始
    while (location != NULL && *word != 0) {
      if (*word >= 'A' && *word <= 'Z') {
        char_code = *word - 'A';
      } else if (*word >= 'a' && *word <= 'z') {
        char_code = *word - 'a';
      } else {
        return 0;
      } // 不合法的单词
      //转入相应分支指针
      location = location->branch[char_code];
      word++;
    }
    //找到，获取数据，成功返回
    if (location != NULL && location->data != NULL) {
      strcpy(entry, location->data);
      return 1;
    } else {
      return 0;
    } // 不合法的单词
  }

  void insert(const char *word, const char *entry) {
    char char_code;
    Trie_node *location = root; //从根结点开始
    while (location != NULL && *word != 0) {
      if (*word >= 'A' && *word <= 'Z') {
        char_code = *word - 'A';
      } else if (*word >= 'a' && *word <= 'z') {
        char_code = *word - 'a';
      } else {
        return;
      }

      //不存在此分支
      if (location->branch[char_code] == NULL) {
        location->branch[char_code] = new Trie_node; //创建空分支
      }

      //转入分支
      location = location->branch[char_code];
      word++;
    }

    if (location->data == NULL) { //插入数据
      location->data = new char[strlen(entry) + 1];
      strcpy(location->data, entry); //给data赋值表明单词存在
    }
  }
  int remove(const char *word, char *entry);

protected:
  struct Trie_node {
    char *data; //若不为空，表示从root到此结点构成一个单词
    Trie_node *branch[num_chars]; //分支
    Trie_node() {
      data = NULL;
      for (int i = 0; i < num_chars; ++i) {
        branch[i] = NULL;
      }
    }
  };

  Trie_node *root; //根结点(指针)
};

int main() {
  Trie t;
  char entry[100];
  t.insert("a", "DET");
  t.insert("abacus", "NOUN");
  t.insert("abalone", "NOUN");
  t.insert("abandon", "VERB");
  t.insert("abandoned", "ADJ");
  t.insert("abashed", "ADJ");
  t.insert("abate", "VERB");
  t.insert("this", "PRON");
  if (t.trie_search("this", entry)) {
    cout << "'this' was found. pos: " << entry << endl;
  }
  if (t.trie_search("abate", entry)) {
    cout << "'abate' is found. pos: " << entry << endl;
  }
  if (t.trie_search("baby", entry)) {
    cout << "'baby' is found. pos: " << entry << endl;
  } else {
    cout << "'baby' does not exist at all!" << endl;
  }
}