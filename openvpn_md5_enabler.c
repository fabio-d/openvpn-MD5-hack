#define _GNU_SOURCE // for RTLD_NEXT

#include <dlfcn.h>
#include <stdio.h>

#include <openssl/ssl.h>

SSL_CTX *SSL_CTX_new(const SSL_METHOD *method)
{
	typeof(SSL_CTX_new) *real_SSL_CTX_new = dlsym(RTLD_NEXT, "SSL_CTX_new");

	SSL_CTX *ctx = real_SSL_CTX_new(method);
	SSL_CTX_set_security_level(ctx, 0);

	return ctx;
}
