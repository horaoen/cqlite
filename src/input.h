#pragma once

#include "row.h"
#include <stddef.h>
typedef struct {
  char *buffer;
  size_t buffer_length;
  size_t input_length;
} InputBuffer;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_NEGATIVE_ID,
  PREPARE_STRING_TOO_LONG,
  PREPARE_UNRECOGNIZED_STATEMENT,
} PrepareResult;

void read_input(InputBuffer *input_buffer);

InputBuffer *new_input_buffer();

void print_prompt();

void close_input_buffer(InputBuffer *input_buffer);

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement);
