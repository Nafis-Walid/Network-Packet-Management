#include "hash-table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "app.h"

#define PRIME1 2654435761U
#define PRIME2 2246822519U

void free_hash_node(void* key_data_pair);

hash_table_t* hash_table_create(uint32_t table_size, uint32_t (*hash_func)(void*, uint32_t), bool (*key_compare_function)(void*, void*))
{
    hash_table_t* table = NULL;

    table = (hash_table_t*)calloc(table_size * sizeof(singly_list_node_t*) + sizeof(hash_table_t), 1);

    if(table == NULL)
    {
        perror("Memory Allocation Failed while creating Table");
        return NULL;
    }

    table->hash_table_size = table_size;
    table->hash_func = hash_func;
    table->hash_key_compare_function = key_compare_function;

    return table;
}

void hash_table_insert(hash_table_t* table, void* key, void* data)
{
    key_data_pair_t* key_data_pair = NULL;
    uint32_t index = 0;

    key_data_pair = (key_data_pair_t*)malloc(sizeof(key_data_pair_t));
    key_data_pair->key = key;
    key_data_pair->data = data;
    index = table->hash_func(key, table->hash_table_size);
    singly_list_insert_at_first(&table->hash_table_elements[index], (void*)key_data_pair);
}

void* hash_table_search(hash_table_t* table, void* key)
{  
    singly_list_node_t* found = NULL;

    if(table == NULL)
        return NULL;

    found = singly_list_search(&table->hash_table_elements[table->hash_func(key, table->hash_table_size)], key, table->hash_key_compare_function);
    if(found == NULL)
        return NULL;
    
    found = GET_DATA(found->data);
    return found;
}

void hash_table_free(hash_table_t* table)
{
    uint32_t i = 0;

    for(i = 0; i < table->hash_table_size; i++)
        singly_list_free(&table->hash_table_elements[i], free_hash_node);

    free(table);
}

void free_hash_node(void* key_data_pair)
{
    free(((key_data_pair_t*)key_data_pair)->key);
    free(((key_data_pair_t*)key_data_pair)->data);
}


bool is_prime(uint32_t num)
{
    uint32_t i = 0;

    if(num <= 1)
        return false;

    for(i = 2; i * i <= num; i++)
        if(num % i == 0)
            return false;

    return true;
}

uint32_t next_prime(uint32_t num)
{
    if(num < 2)
        return 2;

    if(num % 2 == 0)
        num++;

    while(!is_prime(num))
        num += 2;

    return num;
}


