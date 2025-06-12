template <typename T, typename Comparator>
ScapeGoat<T, Comparator>::ScapeGoat(double alpha)
    : root_(nullptr), size_(0), max_size_(0), kAlpha(alpha) {
  if (alpha <= kMinAlpha || alpha >= kMaxAlpha)
    throw std::invalid_argument("Alpha must be in (0.5, 1.0)");
}

template <typename T, typename Comparator>
ScapeGoat<T, Comparator>::~ScapeGoat() {
  deleteSubtree(root_);
}

template <typename T, typename Comparator>
int ScapeGoat<T, Comparator>::log_base(double base, double n) const {
  return static_cast<int>(std::floor(std::log(n) / std::log(base)));
}

template <typename T, typename Comparator>
bool ScapeGoat<T, Comparator>::find(T key, Node<T>**& ptr) {
  ptr = &root_;
  while (*ptr) {
    if (key == (*ptr)->key_)
      return true;

    int dir = cmp(key, (*ptr)->key_);
    ptr = &(*ptr)->children_[dir];
  }
  return false;
}

template <typename T, typename Comparator>
bool ScapeGoat<T, Comparator>::insert(T key) {
  std::stack<std::pair<Node<T>**, int>> path;
  Node<T>** current = &root_;

  while (*current) {
    if (key == (*current)->key_)
      return false;

    int dir = cmp(key, (*current)->key_);
    path.push({current, dir});
    current = &(*current)->children_[dir];
  }

  *current = new Node<T>(key);
  ++size_;
  max_size_ = std::max(max_size_, size_);

  const int h_alpha = log_base(1.0 / kAlpha, max_size_);

  if (static_cast<int>(path.size()) <= h_alpha)
    return true;

  Node<T>** scapegoat = findScapegoat(path);
  if (scapegoat) {
    std::vector<Node<T>*> nodes;
    flatten(*scapegoat, nodes);
    *scapegoat = rebuild(nodes, 0, nodes.size() - 1);
  }
  return true;
}

template <typename T, typename Comparator>
bool ScapeGoat<T, Comparator>::remove(T key) {
  Node<T>** ptr;
  if (!find(key, ptr))
    return false;

  if ((*ptr)->children_[0] && (*ptr)->children_[1]) {
    Node<T>** successor_node = ptr;
    successor(successor_node);
    (*ptr)->key_ = (*successor_node)->key_;
    ptr = successor_node;
  }

  Node<T>* temp = *ptr;
  int dir = temp->children_[1] ? 1 : 0;
  *ptr = temp->children_[dir];
  delete temp;
  --size_;

  if (size_ < kAlpha * max_size_) {
    std::vector<Node<T>*> nodes;
    flatten(root_, nodes);
    root_ = rebuild(nodes, 0, nodes.size() - 1);
    max_size_ = size_;
  }
  return true;
}

template <typename T, typename Comparator>
void ScapeGoat<T, Comparator>::flatten(Node<T>* node,
                                       std::vector<Node<T>*>& nodes) {
  inorder(node, [&nodes](Node<T>* n) {
    n->children_[0] = n->children_[1] = nullptr;
    nodes.push_back(n);
  });
}

template <typename T, typename Comparator>
Node<T>* ScapeGoat<T, Comparator>::rebuild(std::vector<Node<T>*>& nodes,
                                           int start, int end) {
  if (start > end)
    return nullptr;

  int mid = (start + end) / 2;
  Node<T>* root = nodes[mid];

  root->children_[0] = rebuild(nodes, start, mid - 1);
  root->children_[1] = rebuild(nodes, mid + 1, end);

  return root;
}

template <typename T, typename Comparator>
int ScapeGoat<T, Comparator>::size(Node<T>* node) {
  if (!node)
    return 0;
  return 1 + size(node->children_[0]) + size(node->children_[1]);
}

template <typename T, typename Comparator>
void ScapeGoat<T, Comparator>::deleteSubtree(Node<T>* node) {
  if (!node)
    return;
  deleteSubtree(node->children_[0]);
  deleteSubtree(node->children_[1]);
  delete node;
}

template <typename T, typename Comparator>
Node<T>** ScapeGoat<T, Comparator>::findScapegoat(
    std::stack<std::pair<Node<T>**, int>>& path) {
  while (!path.empty()) {
    auto [node_ptr, dir] = path.top();
    path.pop();

    Node<T>* node = *node_ptr;
    int s_total = size(node);
    int s_child = size(node->children_[dir]);

    if (s_child > kAlpha * s_total)
      return node_ptr;
  }
  return nullptr;
}

template <typename T, typename Comparator>
const Node<T>* ScapeGoat<T, Comparator>::getRoot() {
  return root_;
}

template <typename T, typename Comparator>
uint64_t ScapeGoat<T, Comparator>::getSize() {
  return size_;
}

template <typename T, typename Comparator>
uint64_t ScapeGoat<T, Comparator>::getMaxSize() {
  return max_size_;
}

template <typename T, typename Comparator>
double ScapeGoat<T, Comparator>::getAlpha() {
  return kAlpha;
}
