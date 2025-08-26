#include "app.h"
#include <stdlib.h>

#define SOURCE_IP 0
#define DEST_IP 1
#define PACKET_FILE_SMALL "database/exported-packets.txt"
#define PACKET_FILE_LARGE "database/udp-big.txt"
#define PACKET_FILE_TEST "database/udp.txt"
#define INITIAL_TABLE_SIZE 13
#define PRIME1 2654435761U
#define PRIME2 2246822519U
#define INDEX_FLD 7
#define SRC_IP_FLD 16
#define DEST_IP_FLD 17
#define CNT_FLD 14
#define HASH_TABLE_BORDER\
    "+---------+-----------------+------------------+----------------+\n"
#define HASH_TABLE_HEADER \
    "+---------------------------------------------------------------+\n"\
    "|                            Hash Table                         |\n"\
    "+---------+-----------------+------------------+----------------+\n"\
    "|  Index  |    Source IP    |  Destination IP  |  Packet Count  |\n"\
    "+---------+-----------------+------------------+----------------+\n"
#define LINKED_LIST_BORDER\
    "+-----------------+------------------+----------------+\n"
#define LINKED_LIST_HEADER \
    "+-----------------------------------------------------+\n"\
    "|                      Linked List                    |\n"\
    "+-----------------+------------------+----------------+\n"\
    "|    Source IP    |  Destination IP  |  Packet Count  |\n"\
    "+-----------------+------------------+----------------+\n"

typedef struct ip_frequency_pair
{
    ipv4_t src_dest[2];
    uint32_t num_pkt;
}ip_frequency_pair_t;


void* get_key(ipv4_header_t* ip_hdr);
uint32_t ipv4_hash(void* key, uint32_t hash_table_size);
bool ipv4_compare(void* ip1, void* ip2);
void hash_table_print(hash_table_t* table);
void linked_list_print(singly_list_t* list);

void run_packet_management_project()
{
    singly_list_t ip_freq_list = {0};
    FILE* pkt_file = NULL;
    ipv4_t* key = NULL;
    ethernet_packet_t* eth_packet = NULL;
    hash_table_t* table = NULL;
    ip_frequency_pair_t* found = NULL;
    ip_frequency_pair_t* new_node_data = NULL;
    void* hash_data = NULL;

    pkt_file = fopen(PACKET_FILE_TEST, "r");
    table = hash_table_create(INITIAL_TABLE_SIZE, ipv4_hash, ipv4_compare);

    while((eth_packet = get_ethernet_packet(get_packet(pkt_file))))
    {
        key = (ipv4_t*)get_key(&eth_packet->eth_payload.ip_header);
        
        if((found = hash_table_search(table, key)) == NULL)
        {
            new_node_data = (ip_frequency_pair_t*)malloc(sizeof(ip_frequency_pair_t));

            if(new_node_data == NULL)
            {
                perror("Memory Allocation Failed");
                free(key);
                continue;
            }

            new_node_data->src_dest[SOURCE_IP] = key[SOURCE_IP];
            new_node_data->src_dest[DEST_IP] = key[DEST_IP];
            new_node_data->num_pkt = 1;
            
            hash_data = singly_list_insert_at_first(&ip_freq_list, new_node_data);

            if(hash_data)
                hash_table_insert(table, key, hash_data);
        }
        else
        {
            free(key);
            found = ((singly_list_node_t*)found)->data;
            found->num_pkt++;
        }
        
        free(eth_packet);
    }

    hash_table_print(table);
    linked_list_print(&ip_freq_list);

    singly_list_free(&ip_freq_list, free);
    hash_table_free(table);
    fclose(pkt_file);


    return;
}

