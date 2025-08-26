#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "linked-list.h"

#define GET_KEY(ptr) (((key_data_pair_t*)(ptr))->key)
#define GET_DATA(ptr) (((key_data_pair_t*)(ptr))->data)

#pragma pack(1)
typedef struct key_data_pair
{
    void* key;
    void* data;
}key_data_pair_t;
typedef struct hash_table
{
    uint32_t hash_table_size;
    uint32_t hash_table_capacity;
    uint32_t (*hash_func)(void*, uint32_t);
    bool (*hash_key_compare_function)(void*, void*);
    singly_list_t hash_table_elements[0];
}hash_table_t;
#pragma pack()

hash_table_t* hash_table_create(uint32_t table_size, uint32_t (*hash_func)(void*, uint32_t), bool (*key_compare_function)(void*, void*));
extern void* hash_table_search(hash_table_t* table, void* key);
void hash_table_insert(hash_table_t* table, void* key, void* data);
void hash_table_free(hash_table_t* table);

#endif/*__HASH_TABLE_H__*/