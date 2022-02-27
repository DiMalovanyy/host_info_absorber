#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <string.h>

typedef struct {
	uint16_t port_num;
	char* port_description;
} protocol_t;

static const protocol_t protocols[] = {
	{21, "FTP"},
	{22, "SSH"},
	{25, "SMTP (sending email)"},
	{53, "DNS (domain name service)"},
	{80, "HTTP (web server)"},
	{110, "POP3 (email inbox)"},
	{123, "NTP (Network Time Protocol)"},
	{143, "IMAP (email inbox)"},
	{443, "HTTPS (secure web server)"},
	{465, "SMTPS (send secure email)"},
	{631, "CUPS (print server)"},
	{993, "IMAPS (secure email inbox)"},
	{995, "POP3 (secure email inbox)"}
};

static const uint32_t protocol_size = 13;

static inline int get_protocol_by_port(uint16_t port_num, char *dst)  {
	uint32_t i;
	for (i = 0; i < protocol_size; i++) {
		if (protocols[i].port_num == port_num) {
			strcpy(dst, protocols[i].port_description);
			return 0;
		}
	}
	return -1;
}

static inline void get_all_ports(uint16_t* dst) {
	uint32_t i;
	for(i = 0; i < protocol_size; i++) {
		dst[i] = protocols[i].port_num;
	}
}

#endif
