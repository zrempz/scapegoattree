#pragma once

template <typename T>
class Node {
 public:
  T key_;
  Node* children_[2];

  explicit Node(T key) noexcept : key_(key) {
    children_[0] = children_[1] = nullptr;
  }

  ~Node() = default;
};
