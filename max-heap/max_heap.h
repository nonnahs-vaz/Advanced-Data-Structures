#include "node.h"
typedef struct
{
    node *head;
    node *last_node;

    void init_heap();
    node *create_node(int value);
    void insert(int value);
    node *find_incomplete_node();
    void sift_up(node *child);
    void delete_max();
    node *find_largest_child(node *parent);
    void delete_last_node();
    void sift_down(node *parent);
    void resolve_relationships(node *new_node);
    void display();
    void inorder(node *root);
    void swap(node *a, node *b);
    void update_parent_of_last_node();
} heap;
