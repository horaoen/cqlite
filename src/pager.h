#pragma once

#include "row.h"
#include <stdint.h>

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE 4096

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void *pages[TABLE_MAX_PAGES];
} Pager;

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);

Pager *pager_open(const char *filename);
