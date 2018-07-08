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

	fprintf(stderr, "SSL_new: Disabling time checks\n");
	X509_VERIFY_PARAM *param = SSL_get0_param(s);
	X509_VERIFY_PARAM_set_flags(param, X509_V_FLAG_NO_CHECK_TIME);

	return s;
}
