/*
 * Copyright (C) 2014, 2015  Internet Systems Consortium, Inc. ("ISC")
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
#include "arpananme.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int arpaname(const char* ip_address, char* dst) {
	unsigned char buf[16];
	int i;

	if (inet_pton(AF_INET6, ip_address, buf) == 1) {
		char ipv6_res[257];
		for (i = 15; i >= 0; i--) {
			char oct_buf[16];
			sprintf(oct_buf, "%X.%X.", buf[i] & 0xf,
				(buf[i] >> 4) & 0xf);
			strcat(ipv6_res, oct_buf);
		}
		sprintf(dst, "%s%s", ipv6_res, "IP6.ARPA");
		return 0;
	}
	if (inet_pton(AF_INET, ip_address, buf) == 1) {
		sprintf(dst, "%u.%u.%u.%u.IN-ADDR.ARPA",
			buf[3], buf[2], buf[1], buf[0]);
		return 0;
	}
	return -1;

}
