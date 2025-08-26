#include "udp-packet.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "hash-table.h"


#define CHAR_NEW_LINE '\n'
#define CHAR_NULL '\0'
#define CHAR_SPACE ' '
#define BYTE_DELIMITER " "
#define SEPARATOR CHAR_NEW_LINE
#define MAX_LINE_LENGTH 75
#define PACKET_ADDRESS_LEN 6
#define SPACE_PER_HEXBYTE 3
#define BUFFER_LEN 100
#define CARRIAGE_RETURN 13
#define BYTES_PER_LINE 16
#define UDP_PACKET_MAX_SIZE 65535


void process_ethernet_packet(ethernet_packet_t* eth_packet);
void process_ipv4_packet(ipv4_packet_t* ipv4_packet);
void process_udp_packet(udp_packet_t* udp_packet);
void print_ethernet_packet(const ethernet_packet_t* eth_packet);
void print_udp_packet(const udp_packet_t* udp_packet);
void print_mac(const mac_address_t* mac_address);
static bool hex_byte_to_decimal(const char* hex_byte, byte* byt);

ethernet_packet_t *get_ethernet_packet(byte* packet)
{
    ethernet_packet_t* eth_packet = NULL;

    if(packet == NULL)
        return NULL;

    process_ethernet_packet(eth_packet = (ethernet_packet_t*)packet);
    
    return eth_packet;
}

void process_ethernet_packet(ethernet_packet_t* eth_packet)
{
    eth_packet->eth_header.eth_type = ntohs(eth_packet->eth_header.eth_type);

    DEBUG_BLOCK(print_ethernet_packet((const ethernet_packet_t*)eth_packet););

    process_ipv4_packet(&eth_packet->eth_payload);

    return;
}

void process_ipv4_packet(ipv4_packet_t *ipv4_packet)
{
    ipv4_packet->ip_header.ip_tl = ntohs(ipv4_packet->ip_header.ip_tl);
    ipv4_packet->ip_header.ip_id = ntohs(ipv4_packet->ip_header.ip_id);
    ipv4_packet->ip_header.ip_checksum = ntohs(ipv4_packet->ip_header.ip_checksum);

    DEBUG_BLOCK(print_ipv4_packet(ipv4_packet););

    process_udp_packet(&ipv4_packet->ip_payload);

    return;
}

void process_udp_packet(udp_packet_t *udp_packet)
{
    udp_packet->udp_header.udp_src_port = ntohs(udp_packet->udp_header.udp_src_port);
    udp_packet->udp_header.udp_dest_port = ntohs(udp_packet->udp_header.udp_dest_port);
    udp_packet->udp_header.udp_length = ntohs(udp_packet->udp_header.udp_length);
    udp_packet->udp_header.udp_checksum = ntohs(udp_packet->udp_header.udp_checksum);
    DEBUG_BLOCK(print_udp_packet(udp_packet););

    return;
}

void print_ethernet_packet(const ethernet_packet_t* eth_packet)
{
    if(eth_packet == NULL)
    {
        DEBUG_PRINT("Header is NULL\n");
        return;
    }

    printf("Destination MAC Address   - ");
    print_mac(&eth_packet->eth_header.dest_mac);
    printf("\nSource MAC Address        - ");
    print_mac(&eth_packet->eth_header.src_mac);
    printf("\nEthernet Type             - %#.4x\n", eth_packet->eth_header.eth_type);

    return;
}

void print_ipv4_packet(const ipv4_packet_t *ipv4_packet)
{
    printf
    (
        "Version            : %hhu\n"
        "Header length      : %hhu\n"
        "Type of Service    : %hhu\n"
        "Total Length       : %hu\n"
        "Identification     : %hu\n"
        "Fragment_offset    : %hu\n"
        "Time to Live       : %hhu\n"
        "Protocol           : %hhu\n"
        "ip_checksum        : %#.4x\n"
        "Source IP          : ",
        ipv4_packet->ip_header.ip_version,
        ipv4_packet->ip_header.ip_hl,
        ipv4_packet->ip_header.ip_tos,
        ipv4_packet->ip_header.ip_tl,
        ipv4_packet->ip_header.ip_id,
        ipv4_packet->ip_header.ip_frag_off_flag,
        ipv4_packet->ip_header.ip_ttl,
        ipv4_packet->ip_header.ip_protocol,
        ipv4_packet->ip_header.ip_checksum
    );

    ipv4_print((void*)&ipv4_packet->ip_header.ip_src);
    printf("\nDestination IP     : ");
    ipv4_print((void*)&ipv4_packet->ip_header.ip_dest);
    printf("\n");

    return;
}

void print_udp_packet(const udp_packet_t* udp_packet)
{
    printf
    (
        "Source Port        : %hu\n"
        "Destination Port   : %hu\n"
        "Length             : %hu\n"
        "Checksum           : %#.4x\n",
        udp_packet->udp_header.udp_src_port,
        udp_packet->udp_header.udp_dest_port,
        udp_packet->udp_header.udp_length,
        udp_packet->udp_header.udp_checksum
    );

    return;
}

void print_mac(const mac_address_t* mac_address)
{
    uint8_t i = 0;

    printf("%.2x", mac_address->bytes[0]);

    for(i = 1; i < MAC_SIZE; i++)
        printf(":%.2x", mac_address->bytes[i]);
    
    fflush(stdout);

    return;
}

uint32_t ipv4_print(void* ipv4_address)
{
    byte* byt = NULL;
    uint32_t num_chars = 0;

    byt = ipv4_address;
    num_chars = printf("%hhu.%hhu.%hhu.%hhu", byt[0], byt[1], byt[2], byt[3]);
    
    fflush(stdout);

    return num_chars;
}

byte* get_packet(FILE* pkt_file)
{
    char* tmp = NULL;
    byte* packet = NULL;
    uint32_t num_bytes = 0;
    static bool line_present = true;
    char line[MAX_LINE_LENGTH] = {0};

    if(!line_present)
        return NULL;

    packet = (byte*)malloc(UDP_PACKET_MAX_SIZE);

    while((line_present = (fgets(line, MAX_LINE_LENGTH, pkt_file) != NULL)) && strlen(line) > PACKET_ADDRESS_LEN)
    {
        *(line + PACKET_ADDRESS_LEN + SPACE_PER_HEXBYTE * BYTES_PER_LINE) = CHAR_NULL;
        tmp = line + strlen(line);

        while(*(--tmp) == CHAR_SPACE);/*trim spaces*/

        *(tmp + 1) = CHAR_NULL;
        tmp = strtok(line + PACKET_ADDRESS_LEN, BYTE_DELIMITER);

        while(tmp != NULL)
        {
            if(hex_byte_to_decimal(tmp, packet + num_bytes) == false)
                return NULL;

            num_bytes++;
            tmp = strtok(NULL, BYTE_DELIMITER);
        }
    }

    packet = realloc(packet, num_bytes);

    return packet;
}

static bool hex_byte_to_decimal(const char* hex_byte, byte* byt)
{
    if(strlen(hex_byte) != 2)
        return false;

    if(sscanf(hex_byte, "%2hhx", byt) != 1)
        return false;

    return true;
}
