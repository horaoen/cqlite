#pragma once

#include <stdint.h>

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE 4096

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void *pages[TABLE_MAX_PAGES];
  uint32_t pages_num;
} Pager;

void pager_flush(Pager *pager, uint32_t page_num);

Pager *open_file(const char *filename);
