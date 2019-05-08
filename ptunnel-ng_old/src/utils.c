/*
 * utils.c
 * ptunnel is licensed under the BSD license:
 *
 * Copyright (c) 2004-2011, Daniel Stoedle <daniels@cs.uit.no>,
 * Yellow Lemon Software. All rights reserved.
 *
 * Copyright (c) 2017-2019, Toni Uhlig <matzeton@googlemail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the Yellow Lemon Software nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Contacting the author:
 * You can get in touch with me, Daniel Stødle (that's the Norwegian letter oe,
 * in case your text editor didn't realize), here: <daniels@cs.uit.no>
 *
 * The official ptunnel website is here:
 * <http://www.cs.uit.no/~daniels/PingTunnel/>
 *
 * Note that the source code is best viewed with tabs set to 4 spaces.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#ifdef HAVE_BSD_STDLIB_H
#include <bsd/stdlib.h>
#endif

#ifndef WIN32
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#else
#include <ws2tcpip.h>
#endif
#include <sys/time.h>

#include "utils.h"
#include "options.h"

void pt_log(int level, const char *fmt, ...) {
	va_list args;
	const char *header[] = { "[err]: ",
	                         "[inf]: ",
	                         "[evt]: ",
	                         "[vbs]: ",
	                         "[dbg]: ",
	                         "[xfr]: " };
#ifndef WIN32
	int syslog_levels[] = {LOG_ERR, LOG_NOTICE, LOG_NOTICE, LOG_INFO, LOG_DEBUG, LOG_DEBUG};
#endif /* !WIN32 */

	if (level <= opts.log_level) {
		va_start(args, fmt);
#ifndef WIN32
		if (opts.use_syslog) {
			char log[255];
			int header_len;
			header_len = snprintf(log,sizeof(log),"%s",header[level]);
			vsnprintf(log+header_len,sizeof(log)-header_len,fmt,args);
			syslog(syslog_levels[level], "%s", log);
		}
		else
#endif /* !WIN32 */
			fprintf(opts.log_file, "%s", header[level]), vfprintf(opts.log_file, fmt, args);
		va_end(args);
#ifndef WIN32
		if (opts.log_file != stdout && !opts.use_syslog)
#else
		if (opts.log_file != stdout)
#endif
			fflush(opts.log_file);
	}
}

double time_as_double(void) {
	double          result;
	struct timeval  tt;

	gettimeofday(&tt, 0);
	result = (double)tt.tv_sec + ((double)tt.tv_usec / (double)10e5);
	return result;
}

int host_to_addr(const char *hostname, uint32_t *result)
{
	int ret;
	struct addrinfo *addrs = NULL;
	struct addrinfo hints;
	struct sockaddr_in *addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	if ((ret = getaddrinfo(hostname, NULL, &hints, &addrs)) != 0)
		return ret;
	addr = (struct sockaddr_in *) addrs->ai_addr;
	*result = *(uint32_t *) &addr->sin_addr;
	freeaddrinfo(addrs);

	return 0;
}

#if 0
static const char hextab[] = "0123456789ABCDEF";

void print_hexstr(unsigned char *buf, size_t siz) {
	char *out = (char *) calloc(3, siz+1);
	unsigned char high, low;

	for (size_t i = 0; i < siz; ++i) {
		high = (buf[i] & 0xF0) >> 4;
		low  = buf[i] & 0x0F;
		out[i  ] = hextab[high];
		out[i+1] = hextab[low];
		out[i+2] = ' ';
	}

	printf("%s\n", out);
	free(out);
}
#endif

int pt_random(void) {
#ifdef HAVE_ARC4RANDOM
	return arc4random();
#else
#ifdef HAVE_RANDOM
#ifndef TIME_UTC
#define TIME_UTC 1
#endif
	struct timespec ts;

	assert(timespec_get(&ts, TIME_UTC));
	srandom(ts.tv_nsec ^ ts.tv_sec);
	return random();
#else
	srand(time(0));
	return rand();
#endif
#endif
}
