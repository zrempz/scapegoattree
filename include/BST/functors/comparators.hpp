#pragma once

template <typename T>
class LessThan {
 public:
  inline bool operator()(T x, T y) { return x < y; }
};
