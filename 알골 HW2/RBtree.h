//
// Created by root on 18. 5. 13.
//

#ifndef HW2_RBTREE_H
#define HW2_RBTREE_H

typedef struct tree {
    int key;
    int sub_size;
    bool is_black;
    struct tree* parent;
    struct tree* left;
    struct tree* right;
} tree;

class RBtree {
public:
    RBtree();
    RBtree(int value);
    ~RBtree();

    void malloc_tree(tree* ptr, bool erase);

    tree* get_node(int type, tree* source);
    void set_node(int type, tree* dest, tree* ptr);

    tree* search(int value);

    tree* get_min(tree* source);

    int Insert(int value);
    int Delete(int value);

    void Transplant(tree* u, tree* v);

    void InsertFix(tree* curr);
    void DeleteFix(tree* curr);

    void Leftrotate(tree* curr);
    void Rightrotate(tree* curr);

    int get_node_count();

    private:
    struct tree* root;
    struct tree* nil;
    int node_count;

};

#endif //HW2_RBTREE_H
