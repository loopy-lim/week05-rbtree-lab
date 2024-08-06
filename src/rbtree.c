#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->key = -1;
  p->nil->right = p->nil;
  p->nil->left = p->nil;
  p->nil->parent = p->nil;
  p->root = p->nil;
  return p;
}

int is_left_node(const node_t *parentNode, const node_t *node)
{
  return parentNode->left == node;
};

void post_order_delete(const rbtree *t, node_t *node)
{
  if (node != t->nil)
  {
    post_order_delete(t, node->left);
    post_order_delete(t, node->right);
    free(node);
  }
}

void delete_rbtree(rbtree *t)
{
  post_order_delete(t, t->root);
  free(t->nil);
  free(t);
}

void right_spin(rbtree *t, node_t *parentNode)
{
  node_t *leftNode = parentNode->left;
  parentNode->left = leftNode->right;
  if (leftNode->right != t->nil)
    leftNode->right->parent = parentNode;
  leftNode->parent = parentNode->parent;
  if (parentNode->parent == t->nil)
    t->root = leftNode;
  else if (parentNode == parentNode->parent->right)
    parentNode->parent->right = leftNode;
  else
    parentNode->parent->left = leftNode;
  leftNode->right = parentNode;
  parentNode->parent = leftNode;
}

void left_spin(rbtree *t, node_t *parentNode)
{
  node_t *rightNode = parentNode->right;
  parentNode->right = rightNode->left;
  if (rightNode->left != t->nil)
    rightNode->left->parent = parentNode;
  rightNode->parent = parentNode->parent;
  if (parentNode->parent == t->nil)
    t->root = rightNode;
  else if (parentNode == parentNode->parent->left)
    parentNode->parent->left = rightNode;
  else
    parentNode->parent->right = rightNode;
  rightNode->left = parentNode;
  parentNode->parent = rightNode;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *z = malloc(sizeof(node_t));
  z->key = key;

  node_t *y = t->nil, *x = t->root;
  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_spin(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_spin(t, z->parent->parent);
      }
    }
    else
    {
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_spin(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_spin(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *targetNode = t->root;
  while (targetNode != t->nil)
  {
    if (targetNode->key == key)
      return targetNode;
    if (targetNode->key < key)
      targetNode = targetNode->right;
    else
      targetNode = targetNode->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  node_t *minNode = t->root;
  while (minNode->left != t->nil)
    minNode = minNode->left;
  return minNode;
}

node_t *rbtree_max(const rbtree *t)
{
  node_t *maxNode = t->root;
  while (maxNode->right != t->nil)
    maxNode = maxNode->right;
  return maxNode;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
};

node_t *tree_minimum(rbtree *t, node_t *v)
{
  while (v->left != t->nil)
    v = v->left;
  return v;
}

void rb_delete_fixup(rbtree *t, node_t *x)
{
  node_t *w = NULL;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_spin(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_spin(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_spin(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_spin(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_spin(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_spin(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
};

int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *y = p, *x = NULL;
  ;
  color_t yOriginColor = y->color;

  if (p->left == t->nil)
  {
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    rb_transplant(t, p, p->left);
  }
  else
  {
    y = tree_minimum(t, p->right);
    yOriginColor = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else
    {
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (yOriginColor == RBTREE_BLACK)
    rb_delete_fixup(t, x);
  return 0;
}

int inorder_insert_to_array(const rbtree *t, const node_t *node, key_t *arr, int deps)
{
  if (node != t->nil)
  {
    deps = inorder_insert_to_array(t, node->left, arr, deps);
    arr[deps++] = node->key;
    deps = inorder_insert_to_array(t, node->right, arr, deps);
  }
  return deps;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  return inorder_insert_to_array(t, t->root, arr, 0);
}
