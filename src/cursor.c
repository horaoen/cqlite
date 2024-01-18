#include "cursor.h"
#include "b-tree.h"
#include "table.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Cursor *table_start(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = table->root_page_num;
  cursor->cell_idx = 0;

  void *root_node = get_page(table->pager, table->root_page_num);
  uint32_t cells_num = *get_leaf_node_cells_num(root_node);
  cursor->end_of_table = (cells_num == 0);

  return cursor;
}

Cursor *table_end(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = table->root_page_num;

  void *root_node = get_page(table->pager, table->root_page_num);
  uint32_t cells_num = *get_leaf_node_cells_num(root_node);
  cursor->cell_idx = cells_num;
  cursor->end_of_table = true;

  return cursor;
}

void *cursor_value(Cursor *cursor) {
  uint32_t page_num = cursor->page_num;
  void *page = get_page(cursor->table->pager, page_num);

  return get_leaf_node_value(page, cursor->cell_idx);
}

void cursor_advnce(Cursor *cursor) {
  uint32_t page_num = cursor->page_num;
  void *node = get_page(cursor->table->pager, page_num);

  cursor->cell_idx += 1;
  if (cursor->cell_idx >= (*get_leaf_node_cells_num(node))) {
    cursor->end_of_table = true;
  }
}

void print_cursor(Cursor *cursor) {
  printf("Cursor (page_num: %d, cell_idx: %d, end_of_table: %d)\n",
         cursor->page_num, cursor->cell_idx, cursor->end_of_table);
}
