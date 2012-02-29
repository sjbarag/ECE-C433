//Originally code by Dr.Harish Sethu. ECE-C433. Spring 2006-2007
//to run: gcc incrementServer.c -lnsl -lsocket -o incrementServer
//Takes no parameters

#include <stdio.h>            /* needed for file io */
#include <sys/socket.h>       /* needed for socket(), bind() and connect() */
#include <arpa/inet.h>        /* needed for socket structures and byte-order operations */
#include <stdlib.h>           /* needed for several useful functions such as atoi() */
#include <string.h>           /* needed for memset and related string functions */
#include <unistd.h>           /* needed for close() */

#define BACKLOG 5
#define RECVBUFSIZE 64

int main( int argc, char *argv[] )
{

	/* client and server sockets and addresses */
	int serverSocket;
	int clientSocket;
	int server_port;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	unsigned int clientAddrLen;

	/* buffer into which number received from client is stored */
	char recvBuffer[RECVBUFSIZE];
	/* number of bytes received from the client */
	int numBytesRcvd;
	/* character used to store each byte received from client */
	char c;

	/* buffer in which the incremented number is stored */
	char incNumberString[RECVBUFSIZE];

	/* number of bytes to send to client in order to send the incremented number */
	unsigned int incNumberStringLen;

	if ( argc != 2 )
	{
		fprintf( stderr, "usage: incrementServer <ServerPortNumber>\n" );
		fprintf( stderr, "Defaulting to port 10001\n");
		server_port = 10001;
	}
	else if ( argc == 2 )
		server_port = atoi( argv[1] );

	/* create server side socket */
	serverSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( serverSocket < 0 )
	{
		perror( "socket error" );
		exit( 1 );
	}
	printf( "Socket created.\n" );

	/* zer out socket address */
	bzero( &serverAddr, sizeof( serverAddr ) );

	/* set socket address to server address and port. Note that the constant
	   INADDR_ANY implies that the client may reach the server through any
	   incoming interface. */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	serverAddr.sin_port = htons( server_port );

	/* bind server address and port to server side socket */
	if ( bind( serverSocket, (struct sockaddr *) &serverAddr, sizeof( serverAddr ) ) < 0 )
	{
		perror( "bind() error" );
		exit( 1 );
	}
	printf( "Address binding successful.\n" );

	/* call listen on the server side socket */
	if ( listen( serverSocket, BACKLOG ) < 0 )
	{
		perror( "listen() error" );
		exit( 1 );
	}
	printf( "Server process is listening for connection requests.\n" );

	/* Now, prepare to accept connection requests from clients */
	clientAddrLen = sizeof( clientAddr );

	/* You run the for loop forever since as soon as the server serves a client,
	   it should get ready to serve another client. */
	for (;;)
	{
		clientSocket = accept( serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen );
		if ( clientSocket < 0 )
		{
			perror( "accept() error" );
			exit( 1 );
		}

		/* Begin receiving bytes from the client */
		numBytesRcvd = 0;
		c = ' ';
		while ( c != '\0' )
		{
			if ( read( clientSocket, &c, 1 ) == 1 )
				{
					recvBuffer[ numBytesRcvd ] = c;
					numBytesRcvd++;
				}
			else
				{
					perror( "read() error" );
					exit( 1 );
				}
		}
		printf( "Received number %s from a client.\n", recvBuffer );

		/* increment received number and store it as a string */
		sprintf( incNumberString, "%d", atoi( recvBuffer ) + 1 );

		/* number of bytes to send to client is one more than the number of
		   digits in the incremented number since we also count the delimiter '\0' */
		incNumberStringLen = strlen( incNumberString ) + 1;

		/* send incremented number to client */
		if ( write( clientSocket, incNumberString, incNumberStringLen ) != incNumberStringLen )
		{
			perror( "send error" );
			exit( 1 );
		}
		printf( "Returned number %s to the client.\n", incNumberString );

		close( clientSocket );
	}
}
