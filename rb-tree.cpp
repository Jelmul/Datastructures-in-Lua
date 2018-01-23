#include <iostream>
#include <iomanip>
#include "rb-tree.h"

using namespace std;

void RBtree::insert(int i)
{
    node *n = new node;
    n->key = i;
    n->left = NULL;
    n->right = NULL;

    if(root == NULL)
    {
        root = n;
        n->parent = NULL;
    }
    else
    {
        node *p = root;
        node *p_parent = NULL;
        while (p != NULL){
            p_parent = p;
            if(p->key < i){
                p = p->right;
            } else {
                p = p->left;
            }
        }
        n->parent = p_parent;
        n->color = 'r';
        if(p_parent->key < n->key) {
            p_parent->right = n;
        } else {
            p_parent->left = n;
        }
    }
    size++;
    insertfix(n);
}

void RBtree::insertfix(node *z)
{
    while(z != root && z->parent->color == 'r')
    {
        if(z->parent == z->parent->parent->left)
        {
            node *g = z->parent->parent;
            node *y = g->right;
            if(y !=NULL && y->color == 'r')
            {
                z->parent->color = 'b';
                y->color = 'b';
                g->color = 'r';
                z = g;
            }
            else
            {
                if(z->parent->right == z)
                {
                    z = z->parent;
                    leftrotate(z);
                }

                z->parent->color = 'b';
                g->color = 'r';
                rightrotate(g);
            }
        }
        else
        {
            node *g = z->parent->parent;
            node *y = g->left;

            if(y !=NULL && y->color == 'r')
            {
                z->parent->color = 'b';
                y->color = 'b';
                g->color = 'r';
                z = g;
            }
            else
            {
                if(z->parent->left == z)
                {
                    z = z->parent;
                    rightrotate(z);
                }

                z->parent->color = 'b';
                g->color = 'r';
                leftrotate(g);
            }
        }
    }
    root->color='b';
}

void RBtree::del(int i)
{
    if(root == NULL)  return;

    node *p;
    p = root;
    node *y = NULL;
    node *q = NULL;
    bool found = false;
    while(p != NULL && !found){
        if(p->key == i){
            found = true;
            size--;
        }

        if(!found){
            if(p->key < i)
                p = p->right;
            else
                p = p->left;
        }
    }
    if(!found)  return;

    if(p->left == NULL || p->right == NULL)
        y = p;
    else
        y = successor(p);
    if(y->left != NULL){
        q = y->left;
    }
    else
    {
        if(y->right != NULL){
            q = y->right;
        }
        else{
            q = NULL;
        }
    }
    if(q != NULL){
        q->parent = y->parent;
    }
    if(y->parent == NULL){
        root = q;
    } else {
        if(y == y->parent->left){
            y->parent->left = q;
        } else {
            y->parent->right = q;
        }
    }
    if(y != p){
        p->color = y->color;
        p->key = y->key;
    }
    if(y->color == 'b'){
        delfix(p);
    }
}

void RBtree::delfix(node *p)
{
    node *s;
    while(p != root && p->color == 'b')
    {
        if(p->parent->left == p){
            s = p->parent->right;
            if(s->color == 'r'){
                s->color = 'b';
                p->parent->color = 'r';
                leftrotate(p->parent);
                s = p->parent->right;
            }
            if(s->right->color == 'b' && s->left->color == 'b'){
                s->color = 'r';
                p = p->parent;
            } else {
                if(s->right->color == 'b') {
                    s->left->color = 'b';
                    s->color = 'r';
                    rightrotate(s);
                    s = p->parent->right;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->right->color = 'b';
                leftrotate(p->parent);
                p=root;
            }
        } else {
            s = p->parent->left;
            if(s->color == 'r') {
                s->color = 'b';
                p->parent->color = 'r';
                rightrotate(p->parent);
                s = p->parent->left;
            }
            if(s->left->color == 'b' && s->right->color == 'b')
            {
                s->color = 'r';
                p = p->parent;
            } else {
                if(s->left->color == 'b') {
                    s->right->color = 'b';
                    s->color = 'r';
                    leftrotate(s);
                    s = p->parent->left;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->left->color = 'b';
                rightrotate(p->parent);
                p = root;
            }
        }
        p->color = 'b';
        root->color = 'b';
    }
}

void RBtree::leftrotate(node *p)
{
    if(p->right == NULL)
        return ;
    else
    {
        node *y = p->right;
        if(y->left != NULL){
            p->right = y->left;
            y->left->parent = p;
        } else {
            p->right = NULL;
        }
        if(p->parent != NULL){
            y->parent = p->parent;
        }
        if(p->parent == NULL){
            root = y;
            y->parent = NULL;
        } else {
            if(p == p->parent->left){
                p->parent->left = y;
            } else{
                p->parent->right = y;
            }
        }
        y->left = p;
        p->parent = y;
    }
}
void RBtree::rightrotate(node *p)
{
    if(p->left == NULL)
        return ;
    else
    {
        node *y = p->left;
        if(y->right != NULL) {
            p->left = y->right;
            y->right->parent = p;
        } else {
            p->left = NULL;
        }
        if(p->parent != NULL) {
            y->parent = p->parent;
        }
        if(p->parent == NULL){
            root = y;
        } else {
            if(p == p->parent->left){
                p->parent->left = y;
            } else {
                p->parent->right = y;
            }
        }
        y->right = p;
        p->parent = y;
    }
}

node* RBtree::successor(node *p)
{
    node *y = NULL;
    if(p->left != NULL){
        y = p->right;
        while(y->left != NULL)
            y = y->left;
    }
    else{
        y = p->left;
        while(y->right != NULL)
            y = y->right;
    }
    return y;
}

bool RBtree::search(int x)
{
    if(root == NULL)  return false;
    node *p = root;
    bool found = false;
    while(p != NULL && !found)
    {
        if(p->key == x)
            return true;
        if(!found)
        {
            if(p->key > x){
                p = p->left;
            } else {
                p = p->right;
            }
        }
    }
    return false;
}

void postorder(node* p, int indent)
{
    if(p != NULL) {
        if(p->left) postorder(p->left, indent+4);
        if(p->right) postorder(p->right, indent+4);
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        cout<< p->key << "\n ";
    }
}
