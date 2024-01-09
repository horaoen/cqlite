#pragma once

#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

static const uint32_t ID_SIZE = size_of_attribute(Row, id);
static const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
static const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
static const uint32_t ID_OFFSET = 0;
static const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
static const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
static const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);
