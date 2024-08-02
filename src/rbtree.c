#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  p->nil = NULL;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *right_spin(node_t *self)
{
  node_t *parentNode = self->parent, *grandParentNode = parentNode->parent;
  grandParentNode->right = parentNode->left;
  parentNode->left = grandParentNode;
  return parentNode;
}

node_t *left_spin(node_t *self)
{
  node_t *parentNode = self->parent, *grandParentNode = parentNode->parent;
  grandParentNode->left = parentNode->right;
  parentNode->right = grandParentNode;
  return parentNode;
}

node_t *spin(node_t *self, int isLeft)
{
  return isLeft ? left_spin(self) : right_spin(self);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *newNode = malloc(sizeof(node_t));
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->parent = NULL;

  // 맨 처음 넣는 경우
  if (t->root == NULL)
  {
    newNode->color = RBTREE_BLACK;
    t->root = newNode;
    return t->root;
  }

  node_t *targetNode = t->root;
  while (targetNode == t->nil)
  {
    if (targetNode->key > key)
      targetNode = targetNode->right;
    else
      targetNode = targetNode->left;
    newNode->parent = targetNode;
  }
  targetNode = newNode;
  while (targetNode->parent->color == RBTREE_RED)
  {
    node_t *parentNode = targetNode->parent;
    node_t *grandParentNode = parentNode->parent;

    int isUncleLeftNode = grandParentNode->left != parentNode;
    node_t *uncleNode = isUncleLeftNode ? grandParentNode->left : grandParentNode->right;

    if (uncleNode->color == RBTREE_RED)
    {
      uncleNode->color = RBTREE_BLACK;
      parentNode->color = RBTREE_BLACK;
      grandParentNode->color = RBTREE_RED;
      targetNode = grandParentNode;
      continue;
    }
    else
    {
      int isCurve = targetNode == (isUncleLeftNode ? grandParentNode->right->left : grandParentNode->left->right);
      if (isCurve)
      {
        spin(targetNode, !isUncleLeftNode);
      }
      color_t tmpColor = parentNode->color;
      parentNode->color = grandParentNode->color;
      grandParentNode->color = tmpColor;
      targetNode = grandParentNode;
    }
  }
  t->root->color = RBTREE_BLACK;

  return newNode;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
