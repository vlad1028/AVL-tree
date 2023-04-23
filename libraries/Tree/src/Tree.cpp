#include "tree/Tree.hpp"

AVL::Node::Node(int value) {
    //    this->parent = nullptr;
    this->left   = nullptr;
    this->right  = nullptr;
    this->value  = value;
    this->height = 0;
}
// AVL::Node::Node(AVL::Node* const parent, int value) : Node(value) {
//     //    this->parent = parent;
// }
AVL::Node::~Node() {
    //    parent = nullptr;
    left  = nullptr;
    right = nullptr;
}

AVL::AVL() {
    root      = nullptr;
    tree_size = 0;
}
AVL::AVL(int root_value) {
    root      = new Node(root_value);
    tree_size = 1;
}

AVL::~AVL() {
    delete_subtree(root);
}

void AVL::delete_subtree(Node* node) {
    if (!node)
        return;
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

std::pair<std::int32_t, std::int32_t> AVL::get_children_height(Node* node) const {
    std::int32_t l = -1;
    std::int32_t r = -1;
    if (node->left)
        l = static_cast<int32_t>(node->left->height);
    if (node->right)
        r = static_cast<int32_t>(node->right->height);
    return {l, r};
}

void AVL::update_height(Node* node) {
    auto children_height = get_children_height(node);
    node->height         = std::max(children_height.first, children_height.second) + 1;
}

int32_t AVL::diff(Node* const node) const {
    auto children_height = get_children_height(node);
    return children_height.second - children_height.first;
}

AVL::Node* AVL::balance(Node* node) {
    update_height(node);
    int32_t d = diff(node);
    if (d == 2) {
        if (node->right && diff(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    if (d == -2) {
        if (node->left && diff(node->left) > 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

AVL::Node* AVL::rotate_left(AVL::Node* a) {
    Node* b  = a->right;
    a->right = b->left;
    b->left  = a;

    update_height(a);
    update_height(b);

    return b;
}
AVL::Node* AVL::rotate_right(AVL::Node* a) {
    Node* b  = a->left;
    a->left  = b->right;
    b->right = a;

    update_height(a);
    update_height(b);

    return b;
}

bool AVL::empty() const noexcept {
    return tree_size == 0;
}
std::size_t AVL::size() const noexcept {
    return tree_size;
}
std::vector<int> AVL::values() const noexcept {
    std::vector<int> ans;
    ans.reserve(tree_size);
    fill_with_values(ans, root);
    return ans;
}
void AVL::fill_with_values(std::vector<int>& v, AVL::Node* const a) const {
    if (a != nullptr) {
        fill_with_values(v, a->left);
        v.push_back(a->value);
        fill_with_values(v, a->right);
    }
}

bool AVL::contains(int value) const noexcept {
    Node const* current = root;
    while (current != nullptr && current->value != value) {
        if (value < current->value)
            current = current->left;
        else
            current = current->right;
    }
    return current != nullptr;
}

AVL::Node* AVL::find_min(AVL::Node* cur) const {
    while (cur->left != nullptr)
        cur = cur->left;
    return cur;
}
AVL::Node* AVL::delete_min(AVL::Node* node) {
    if (!node->left)
        return node->right;
    node->left = delete_min(node->left);
    return balance(node);
}

bool AVL::insert(int value) {
    if (!root) {
        ++tree_size;
        root = new Node(value);
        return true;
    }
    auto res = insert(root, value);
    root     = res.second;
    if (res.first)
        ++tree_size;
    return res.first;
}

std::pair<bool, AVL::Node*> AVL::insert(Node* node, int value) {
    if (!node)
        return {true, new Node(value)};
    if (value == node->value)
        return {false, node};

    bool inserted;
    if (value < node->value) {
        auto res   = insert(node->left, value);
        inserted   = res.first;
        node->left = res.second;
    } else {
        auto res    = insert(node->right, value);
        inserted    = res.first;
        node->right = res.second;
    }
    return {inserted, balance(node)};
}

bool AVL::remove(int value) {
    auto res = remove(root, value);
    root     = res.second;
    if (res.first)
        --tree_size;
    return res.first;
}

std::pair<bool, AVL::Node*> AVL::remove(Node* node, int value) {
    if (!node)
        return {false, nullptr};
    bool removed;
    if (value < node->value) {
        auto res   = remove(node->left, value);
        removed    = res.first;
        node->left = res.second;
    } else if (value > node->value) {
        auto res    = remove(node->right, value);
        removed     = res.first;
        node->right = res.second;
    } else {
        return {true, delete_node(node)};
    }
    return {removed, balance(node)};
}
AVL::Node* AVL::delete_node(AVL::Node* node) {
    Node* l = node->left;
    Node* r = node->right;
    delete node;

    if (!r)
        return l;
    Node* m  = find_min(r);
    m->right = delete_min(r);
    m->left  = l;
    return balance(m);
}
