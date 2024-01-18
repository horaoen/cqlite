#include "b-tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t *get_leaf_node_cells_num(void *node) {
  return node + LEAF_NODE_CELLS_NUM_OFFSET;
}

void *get_leaf_node_cell(void *node, uint32_t cell_idx) {
  return node + LEAF_NODE_HEADER_SIZE + cell_idx * LEAF_NODE_CELL_SIZE;
}

uint32_t *get_leaf_node_key(void *node, uint32_t cell_idx) {
  return get_leaf_node_cell(node, cell_idx);
}

void *get_leaf_node_value(void *node, uint32_t cell_idx) {
  return get_leaf_node_cell(node, cell_idx) + LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(void *node) { *get_leaf_node_cells_num(node) = 0; }

void insert_leaf_node(Cursor *cursor, uint32_t key, Row *value) {
  void *node = get_page(cursor->table->pager, cursor->page_num);

  uint32_t num_cells = *get_leaf_node_cells_num(node);
  if (num_cells >= LEAF_NODE_MAX_CELLS) {
    // Node full
    printf("Need to implement splitting a leaf node.\n");
    exit(EXIT_FAILURE);
  }

  if (cursor->cell_idx < num_cells) {
    // Make room for new cell
    for (uint32_t i = num_cells; i > cursor->cell_idx; i--) {
      memcpy(get_leaf_node_cell(node, i), get_leaf_node_cell(node, i - 1),
             LEAF_NODE_CELL_SIZE);
    }
  }

  *(get_leaf_node_cells_num(node)) += 1;
  *(get_leaf_node_key(node, cursor->cell_idx)) = key;
  print_row(value);
  serialize_row(value, get_leaf_node_value(node, cursor->cell_idx));
}

void print_leaf_node(void *node) {
  uint32_t num_cells = *get_leaf_node_cells_num(node);
  printf("leaf (size %d)\n", num_cells);
  for (uint32_t i = 0; i < num_cells; i++) {
    uint32_t key = *get_leaf_node_key(node, i);
    Row *row = get_leaf_node_value(node, i);
    printf("  - %d : key-{%d}, value-", i, key);
    print_row(row);
  }
}

void print_constants() {
  printf("ROW_SIZE: %d\n", ROW_SIZE);
  printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
  printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
  printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
  printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
  printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}
