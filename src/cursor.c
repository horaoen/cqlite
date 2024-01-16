#include "cursor.h"
#include "row.h"
#include "table.h"
#include <stdlib.h>

Cursor *table_start(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = 0;
  cursor->end_of_table = (table->row_nums == 0);
  return cursor;
}

Cursor *table_end(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = table->row_nums;
  cursor->end_of_table = true;
  return cursor;
}

void *cursor_value(Cursor *cursor) {
  uint32_t row_num = cursor->row_num;
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void *page = get_page(cursor->table->pager, page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

void cursor_advnce(Cursor *cursor) {
  cursor->row_num += 1;
  if (cursor->row_num >= cursor->table->row_nums) {
    cursor->end_of_table = true;
  }
}
