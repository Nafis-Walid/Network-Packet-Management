#ifndef __UDP_PACKET_H__
#define __UDP_PACKET_H__

#include <stdint.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include "debug.h"

#define MAC_SIZE 6
#define IP_SIZE 4
#define ETH_TYPE_SIZE 2

typedef uint8_t byte;
typedef uint32_t ipv4_t;

#pragma pack(1)

typedef struct 
{
    uint16_t length;
    byte data[0];
}byte_data_t;

typedef struct 
{
    byte bytes[MAC_SIZE];
}mac_address_t;

typedef struct
{
    mac_address_t dest_mac;
    mac_address_t src_mac;
    uint16_t eth_type;
}ethernet_header_t;

typedef struct
{
    #if BYTE_ORDER == LITTLE_ENDIAN
        uint8_t ip_hl : 4;                /* Internet Header Length (4 bits) */
        uint8_t ip_version : 4;
    #endif
    #if BYTE_ORDER == BIG_ENDIAN
        uint8_t ip_version : 4;
        uint8_t ip_hl : 4;                /* Internet Header Length (4 bits) */
    #endif
 
    uint8_t ip_tos;                /* Type of Service (8 bits) */
    uint16_t ip_tl;     /* Total Length (16 bits) */
    uint16_t ip_id;   /* Identification (16 bits) */
    uint16_t ip_frag_off_flag;  /* Fragment Offset (13 bits) */
    uint8_t ip_ttl;                /* Time to Live (8 bits) */
    uint8_t ip_protocol;           /* Protocol (8 bits) */
    uint16_t ip_checksum;         /* Header Checksum (16 bits) */
    uint32_t ip_src;           /* Source IP Address (32 bits) */
    uint32_t ip_dest;            /* Destination IP Address (32 bits) */
} ipv4_header_t;

typedef struct
{
    uint16_t udp_src_port;    /* Source port (16 bits) */
    uint16_t udp_dest_port;   /* Destination port (16 bits) */
    uint16_t udp_length;      /* Length of UDP header and data (16 bits) */
    uint16_t udp_checksum;    /* Checksum (16 bits) */
} udp_header_t;

typedef struct
{
    udp_header_t udp_header;
    byte udp_payload[0];
}udp_packet_t;

typedef struct
{
    ipv4_header_t ip_header;
    udp_packet_t ip_payload;
}ipv4_packet_t;

typedef struct
{
    ethernet_header_t eth_header;
    ipv4_packet_t eth_payload;
}ethernet_packet_t;

#pragma pack()

extern ethernet_packet_t* get_ethernet_packet(byte* packet);
extern void print_ethernet_packet(const ethernet_packet_t* eth_packet);
extern void print_ipv4_packet(const ipv4_packet_t *ipv4_packet);
extern uint32_t ipv4_print(void* ipv4_address);
extern byte* get_packet(FILE* pkt_file);

#endif/*__UDP_PACKET_H__*/