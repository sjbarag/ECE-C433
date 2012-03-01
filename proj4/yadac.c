/* YADA - Yet Another Dropbox App
 * Client Application
 * Author: Sean Barag
 * Drexel University ECE-C433 | Winter 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>


#define DIGEST_NAME "md5"

int main( int argc, char *argv[])
{
	if( argc != 2 )
	{
		fprintf(stderr, "Usage: yadac [option]\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "    l, link  \tLink the current directory with the remote server\n");
		fprintf(stderr, "    u, unlink\tUnlink the current directory with the remote server\n");
		fprintf(stderr, "    s, sync  \tSynchronize the files within the current directory with those on the server\n");
		fprintf(stderr, "    i, info  \tPrint output state of current folder\n");
		exit(1);
	}
	else
	{
		char *buf = (char *) malloc( 25 * sizeof(char) );

		EVP_MD_CTX mdctx;
		const EVP_MD *md;
		unsigned char hashed[EVP_MAX_MD_SIZE];
		int md_len;

		OpenSSL_add_all_digests();

		md = EVP_get_digestbyname( DIGEST_NAME );

		EVP_MD_CTX_init( &mdctx );
		EVP_DigestInit_ex( &mdctx, md, NULL );

		/* read 25 characters at a time */
		while( fgets(buf, 25, stdin) != NULL )
		{
			printf("Read:\t%s\n", buf);
			EVP_DigestUpdate(&mdctx, buf, strlen(buf));
		}
		EVP_DigestFinal_ex( &mdctx, hashed, &md_len );
		printf("Digest is: ");
		int i = 0;
		for( i; i < md_len; i++ )
			printf("%02x", hashed[i]);
		printf("\n");
	}
}
