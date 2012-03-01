/* YADA - Yet Another Dropbox App
 * Client Application
 * Author: Sean Barag
 * Drexel University ECE-C433 | Winter 2012
 */

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv[])
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
		printf("OH HAI.\n");
	}
}
