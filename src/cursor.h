#pragma once

#include "table.h"
#include <stdbool.h>
#include <stdint.h>
typedef struct {
  Table *table;
  uint32_t page_num;
  uint32_t cell_idx;
  bool end_of_table;
} Cursor;

Cursor *table_start(Table *table);

Cursor *table_end(Table *table);

void *cursor_value(Cursor *cursor);

void cursor_advnce(Cursor *cursor);

void print_cursor(Cursor *cursor);
