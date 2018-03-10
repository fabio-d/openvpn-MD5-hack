#define _GNU_SOURCE // for RTLD_NEXT

#include <dlfcn.h>
#include <stdio.h>

#include <openssl/ssl.h>

SSL *SSL_new(SSL_CTX *ctx)
{
	typeof(SSL_new) *SSL_new = dlsym(RTLD_NEXT, "SSL_new");

	SSL *s = SSL_new(ctx);
	fprintf(stderr, "SSL_new: Downgrading security level to 0\n");
	SSL_set_security_level(s, 0);

	return s;
}
