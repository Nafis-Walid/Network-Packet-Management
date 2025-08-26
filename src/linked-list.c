#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"
#include "debug.h"

singly_list_node_t *singly_list_create_node(void *data);

singly_list_node_t *singly_list_create_node(void *data)
{
    singly_list_node_t *new_node = NULL;

    new_node = (singly_list_node_t *)malloc(sizeof(singly_list_node_t));

    if (new_node == NULL)
    {
        perror("Memory allocation failed while creating Node.");
        return NULL;
    }

    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}

void singly_list_free_node(singly_list_node_t* node, void(*free_data)(void*))
{
    free_data(node->data);
    free(node);

    return;
}

singly_list_node_t* singly_list_insert_at_first(singly_list_t* list, void *data)
{
    singly_list_node_t *new_node = NULL;

    new_node = singly_list_create_node(data);

    if(list->head == NULL)
    {
        list->head = new_node;
        return new_node;
    }
    
    new_node->next = list->head;
    list->head = new_node;

    return new_node;
}

singly_list_node_t* singly_list_search(singly_list_t* list, void* data, bool(*compare_func)(void*, void*))
{
    singly_list_node_t* tmp = NULL;

    tmp = list->head;

    while(tmp != NULL)
    {
        DEBUG_PRINT("%p | %p", tmp->data, data);
        if(compare_func(*((void**)(tmp->data)), data))
            return tmp;
        
        tmp = tmp->next;
    }
    return NULL;
}

void singly_list_free(singly_list_t* list, void(*free_data)(void*))
{
    singly_list_node_t* temp = NULL;
    singly_list_node_t* curr = NULL;

    temp = list->head;
    if(free_data == NULL)
        free_data = free;/*default free if NULL is passed*/

    while(temp != NULL)
    {
        curr = temp;
        temp = temp->next;
        free_data(curr->data);
        curr->data = NULL;
        free(curr);
    }
 
    return;
}
