//
// Copyright 2021 Staysail Systems, Inc. <info@staysail.tech>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>

#include <zephyr.h>
#include <sys/__assert.h>

static void thread_ping(void *arg)
{
	int rc = 0;
	nng_socket sock;

	printf("starting ping\n");
	nng_thread_set_name(NULL, "nng:ping");

	rc = nng_req0_open(&sock);
	__ASSERT(rc == 0, "failed opening socket");
	rc = nng_dial(sock, "inproc://server", NULL, 0);
	__ASSERT(rc == 0, "failed dialing");
	nng_setopt_ms(sock, NNG_OPT_SENDTIMEO, 1000);

	while (rc == 0) {
		char *buf = NULL;
		size_t sz = 0;

		rc = nng_send(sock, "PING", 4, 0);

		if (rc == 0) {
			rc = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC);

			if (rc == 0) {
				printf("ping: received %.*s\n", (int) sz, buf);
				nng_free(buf, sz);
			}
		}
		nng_msleep(100);
	}
	printf("ping: last error: %s\n", nng_strerror(rc));
	nng_close(sock);
}

static void thread_pong(void *arg)
{
	int rc = 0;
	nng_socket sock;
	int n = 10;

	printf("starting pong\n");
	nng_thread_set_name(NULL, "nng:pong");

	rc = nng_rep0_open(&sock);
	__ASSERT(rc == 0, "failed opening socket");
	rc = nng_listen(sock, "inproc://server", NULL, 0);
	__ASSERT(rc == 0, "failed dialing");

	while (n-- && rc == 0) {
		char *buf = NULL;
		size_t sz = 0;

		rc = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC);

		if (sz == 4) {
			printf("pong: received %.*s\n", 4, buf);
			buf[1] = 'O';
			rc = nng_send(sock, buf, sz, NNG_FLAG_ALLOC);
			__ASSERT(rc == 0, "failed send");
			continue;
		}
		nng_free(buf, sz);
	}
	printf("pong: last error: %s\n", nng_strerror(rc));
	nng_close(sock);
}

void main(void)
{
	int stop_stats = 0;
	nng_thread *ping = NULL;
	nng_thread *pong = NULL;
	nng_thread *stats = NULL;

	printf("starting threads\n");

	nng_thread_create(&pong, thread_pong, NULL);
	nng_thread_create(&ping, thread_ping, NULL);

	nng_thread_destroy(pong);
	nng_thread_destroy(ping);

	nng_msleep(1000);
	stop_stats = 1;
	nng_thread_destroy(stats);

	nng_fini();
}
