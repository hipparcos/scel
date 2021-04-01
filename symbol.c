#include "symbol.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "panic.h"
#include "scel.h"
#include "utf8.h"

struct scel_symbol
{
  const byte* id;
  /* AVL tree */
  struct scel_symbol *left;
  struct scel_symbol *right;
  size_t height;
};

const size_t SCEL_SYMBOL_SIZE = sizeof(struct scel_symbol);

struct scel_symbol *
scel_symbol_init(void *buf, const byte *id)
{
  struct scel_symbol *sym = (struct scel_symbol*) buf;
  memset(sym, 0, SCEL_SYMBOL_SIZE);
  sym->id = id;
  return sym;
}

static struct scel_symbol *
avl_rotate_left(struct scel_symbol *tree)
{
  struct scel_symbol *new_left = tree;
  struct scel_symbol *new_left_right = tree->right->left;
  tree = tree->right;
  tree->left = new_left;
  new_left->right = new_left_right;
  return tree;
}

static struct scel_symbol *
avl_rotate_right(struct scel_symbol *tree)
{
  struct scel_symbol *new_right = tree;
  struct scel_symbol *new_right_left = tree->left->right;
  tree = tree->left;
  tree->right = new_right;
  new_right->left = new_right_left;
  return tree;
}

static int
avl_height(const struct scel_symbol *tree)
{
  if (!tree) {
    return 0;
  }
  return tree->height;
}

static int
avl_balance_factor(const struct scel_symbol *tree)
{
  if (!tree) {
    return 0;
  }
  return avl_height(tree->right) - avl_height(tree->left);
}

static struct scel_symbol *
avl_update_height(struct scel_symbol *tree)
{
  tree->height = 0;
  if (tree->left && (tree->left->height > tree->height)) {
    tree->height = tree->left->height;
  }
  if (tree->right && (tree->right->height > tree->height)) {
    tree->height = tree->right->height;
  }
  tree->height += 1;
  return tree;
}

static struct scel_symbol *
avl_balance(struct scel_symbol *tree)
{
  if (!tree) {
    return NULL;
  }
  int balance_factor = avl_balance_factor(tree);
  if (balance_factor > 1) {
    if (avl_balance_factor(tree->right) < -1) {
      tree->right = avl_rotate_right(tree->right);
    }
    tree = avl_rotate_left(tree);
  }
  if (balance_factor < -1) {
    if (avl_balance_factor(tree->right) > 1) {
      tree->left = avl_rotate_left(tree->left);
    }
    tree = avl_rotate_right(tree);
  }
  return avl_update_height(tree);
}

static struct scel_symbol *
avl_insert(struct scel_symbol* tree, struct scel_symbol *new)
{
  if (!tree) {
    return new;
  }
  int s = strcmp((const char*)new->id, (const char *)tree->id);
  if (s < 0)
    tree->left = avl_insert(tree->left, new);
  else if (s > 0)
    tree->right = avl_insert(tree->right, new);
  else
    panic("duplicated symbol %s", new->id);
  return avl_balance(tree);
}

struct scel_symbol *
scel_symbol_tree(struct scel_symbol *root, struct scel_symbol *sym)
{
  assert(sym);
  return avl_insert(root, sym);
}

static struct scel_symbol *
avl_lookup(struct scel_symbol *root, const byte *id)
{
  struct scel_symbol *node = root;
  while (node) {
    int c = strcmp((const char*)id, (const char*)node->id);
    if (c == 0)
      return node;
    else if (c < 0)
      node = node->left;
    else if (c > 0)
      node = node->right;
  }
  return node;
}

struct scel_symbol *
scel_symbol_find(struct scel_symbol *root, const byte *id)
{
  assert(root);
  return avl_lookup(root, id);
}
