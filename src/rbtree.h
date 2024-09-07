#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_node(rbtree *, node_t *);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
void rbtree_insert_fixup(rbtree *, node_t *);

node_t *node_find(node_t *, node_t *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);

node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
void left_rotate(rbtree *, node_t *);
void right_rotate(rbtree *, node_t *);

void rbtree_transplant(rbtree *, node_t *, node_t *);
int rbtree_erase(rbtree *, node_t *);

size_t inorder_traversalconst(const rbtree *, const node_t *, key_t *, size_t);
int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
