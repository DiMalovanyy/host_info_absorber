#ifndef IP_UTILS_H
#define IP_UTILS_H

#include <arpa/inet.h>
#include <stdio.h>

//Convert address char* to numeric format
//Return -1 on error
//Return INET family on success and fill char *dst
static inline int convert_to_numeric(const char* src_addr, char* dst) {
	
	// Try convert to IPv4
	int res = inet_pton(AF_INET, src_addr, dst);
	if (res == 1) return AF_INET;
	if (res == -1) {
		perror("Convert address to numeric format failure");
		return -1;
	}
	// otherwise res == 0 - Incompatible address family
	
	res = inet_pton(AF_INET6, src_addr, dst);
	if (res == 1) return AF_INET6;
	if (res == -1) {
		perror("Convert address to numeric format failure");
		return -1;
	}
	return -1;
}

static inline int ip_version(const char* src_addr) {
	char buf[16];
	return convert_to_numeric(src_addr, buf);
}





#endif
