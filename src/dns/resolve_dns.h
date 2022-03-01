#ifndef RESOLVE_DNS_H
#define RESOLVE_DNS_H

// What shoud this object file do:
// Input: host_name
// Output: 
//		list if DNS servers for host_name
//		list os ip addresses for host_name

// Library: libresolv -lresolv (DNS resolver library)
#include <resolve.h>
#include <netdb.h>
					
#define MAX_DNS_SERVERS 20

void ns_error(int h_errno, char* domain);

void resolve(char *host_name);

/* Search all nameservers for the current zone and save to nsList */
void find_nameservers(char* domain, char *nsList[], int* nsNum);

#endif
