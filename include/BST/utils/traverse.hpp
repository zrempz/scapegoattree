#pragma once
#include <BST/node.hpp>

template <typename T, typename Func>
void inorder(Node<T>* root, Func&& fn) {
  if (!root)
    return;

  inorder(root->children_[0], fn);
  fn(root);
  inorder(root->children_[1], fn);
}

template <typename T>
void successor(Node<T>**& q) {
  if (!*q)
    return;

  q = &(*q)->children_[1];
  while (*q && (*q)->children_[0])
    q = &(*q)->children_[0];
}
