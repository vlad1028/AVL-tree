#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

class AVL {
    struct Node {
        //        Node* parent;
        Node* left;
        Node* right;
        int value;
        std::size_t height;

        explicit Node(int value);
        //        Node(Node* parent, int value);

        ~Node();
    };
    Node* root;
    std::size_t tree_size;

    Node* rotate_left(Node* a);
    Node* rotate_right(Node* a);

    void update_height(Node* node);
    int32_t diff(Node* node) const;
    std::pair<int32_t, int32_t> get_children_height(Node* node) const;
    Node* balance(Node* node);
    Node* find_min(Node* cur) const;
    Node* delete_min(Node* node);
    std::pair<bool, Node*> insert(Node* node, int value);
    std::pair<bool, Node*> remove(Node* node, int value);
    Node* delete_node(Node* node);
    void delete_subtree(Node* node);
    void fill_with_values(std::vector<int>& v, Node* a) const;

public:
    AVL();
    explicit AVL(int root_value);
    [[nodiscard]] bool contains(int value) const noexcept;
    bool insert(int value);
    bool remove(int value);
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::vector<int> values() const noexcept;
    ~AVL();
};

#endif
