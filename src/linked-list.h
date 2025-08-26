#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct list_node
{
    void *data;
    struct list_node *next;
}singly_list_node_t;

typedef struct singly_list
{
    singly_list_node_t* head;
}singly_list_t;

extern singly_list_node_t* singly_list_search(singly_list_t *list, void* data, bool(*compara_func)(void*, void*));
extern singly_list_node_t* singly_list_insert_at_first(singly_list_t* list, void *data);
void singly_list_free(singly_list_t* list, void(*free_data)(void*));

#endif/*__LINKED_LIST_H__*/