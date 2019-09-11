//
// Created by root on 18. 5. 13.
//

#include "RBtree.h"
#include "cstdlib"

RBtree::RBtree() {
    root = nullptr;

    nil = new tree;

    nil->parent = nullptr;
    nil->left = nullptr;
    nil->right = nullptr;
    nil->sub_size = 0;
    nil->key = -1;
    nil->is_black = NULL;

    node_count = 0;
}

RBtree::RBtree(int value) {
    root = new tree;

    root->parent = nullptr;
    root->left = nullptr;
    root->right = nullptr;
    root->sub_size = 1;
    root->key = value;
    root->is_black = true;

    node_count = 1;
}

RBtree::~RBtree()
{
    tree* bl = get_min(root);
    tree* curr = root;
    tree* del = nullptr;

    while(curr != nullptr)
    {
        for(bl->left = curr->right ; bl != nullptr && bl->left != nullptr; bl = bl->left) {
            curr->right->parent = bl;
            curr->right = nullptr;
        }

        del = curr;
        curr = curr->left;
        delete del;
    }
}

tree* RBtree::get_node(int type, tree* source) {
    tree* res = nullptr;

    switch(type)
    {
        case 0:
            res = root;
            break;

        case 1:
            res = source->left;
            break;

        case 2:
            res = source->right;
            break;

        case 3:
            res = source->parent;
            break;

        default:
            break;
    }

    return res;
}

void RBtree::set_node(int type, tree *dest, tree *ptr) {

    switch(type)
    {
        case 0:
            root = ptr;
            break;

        case 1:
            dest->left = ptr;
            break;

        case 2:
            dest->right = ptr;
            break;

        case 3:
            dest->parent = ptr;
            break;

        default:
            break;
    }
}

int RBtree::get_node_count() {
    return node_count;
}

tree* RBtree::search(int value) {
    tree* curr = root;

    while(curr != nil && curr!= nullptr)
    {
        if(curr->key > value)
            curr = curr->left;
        else if(curr->key < value)
            curr = curr->right;
        else
            return curr;
    }

    return nil;
}

tree* RBtree::get_min(tree *source) {

    tree* curr = source;
    tree* parent = nullptr;

    while(curr != nil && curr != nullptr)
    {
        parent = curr;
        curr = curr->left;
    }

    return parent;
}

int RBtree::Insert(int value) {
    tree *curr, *parent;
    tree* new_elem;

    parent = nil;
    curr = root;

    while(curr != nil && curr != nullptr)
    {
        parent = curr;

        if (curr->key > value)
            curr = curr->left;
        else if(curr->key < value)
            curr = curr->right;
        else
            return 0;
    }

    new_elem = new tree;
    new_elem->key = value;
    new_elem->sub_size = 1;
    new_elem->is_black = false;
    new_elem->parent = parent;
    new_elem->left = nil;
    new_elem->right = nil;

    if(parent == nil)
        root = new_elem;
    else if(parent->key > new_elem->key)
        parent->left = new_elem;
    else
        parent->right = new_elem;

    if(node_count != 0)
        InsertFix(new_elem);

    node_count++;

    return value;
}

int RBtree::Delete(int value) {
    tree* curr = search(value);
    tree* source = curr;
    tree* temp;

    if(curr == nil)
        return 0;

    bool source_is_black = source->is_black;

    if(curr->left == nil) {
        temp = curr->right;
        Transplant(curr, curr->right);
    }
    else if(curr->right == nil) {
        temp = curr->left;
        Transplant(curr, curr->left);
    }
    else
    {
        source = get_min(curr->right);
        source_is_black = source->is_black;

        temp = source->right;

        if(source->parent == curr) {
            temp->parent = source;
        }
        else
        {
            Transplant(source, source->right);
            source->right = curr->right;
            source->right->parent = source;
        }
        Transplant(curr, source);
        source->left = curr->left;
        source->left->parent = source;

        source->is_black = curr->is_black;
    }

    if(source_is_black)
        DeleteFix(temp);

    node_count--;
    delete curr;

    return value;
}

