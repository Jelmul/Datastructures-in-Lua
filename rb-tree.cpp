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
        n->color = BLACK;
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
        n->color = RED;
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
    while(z != root && z->parent->color == RED)
    {
        if(z->parent == z->parent->parent->left)
        {
            node *g = z->parent->parent;
            node *y = g->right;
            if(y !=NULL && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                g->color = RED;
                z = g;
            }
            else
            {
                if(z->parent->right == z)
                {
                    z = z->parent;
                    leftrotate(z);
                }

                z->parent->color = BLACK;
                g->color = RED;
                rightrotate(g);
            }
        }
        else
        {
            node *g = z->parent->parent;
            node *y = g->left;

            if(y !=NULL && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                g->color = RED;
                z = g;
            }
            else
            {
                if(z->parent->left == z)
                {
                    z = z->parent;
                    rightrotate(z);
                }

                z->parent->color = BLACK;
                g->color = RED;
                leftrotate(g);
            }
        }
    }
    root->color=BLACK;
}

void RBtree::del(int x)
{
     if(root == NULL)
     {
           return ;
     }
     node *p;
     p = root;
     node *y = NULL;
     node *q = NULL;
     int found = 0;
     while(p != NULL && found == 0)
     {
           if(p->key == x)
               found = 1;
               // Where is the size decrease?
           if(found == 0)
           {
                 if(p->key < x)
                    p = p->right;
                 else
                    p = p->left;
           }
     }
     if(found == 0){
        return ;
    }
     else
     {
         if(p->left == NULL || p->right == NULL)
              y = p;
         else
              y = successor(p);
         if(y->left != NULL)
              q = y->left;
         else
         {
              if(y->right != NULL)
                   q = y->right;
              else
                   q = NULL;
         }
         if(q != NULL)
              q->parent = y->parent;
         if(y->parent == NULL)
              root = q;
         else
         {
             if(y == y->parent->left)
                y->parent->left = q;
             else
                y->parent->right = q;
         }
         if(y != p)
         {
             p->color = y->color;
             p->key = y->key;
         }
         if(y->color ==BLACK)
             delfix(q);
        size--;
     }
}

void RBtree::delfix(node *p)
{
    node *s;
    while(p != NULL && p != root && p->color == BLACK)
    {
        if(p->parent->left == p){
            s = p->parent->right;
            if(s != NULL && s->color == RED){
                s->color = BLACK;
                p->parent->color = RED;
                leftrotate(p->parent);
                s = p->parent->right;
            }
            if(s == NULL){
                break;
            }
            if((s->right == NULL || s->right->color == BLACK) && (s->left == NULL || s->left->color == BLACK)){
                s->color = RED;
                p = p->parent;
            } else {
                if(s->right == NULL || s->right->color == BLACK) {
                    if (s->left != NULL )
                        s->left->color = BLACK;
                    s->color = RED;
                    rightrotate(s);
                    s = p->parent->right;
                }
                s->color = p->parent->color;
                p->parent->color = BLACK;
                s->right->color = BLACK;
                leftrotate(p->parent);
                p=root;
            }
        } else {
            s = p->parent->left;
            if(s != NULL && s->color == RED) {
                s->color = BLACK;
                p->parent->color = RED;
                rightrotate(p->parent);
                s = p->parent->left;
            }
            if(s == NULL){
                break;
            }
            if((s->left == NULL || s->left->color == BLACK) && (s->right == NULL || s->right->color == BLACK))
            {
                s->color = RED;
                p = p->parent;
            } else {
                if(s->left == NULL || s->left->color == BLACK) {
                    if (s->right != NULL)
                        s->right->color = BLACK;
                    s->color = RED;
                    leftrotate(s);
                    s = p->parent->left;
                }
                s->color = p->parent->color;
                p->parent->color = BLACK;
                if(s->left != NULL)
                    s->left->color = BLACK;
                rightrotate(p->parent);
                p = root;
            }
        }
        p->color = BLACK;
        root->color = BLACK;
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
            y->parent = NULL;
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
