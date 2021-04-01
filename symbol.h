#ifndef H_SYMBOL__
#define H_SYMBOL__

#include "scel.h"
#include "heap.h"
#include "utf8.h"

/** scel_symbol: a symbol is an identifier pointing to a value.
 ** The value it resolves to depends of the current scope.
 ** Symbols are organized as an AVL so they can directly be allocated
 ** on a scel_heap. */
struct scel_symbol;

/** SCEL_SYMBOL_SIZE: the size of the scel_symbol structure. */
extern const size_t SCEL_SYMBOL_SIZE;

/** scel_symbol_init: init BUF to be a symbol identified by ID.
 ** BUF must be of size SCEL_SYMBOL_SIZE.
 ** ID is not copied so its lifetime must be greater than the symbol. */
struct scel_symbol* scel_symbol_init(void *buf, const byte *id);

/** scel_symbol_tree: put SYM in the tree ROOT.
 ** Allow to find it again by its id from the root.
 ** Return the new root of the AVL.
 ** The id of SYM must not be already part of the tree. */
struct scel_symbol* scel_symbol_tree(struct scel_symbol *root, struct scel_symbol *sym);

/** scel_symbol_find: return the symbol identified by ID or NULL. */
struct scel_symbol* scel_symbol_find(struct scel_symbol *root, const byte *id);

#endif