void RBtree::Transplant(tree *u, tree *v) {

    if(u->parent == nil)
        set_node(0,nullptr,v);
    else if(u->parent->left == u)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

void RBtree::InsertFix(tree *curr) {

    tree *uncle = nil;

    while(!(curr->parent->is_black))
    {
        if(curr->parent == curr->parent->parent->left)
        {
            uncle = curr->parent->right;

            if(!curr->is_black)
            {
                curr->parent->is_black = true;
                uncle->is_black = true;
                curr->parent->parent->is_black = false;
                curr = curr->parent->parent;
            }
            else
            {
                if(curr == curr->parent->right)
                {
                    curr = curr->parent;
                    Leftrotate(curr);
                }

                curr->parent->is_black = true;
                curr->parent->parent->is_black = false;
                Rightrotate(curr->parent->parent);
            }
        }
        else
        {
            uncle = curr->parent->left;

            if(!(curr->is_black))
            {
                curr->parent->is_black = true;
                uncle->is_black = true;
                curr->parent->parent->is_black = false;
                curr = curr->parent->parent;
            }
            else
            {
                if(curr == curr->parent->left)
                {
                    curr = curr->parent;
                    Leftrotate(curr);
                }

                curr->parent->is_black = true;
                curr->parent->parent->is_black = false;
                Rightrotate(curr->parent->parent);
            }
        }

        if(curr == nil)
            break;
    }

    root->is_black = true;
}

void RBtree::DeleteFix(tree *curr) {

    tree* sibling = nil;

    while(curr != root && curr->is_black)
    {
        if(curr == curr->parent->left)
        {
            sibling = curr->parent->right;

            if(!(sibling->is_black))
            {
                sibling->is_black = true;
                curr->parent->is_black = false;
                Leftrotate(curr->parent);
                sibling = curr->parent->right;
            }

            if(sibling->left->is_black && sibling->right->is_black)
            {
                sibling->is_black = false;
                curr = curr->parent;
            }
            else if(sibling->right->is_black)
            {
                sibling->left->is_black = true;
                sibling->is_black = false;
                Rightrotate(sibling);
                sibling = curr->parent->right;
            }

            sibling->is_black = curr->parent->is_black;
            curr->parent->is_black = true;
            sibling->right->is_black = true;
            Leftrotate(curr->parent);
            curr = root;
        }
        else
        {
            sibling = curr->parent->left;

            if(!(sibling->is_black))
            {
                sibling->is_black = true;
                curr->parent->is_black = false;
                Leftrotate(curr->parent);
                sibling = curr->parent->left;
            }

            if(sibling->left->is_black && sibling->right->is_black)
            {
                sibling->is_black = false;
                curr = curr->parent;
            }
            else if(sibling->left->is_black)
            {
                sibling->right->is_black = true;
                sibling->is_black = false;
                Rightrotate(sibling);
                sibling = curr->parent->left;
            }

            sibling->is_black = curr->parent->is_black;
            curr->parent->is_black = true;
            sibling->left->is_black = true;
            Leftrotate(curr->parent);
            curr = root;
        }

        if(curr == nil)
            break;
    }

}

void RBtree::Leftrotate(tree *curr) {
    tree* rchild = curr->right;
    curr->right = rchild->left;

    if(rchild->left != nil)
        rchild->left->parent = curr;

    rchild->parent = curr->parent;

    if(curr->parent == nil)
        root = rchild;
    else if(curr == curr->parent->left)
        curr->parent->left = rchild;
    else
        curr->parent->right = rchild;

    rchild->left = curr;
    curr->parent = rchild;
}

void RBtree::Rightrotate(tree *curr) {
    tree* lchild = curr->left;
    curr->left = lchild->right;

    if(lchild->right != nil)
        lchild->right->parent = curr;

    lchild->parent = curr->parent;

    if(curr->parent == nil)
        root = lchild;
    else if(curr == curr->parent->left)
        curr->parent->left = lchild;
    else
        curr->parent->right = lchild;

    lchild->right = curr;
    curr->parent = lchild;
}