#include "table.h"
#include "input.h"
#include "pager.h"
#include "row.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = TABLE_MAX_PAGES * ROWS_PER_PAGE;

Table *db_open(const char *filename) {
  Pager *pager = pager_open(filename);
  uint32_t num_rows = pager->file_length / ROW_SIZE;

  Table *table = (Table *)malloc(sizeof(Table));
  table->pager = pager;
  table->row_nums = num_rows;
  return table;
}

void db_close(Table *table) {
  Pager *pager = table->pager;
  uint32_t full_pages_num = table->row_nums / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < full_pages_num; i++) {
    if (pager->pages[i] == NULL) {
      continue;
    }
    pager_flush(pager, i, PAGE_SIZE);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
  }

  // There may be a partial page to write to the end of the file
  // This should not be needed after we switch to a B-tree
  uint32_t num_additional_rows = table->row_nums % ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = full_pages_num;
    if (pager->pages[page_num] != NULL) {
      pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  int result = close(pager->file_descriptor);
  if (result == -1) {
    printf("Error closing db file.\n");
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    void *page = pager->pages[i];
    if (page) {
      free(page);
      pager->pages[i] = NULL;
    }
  }
  free(pager);
  free(table);
}

/*
 * 根据页码定位page内存
 */
void *get_page(Pager *pager, uint32_t page_num) {
  if (page_num > TABLE_MAX_ROWS) {
    printf("Tried to fetch page number out of bounds. %d > %d\n", page_num,
           TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }
  if (pager->pages[page_num] == NULL) {
    // Cache miss. Allocate memory and load from file.
    void *page = malloc(PAGE_SIZE);
    uint32_t total_page_num = pager->file_length / PAGE_SIZE;

    // We might save a partial page at the end of the file
    if (pager->file_length % PAGE_SIZE) {
      total_page_num += 1;
    }

    if (page_num <= total_page_num) {
      // 指针定位文件位置
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      // 读取文件
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1) {
        printf("Error reading file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }

    pager->pages[page_num] = page;
  }
  return pager->pages[page_num];
}

/*
 * 根据行号定位table内存
 * parameter:
 *   1. table
 *   2. row_num: 行号
 */
void *row_slot(Table *table, uint32_t row_num) {
  // 页码
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void *page = get_page(table->pager, page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

ExecuteResult execute_insert(Statement *statement, Table *table) {
  if (table->row_nums >= TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }

  Row *row_to_insert = &(statement->row_to_insert);

  // insert
  serialize_row(row_to_insert, row_slot(table, table->row_nums));
  table->row_nums += 1;

  return EXECUTE_SUCCESS;
}

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

ExecuteResult execute_select(Statement *statement, Table *table) {
  Row row;
  for (uint32_t i = 0; i < table->row_nums; i++) {
    deserialize_row(row_slot(table, i), &row);
    print_row(&row);
  }
  return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case (STATEMENT_INSERT):
    return execute_insert(statement, table);
  case (STATEMENT_SELECT):
    return execute_select(statement, table);
  }
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    db_close(table);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}
