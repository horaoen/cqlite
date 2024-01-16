#pragma once

#include "input.h"
#include "pager.h"
#include <stdint.h>

#define TABLE_MAX_PAGES 100

typedef struct {
  uint32_t row_nums;
  Pager *pager;
} Table;

static const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
static const uint32_t TABLE_MAX_ROWS = TABLE_MAX_PAGES * ROWS_PER_PAGE;

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

void *get_page(Pager *pager, uint32_t page_num);

Table *db_open(const char *filename);

void db_close(Table *table);

ExecuteResult execute_statement(Statement *statement, Table *table);

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table);
