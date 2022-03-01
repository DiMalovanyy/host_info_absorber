#include "resolve_dns.h"

#include <resolve.h>
#include <stdbool.h>
#include <netdb.h>
#include <arpa/nameser.h>

extern int h_error; // DNS client error


void resolve(char *host_name, bool debug) {
	


}

int find_nameservers(char* domain, char *nsList[], int* nsNum) {
	union {
		HEADER hdr;				 //defined in resolve.h
		u_char buf[NS_PACKETSZ]; //defined in arpa/nameserver.h
	} response;					 //response buffer
	int respone_len;			 //Response buffer length
	
	ns_msg handle; // Responses descriptor
	
	/* Search for NS queries for domain:
	 * We trust thar domain name is full name, so
	 * we can use res_query() not res_search() 
	*/
	if((responseLen = res_query(domain,  // Domain interesting for
							    ns_c_in, // Internet class
								ns_t_ns, // Search for DNS server notes
								(u_char*)&response,
								sizeof(response))) < 0) {
		ns_error(h_errno, domain);
		return -1;
	}

	/* Initialize descriptor with given response.
	 * Descriptor will used later for extract info
	 */
	if
}





