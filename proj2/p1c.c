#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <pthread.h>

#define _REENTRANT        /* for pthreads! */

/* arguments for the sniff_timer thread */
typedef struct thread_args
{
	unsigned int time;    /* time to sleep in seconds */
	pcap_t *nd;           /* network descriptor */
} THREAD_ARGS;


void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	printf("Got a packet!\n");
}

void *sniff_timer(void *in_args)
{
	THREAD_ARGS *l_args = (THREAD_ARGS *) in_args;
	printf("D/sniff_timer: Starting timer...\n");
	sleep( l_args->time );

	printf("D/sniff_timer: Canceling loop!\n");
	pcap_breakloop( l_args->nd );
}



void main( int argc, char** argv )
{
	/* check for root uid :) */
	if( getuid() != 0 )
	{
		fprintf(stderr,
				"Error: You must this application as root, either via sudo %s",
				"or su.\nExiting...\n");
		exit(-1);
	}

	/* --- misc --- */
	int sleep_time = (argc == 2) ? atoi(argv[1]) : 5; /* time to sniff */
	printf("D/main: Sleep time = %d\n", sleep_time);
	int status = 0;                   /* return for various functions */
	int got_device = 0;               /* flag */

	/* --- pcap specific stuff --- */
	pcap_if_t *all_devs;              /* linked list of all devices that can be opened
	                                   *   with pcap_open_live and friends */
	pcap_if_t *devs_it;               /* iterator for all_devs, to presesrve
	                                   *   all_devs' HEAD */
	pcap_addr_t *temp_addr;           /* temporary storage for an interface's
	                                   *   address info */
	pcap_t *nd;                       /* network descriptor */
	char *device = (char *)malloc(sizeof(char)*8); /* device name.  8 chars is arbitrary */
	char errbuf[PCAP_ERRBUF_SIZE];    /* error buffer */

	/* --- pthread stuff --- */
	pthread_t timer;                  /* threadID for the timer */
	THREAD_ARGS *t_args;              /* thread arguments structure */


	/* ----- populate all_devs ----- */
	status = pcap_findalldevs(&all_devs, errbuf);

	/* ----- get a device ----- */
	if( status != 0 )
	{
		fprintf(stderr,
		        "pcap_findalldevs() failed.  The following error was produced:\n\t%s\n",
		        errbuf);
		exit(-1);
	}
	else if( all_devs == NULL )
	{
		printf("No devices were found.  Bummer!\n");
	}
	else
	{
		devs_it = all_devs;
		while( devs_it != NULL )
		{
			/* avoid loopback */
			if( devs_it->flags != PCAP_IF_LOOPBACK )
			{
				temp_addr = devs_it->addresses;
				while( temp_addr != NULL )
				{
					if( temp_addr->netmask != NULL )
					{
						/* check for IPv4 */
						if( temp_addr->addr->sa_family == AF_INET &&
						    temp_addr->netmask->sa_family == AF_INET )
						{
							strcpy(device, devs_it->name);
							got_device = 1;
							break;
						}
					}
					temp_addr = temp_addr->next;
				}
			}
			devs_it = devs_it->next;
		}
	}

	/* ----- free all_devs and devs_it ----- */
	pcap_freealldevs(all_devs);
	free( devs_it );

	if( !got_device )
	{
		fprintf(stderr, "Error: Couldn't get a device.\nExiting...\n");
		exit(-1);
	}

	/* ----- setup capture ----- */
	nd = pcap_open_live(device, BUFSIZ, 0, 2, errbuf);
	printf("Using device:\t%s\n", device);

	/* ----- fill thread args ----- */
	t_args = (THREAD_ARGS *)malloc(sizeof(THREAD_ARGS));
	t_args->time = sleep_time;
	t_args->nd   = nd;

	/* ----- spawn thread ----- */
	if( (pthread_create( &timer, NULL, sniff_timer, (void *)t_args)) != 0)
	{
		fprintf(stderr, "Error: could not create thread!\nExiting...\n");
		exit(-1);
	}

	/* ----- start loop ----- */
	status = pcap_loop( nd, 0, proc_pkt, NULL );

	/* ----- wait for thread to rejoin ----- */
	pthread_join( timer, NULL );
	printf("D/main: timer thread joined successfully!\n");

	if( status == 0 )
		printf("count exhausted");
	else if( status == -1 )
		fprintf(stderr, "pcap_loop threw error:\n\t%s\n", errbuf);
	else if( status == -2 )
		fprintf(stderr, "Loop terminated by pcap_breakloop\n");
}
