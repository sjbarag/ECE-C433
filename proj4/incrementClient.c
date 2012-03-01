//Originally code by Dr.Harish Sethu. ECE-C433. Spring 2006-2007
//to run: gcc incrementClient.c -lnsl -lsocket -o incrementClient
//Accepts as parameter the IP address of the server and the number to be incremented

#include <stdio.h>         /* needed for file io */
#include <sys/socket.h>    /* needed for socket(), bind() and connect() */
#include <arpa/inet.h>     /* needed for socket structures and byte-order operations */
#include <stdlib.h>        /* needed for several useful functions such as atoi() */
#include <string.h>        /* needed for memset and related string functions */
#include <unistd.h>        /* needed for close() */

#define RECVBUFSIZE 64

int main( int argc, char *argv[] )
{
	/* Socket descriptor and address */
	int sockfd;
	struct sockaddr_in serverAddr;

	/* the number to send to the server */
	int send_number;

	/* the number to receive from the server */
	int recv_number;

	/* number of bytes received from the server */
	int numBytesRcvd = 0;

	/* port on the server to use */
	int server_port;

	if ( argc < 3 )
	{
		fprintf( stderr, "usage: incrementClient <ServerIPaddress> [ServerPortNumber] <number>\n" );
		exit( 0 );
	}
	else if ( argc == 3 )
	{
		fprintf( stderr, "Defaulting to port 10001\n");
		server_port = 10001;

		/* set number to send */
		send_number = atoi( argv[2] );
	}
	else if ( argc == 4 )
	{
		server_port = atoi( argv[2] );

		/* set number to send */
		send_number = atoi( argv[3] );
	}

	/* create socket */
	sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( sockfd < 0 )
	{
		perror( "socket error" );
		exit( 1 );
	}
	printf( "Created socket.\n" );

	/* zero out the socket address */
	bzero( &serverAddr, sizeof( serverAddr ) );

	/* set socket address to server address and port */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( server_port );
	if ( inet_pton( AF_INET, argv[ 1 ], &serverAddr.sin_addr ) <= 0 )
	{
		perror( "inet_pton() failed" );
		exit( 0 );
	}

	/* connect to server */
	if ( connect( sockfd, (struct sockaddr *) &serverAddr, sizeof( serverAddr ) ) < 0 )
	{
		perror( "connect error" );
		exit( 1 );
	}
	printf( "Connected to server.\n" );



	/* an integer is a fixed-width data structure, so we can just use sizeof */
	if ( write( sockfd, &send_number, sizeof(int) ) != sizeof(int) )
	{
		perror( "write error" );
		exit( 1 );
	}
	printf( "Sent number %d to server.\n", send_number );

	/* receive sizeof(int) bytes */
	if ( read( sockfd, &recv_number, sizeof(int) ) != sizeof(int) )
	{
		perror( "read() error" );
		exit( 1 );
	}
	printf( "Received incremented number %d from server.\n", recv_number );

	/* close socket and exit */
	close( sockfd );
	exit( 0 );
}