void linked_list_print(singly_list_t* list)
{
    singly_list_node_t* tmp = NULL;
    char* spaces = NULL;
    uint32_t total_pkt = 0;

    spaces = "                ";
    printf(LINKED_LIST_HEADER);

    if(list == NULL)
    {
        printf("No linked list exists\n");
        return;
    }

    if(list->head == NULL)
    {
        printf("Empty Linked List\n");
        return;
    }

    tmp = list->head;

    while(tmp)
    {
        printf("| ");
        printf(spaces + ipv4_print(&((ip_frequency_pair_t*)(tmp->data))->src_dest[SOURCE_IP]));
        printf("| ");
        printf(spaces + ipv4_print(&((ip_frequency_pair_t*)(tmp->data))->src_dest[DEST_IP]) + SRC_IP_FLD - DEST_IP_FLD);
        printf("| %-*u |\n", CNT_FLD, ((ip_frequency_pair_t*)(tmp->data))->num_pkt);
        total_pkt += ((ip_frequency_pair_t*)(tmp->data))->num_pkt;
        tmp = tmp->next;
    }

    printf(LINKED_LIST_BORDER);
    printf("| %*s | %-*u |\n", SRC_IP_FLD + DEST_IP_FLD + 1, "Total Packets", CNT_FLD, total_pkt);
    printf("+------------------------------------+----------------+\n\n");

    return;
}

void hash_table_print(hash_table_t* table)
{
    char* spaces = NULL;
    uint32_t i = 0;
    singly_list_node_t* temp = NULL;
    bool first_row = false;
    uint32_t total_pkt = 0;

    if(table == NULL)
    {
        printf("No Table exists\n");
        return;
    }

    spaces = "                ";
    printf(HASH_TABLE_HEADER);

    for(i = 0; i < table->hash_table_size; i++)
    {
        first_row = true;
        temp = table->hash_table_elements[i].head;
        printf("| %*u | ", INDEX_FLD, i);

        if(temp == NULL)
            printf("     %*s|\n", SRC_IP_FLD + DEST_IP_FLD + CNT_FLD, "");
        
        while(temp != NULL)
        {
            printf((first_row ? (first_row = false, "") : "| %*s | "), INDEX_FLD, "");
            printf(spaces + ipv4_print(&((ipv4_t*)GET_KEY(temp->data))[SOURCE_IP]));
            printf("| ");
            printf(spaces + ipv4_print(&((ipv4_t*)GET_KEY(temp->data))[DEST_IP]) - 1);
            printf("| %-*u |\n", CNT_FLD, ((ip_frequency_pair_t*)(*((void**)GET_DATA(temp->data))))->num_pkt);
            total_pkt += ((ip_frequency_pair_t*)(*((void**)GET_DATA(temp->data))))->num_pkt;
            temp = temp->next;
        }

        printf(HASH_TABLE_BORDER);
    }

    printf("| %*s | %-*u |\n", INDEX_FLD + SRC_IP_FLD + DEST_IP_FLD + 4, "Total Packets", CNT_FLD, total_pkt);
    printf("+----------------------------------------------+----------------+\n\n");

    return;
}

bool ipv4_compare(void* ip_pair1, void* ip_pair2)
{
    return (bool)(*((uint64_t*)ip_pair1) == *((uint64_t*)ip_pair2));
}

void* get_key(ipv4_header_t* ip_hdr)
{
    ipv4_t* src_dest_ip = NULL;

    src_dest_ip = (ipv4_t*)calloc(2, sizeof(ipv4_t));
    src_dest_ip[SOURCE_IP] =  ip_hdr->ip_src;
    src_dest_ip[DEST_IP] = ip_hdr->ip_dest;

    return (void*)src_dest_ip;
}

uint32_t ipv4_hash(void* key, uint32_t hash_table_size)
{
    uint64_t src_ip_hash = 0;
    uint64_t dest_ip_hash = 0;
    uint64_t hash_value = 0;

    src_ip_hash = ((ipv4_t*)key)[SOURCE_IP] * PRIME1;
    dest_ip_hash = ((ipv4_t*)key)[DEST_IP]  * PRIME2;
    hash_value = src_ip_hash ^ (dest_ip_hash << 1);
    hash_value ^= hash_value >> 16;
    hash_value *= PRIME1;
    hash_value ^= hash_value >> 16;

    return hash_value % hash_table_size;
}
