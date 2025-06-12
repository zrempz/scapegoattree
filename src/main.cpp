#include <iostream>
#include "BST/scapegoat.hpp"

int main() {
  ScapeGoat<int> sgtree(2.f / 3.f);
  bool t;
  t = sgtree.insert(1);
  t = sgtree.insert(20);
  t = sgtree.insert(6);
  t = sgtree.insert(2);
  t = sgtree.remove(4);
  std::cout << t << "\n";
}
