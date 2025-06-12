#pragma once
#include <BST/functors/comparators.hpp>
#include <BST/node.hpp>
#include <BST/utils/traverse.hpp>
#include <cmath>
#include <cstdint>
#include <stack>
#include <stdexcept>
#include <vector>

template <typename T, typename Comparator = LessThan<T>>
class ScapeGoat {
 public:
  explicit ScapeGoat(double alpha);
  ~ScapeGoat();

  bool insert(T key);
  bool remove(T key);
  bool find(T key, Node<T>**& ptr);
  const Node<T>* getRoot();
  uint64_t getSize();
  uint64_t getMaxSize();
  double getAlpha();

 private:
  Node<T>* root_;
  Comparator cmp;
  uint64_t size_;
  uint64_t max_size_;
  const double kAlpha;
  static constexpr double kMinAlpha = 0.5;
  static constexpr double kMaxAlpha = 1.0;

  int size(Node<T>* node);
  int log_base(double base, double n) const;
  void flatten(Node<T>* node, std::vector<Node<T>*>& nodes);
  Node<T>* rebuild(std::vector<Node<T>*>& nodes, int start, int end);
  void deleteSubtree(Node<T>* node);
  Node<T>** findScapegoat(std::stack<std::pair<Node<T>**, int>>& path);
};

#include "BST/scapegoat.tpp"
