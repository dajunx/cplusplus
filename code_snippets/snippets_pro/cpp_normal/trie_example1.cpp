// Trie树 实现，用后续遍历所有节点
// url:http://www.cnblogs.com/dlutxm/archive/2011/10/26/2225660.html

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define Max 26
using namespace std;

typedef struct node {
  struct node *next[Max];
  int num;
} Node;
//创建一个新节点
Node *createNew() {
  Node *p = new Node;
  for (int i = 0; i < Max; i++) {
    p->next[i] = NULL;
  }
  p->num = 0;
  return p;
}
//插入一个字符串
void Insert_str(const char str[], Node *head) {
  int len = strlen(str);
  Node *t, *p = head;
  for (int i = 0; i < len; i++) {
    int c = str[i] - 'a';
    if (p->next[c] == NULL) {
      t = createNew();
      p->next[c] = t;
      p->num++;
      // cout<<p->num<<endl;
      p = p->next[c];
    } else {
      p = p->next[c];
    }
  }
}
int Search_str(char str[], Node *head) {
  Node *p = head;
  int len = strlen(str);
  int count = 0;
  for (int i = 0; i < len; i++) {
    int c = str[i] - 'a';
    if (p->next[c] == NULL) {
      cout << "不存在字符串" << endl;
      count = 0;
      return -1;
    } else {
      p = p->next[c];
      count += p->num;
    }
  }
  return count;
}

//后续遍历元素
std::vector<char> vec_char;
void print_all_node(Node *head) {
  Node *p = head;
  for (int i = 0; i < Max; ++i) {
    if (head->next[i]) {
      print_all_node(head->next[i]);
      vec_char.push_back(static_cast<char>(i + 'a'));
    }
  }
}

int main() {
  Node *head = createNew();

  std::ifstream filein("word.txt");
  if (!filein.is_open()) {
    return -1;
  }

  for (std::string line; std::getline(filein, line);) {
    Insert_str(line.c_str(), head);
  }

  int c = Search_str("ab", head);
  cout << c << endl;

  print_all_node(head);

  return 0;
}