#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->key = -1;

  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *right_spin(rbtree *t, node_t *parentNode, const int isGrandParentLeftNode)
{
  node_t *leftNode = parentNode->left;
  if (t->root == parentNode)
    t->root = leftNode;

  leftNode->parent = parentNode->parent;
  if (isGrandParentLeftNode)
    leftNode->parent->left = leftNode;
  else
    leftNode->parent->right = leftNode;

  parentNode->left = leftNode->right;
  parentNode->left->parent = parentNode;

  parentNode->parent = leftNode;
  leftNode->right = parentNode;

  return leftNode;
}

node_t *left_spin(rbtree *t, node_t *parentNode, const int isGrandParentLeftNode)
{
  node_t *rightNode = parentNode->right;
  if (t->root == parentNode)
    t->root = rightNode;

  rightNode->parent = parentNode->parent;
  if (isGrandParentLeftNode)
    rightNode->parent->left = rightNode;
  else
    rightNode->parent->right = rightNode;

  parentNode->right = rightNode->left;
  parentNode->right->parent = parentNode;

  parentNode->parent = rightNode;
  rightNode->left = parentNode;

  return rightNode;
}

/**
 * 그래프를 회전한다.
 * t: rbtree입니다.
 * parent: 회전의 주체 노드(기준점)
 * isLeft: 왼쪽으로 직선되어 있는지
 * return값은 가장 높은 노드(root와 근접한 노드)
 */
node_t *spin(rbtree *t, node_t *parentNode, const int isLeft)
{
  int isGrandParentLeftNode = parentNode->parent->left == parentNode;
  return isLeft ? left_spin(t, parentNode, isGrandParentLeftNode) : right_spin(t, parentNode, isGrandParentLeftNode);
}

void color_switch(node_t *t1, node_t *t2)
{
  color_t tmpColor = t2->color;
  t2->color = t1->color;
  t1->color = tmpColor;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *newNode = malloc(sizeof(node_t));
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->parent = t->nil;

  // 맨 처음 넣는 경우
  if (t->root == t->nil)
  {
    newNode->color = RBTREE_BLACK;
    t->root = newNode;
    return t->root;
  }
  // Node를 삽입한다.
  node_t *targetNode = t->root;
  while (targetNode != t->nil)
  {
    newNode->parent = targetNode;
    if (key < targetNode->key)
      targetNode = targetNode->left;
    else
      targetNode = targetNode->right;
  }
  if (key < newNode->parent->key)
    newNode->parent->left = newNode;
  else
    newNode->parent->right = newNode;

  targetNode = newNode;
  // 재 정렬
  while (targetNode != t->nil && targetNode->parent->color == RBTREE_RED)
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
    }
    else
    {
      int isCurve = isUncleLeftNode
                        ? grandParentNode->right->left == targetNode
                        : grandParentNode->left->right == targetNode;
      if (isCurve)
      {
        parentNode = spin(t, parentNode, !isUncleLeftNode);
      }
      color_switch(parentNode, grandParentNode);
      targetNode = spin(t, grandParentNode, isUncleLeftNode)->parent;
    }
  }
  t->root->color = RBTREE_BLACK;

  return newNode;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *targetNode = t->root;
  while (targetNode != t->nil && targetNode->key != key)
  {
    if (targetNode->key > key)
      targetNode = targetNode->right;
    else
      targetNode = targetNode->left;
  }
  if (targetNode == t->nil)
    return NULL;
  return targetNode;
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

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int inorder(const rbtree *t, const node_t *node, key_t *arr, int deps)
{
  if (node != t->nil)
  {
    deps = inorder(t, node->left, arr, deps);
    arr[deps++] = node->key;
    deps = inorder(t, node->right, arr, deps);
  }
  return deps;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  return inorder(t, t->root, arr, 0);
}
