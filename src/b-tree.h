#pragma once

#include "cursor.h"
#include "pager.h"
#include "row.h"
#include <stdint.h>

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

/*
 * Common Node Header Layout
 */
static const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
static const uint32_t NODE_TYPE_OFFSET = 0;
static const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
static const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
static const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
static const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
static const uint8_t COMMON_NODE_HEADER_SIZE =
    NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/*
 * Leaf Node Header Layout
 */
static const uint32_t LEAF_NODE_CELLS_NUM_SIZE = sizeof(uint32_t);
static const uint32_t LEAF_NODE_CELLS_NUM_OFFSET = COMMON_NODE_HEADER_SIZE;
static const uint32_t LEAF_NODE_HEADER_SIZE =
    COMMON_NODE_HEADER_SIZE + LEAF_NODE_CELLS_NUM_SIZE;

/*
 * Leaf Node Body Layout
 */
static const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
static const uint32_t LEAF_NODE_KEY_OFFSET = 0;
static const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
static const uint32_t LEAF_NODE_VALUE_OFFSET =
    LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
static const uint32_t LEAF_NODE_CELL_SIZE =
    LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
static const uint32_t LEAF_NODE_SPACE_FOR_CELLS =
    PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
static const uint32_t LEAF_NODE_MAX_CELLS =
    LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

/*
 * 获取节点中的单元格数量
 */
uint32_t *get_leaf_node_cells_num(void *node);

/*
 * 获取节点中的单元格
 */
void *get_leaf_node_cell(void *node, uint32_t cell_num);

/*
 * 获取单元格key
 */
uint32_t *get_leaf_node_key(void *node, uint32_t cell_num);

/*
 * 获取单元格value
 */
void *get_leaf_node_value(void *node, uint32_t cell_num);

/*
 *   初始化叶节点
 */
void initialize_leaf_node(void *node);

/*
 * 插入
 */
void insert_leaf_node(Cursor *cursor, uint32_t key, Row *value);

void print_leaf_node(void *node);

void print_constants();
