#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)malloc(sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  p->nil->parent = p->nil;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil) {
    delete_node(t, t->root);
  }
  free(t->nil);
  free(t);
}

void delete_node(rbtree *t, node_t *n) {
  if (n == t->nil) {
    return;
  }
  delete_node(t, n->left);
  delete_node(t, n->right);
  // printf("Freeing node with key: %d at address: %p\n", n->key, (void*)n);
  free(n);
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;

  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil)
    x->right->parent = y;

  x->parent = y->parent;
  if (y->parent == t->nil)
    t->root = x;
  else if (y == y->parent->right)
    y->parent->right = x;
  else
    y->parent->left = x;

  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *n)
{
  while (n->parent->color == RBTREE_RED)
  {
    if (n->parent == n->parent->parent->left)
    {
      node_t *y = n->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else
      {
        if (n == n->parent->right)
        {
          n=n->parent;
          left_rotate(t, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotate(t, n->parent->parent);
      }
    }
    else
    {
      node_t *y = n->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else
      {
        if (n == n->parent->left)
        {
          n = n->parent;
          right_rotate(t, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotate(t, n->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) 
{
  node_t *y = t->nil;
  node_t *x = t->root;
  while (x != t->nil)
  {
    y = x;
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->parent = y;
  z->key = key;

  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *node_find(node_t *t, node_t *nil, const key_t key) 
{
    while (t != nil) {
        if (key == t->key) {
            return t;
        } else if (key < t->key) {
            t = t->left;
        } else {
            t = t->right;
        }
    }
    return nil;
}

node_t *rbtree_find(const rbtree *t, const key_t key) 
{
  // TODO: implement find
  if (t == NULL || t->root == NULL)
  {
      return NULL;
  }

  node_t *result = node_find(t->root, t->nil, key);
  if (result == t->nil) {
      return NULL;
  }
  return result;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while (cur->left != t->nil)
    cur = cur->left;
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while (cur->right != t->nil)
    cur = cur->right;
  return cur;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *n) {
  while (n != t->root && n->color == RBTREE_BLACK)
  {
    node_t *sibling;
    if (n == n->parent->left)
    {
      sibling = n->parent->right;
      if (sibling->color == RBTREE_RED)
      {
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        left_rotate(t, n->parent);
        sibling = n->parent->right;
      }
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK)
      {
        sibling->color = RBTREE_RED;
        n = n->parent;
      }
      else
      {
        if (sibling->right->color == RBTREE_BLACK)
        {
          sibling->left->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          right_rotate(t, sibling);
          sibling = n->parent->right;
        }
        sibling->color = n->parent->color;
        n->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        left_rotate(t, n->parent);
        n = t->root;
      }
    }
    else
    {
      sibling = n->parent->left;
      if (sibling->color == RBTREE_RED)
      {
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        right_rotate(t, n->parent);
        sibling = n->parent->left;
      }
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK)
      {
        sibling->color = RBTREE_RED;
        n = n->parent;
      }
      else
      {
        if (sibling->left->color == RBTREE_BLACK)
        {
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          left_rotate(t, sibling);
          sibling = n->parent->left;
        }
        sibling->color = n->parent->color;
        n->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        right_rotate(t, n->parent);
        n = t->root;
      }
    }
  }
  n->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *n) {
  // TODO: implement erase
  node_t *x;
  node_t *y = n;
  color_t origincolor = y->color;
  if (n->left == t->nil)
  {
    x = n->right;
    rbtree_transplant(t, n, n->right);
  }
  else if (n->right == t->nil)
  {
    x = n->left;
    rbtree_transplant(t, n, n->left);
  }
  else
  {
    y = y->right;
    while (y->left != t->nil)
      y = y->left;
    origincolor = y->color;
    x = y->right;
    if (y == n->right)
      x->parent = y;
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = n->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, n, y);
    y->left = n->left;
    y->left->parent = y;
    y->color = n->color;
  }
  if (origincolor == RBTREE_BLACK)
    rbtree_delete_fixup(t, x);
  free(n);
  return 0;
}

size_t inorder_traversal(const rbtree *t, const node_t *n, key_t *arr, size_t index) {
    if (n == t->nil) {
        return index;
    }
    index = inorder_traversal(t, n->left, arr, index);
    arr[index++] = n->key;
    index = inorder_traversal(t, n->right, arr, index);

    return index;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t == NULL) {
    return -1;
  }
  inorder_traversal(t, t->root, arr, 0);
  return 0;
}
