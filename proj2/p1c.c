#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <pthread.h>

/* network descriptor */
pcap_t *nd;


void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	printf("Got a packet!\n");
}

/*void sniff_timer(void *args)
{
	printf("D/sniff_timer: Starting timer...\n");
	sleep( atoi(argv[1]) );

	printf("D/sniff_timer: Canceling loop!\n");
}*/



void main( int argc, char* argv )
{
	/* check for root uid :) */
	if( getuid() != 0 )
	{
		fprintf(stderr, "Error: You must this application as root, either via sudo or su.\nExiting...\n");
		exit(-1);
	}

	/* check argc */
	if( argc != 2 )
	{
		fprintf(stderr, "Usage: ./p1c <number of seconds to sniff>\nExiting...\n");
		exit(-1);
	}

	int status = 0;                   /* return for various functions */
	char errbuf[PCAP_ERRBUF_SIZE];    /* error buffer */
	int got_device = 0;               /* flag */

	pcap_if_t *all_devs;              /* linked list of all devices that can be opened
	                                   *   with pcap_open_live and friends */
	pcap_if_t *devs_it;               /* iterator for all_devs, to presesrve
	                                   *   all_devs' HEAD */
	pcap_addr_t *temp_addr;           /* temporary storage for an interface's
	                                   *   address info */

	char *device = (char *)malloc(sizeof(char)*8); /* device name.  8 chars is arbitrary */


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

	/* ----- start loop ----- */
	status = pcap_loop( nd, 0, proc_pkt, NULL );

	if( status == 0 )
		printf("count exhausted");
	else if( status == -1 )
		fprintf(stderr, "pcap_loop threw error:\n\t%s\n", errbuf);
	else if( status == -2 )
		fprintf(stderr, "Loop terminated by pcap_breakloop\n");

}
