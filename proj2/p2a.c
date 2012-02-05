#include "p1c.h"
#include <netinet/ether.h>

/* pcap_loop callback function */
void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	struct ether_header* eth_hdr = (struct ether_header*)bytes;
	printf("src:\t%017s\n", ether_ntoa( (struct ether_addr*)eth_hdr->ether_shost ) );
	printf("dst:\t%017s\n", ether_ntoa( (struct ether_addr*)eth_hdr->ether_dhost ) );
}

/* pthread function to cancel the pcap_loop after a set amount of time
 * in_args must be cast to type THREAD_ARGS, defined above */
void *sniff_timer(void *in_args)
{
	THREAD_ARGS *l_args = (THREAD_ARGS *) in_args;
	sleep( l_args->time );
	pcap_breakloop( l_args->nd );
	if( pkt_count == 0 )
		printf("%d\n", pkt_count); /* it never printed in proc_pkt */
	else
		printf("\n");     /* add final newline for proc_pkt */
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

	device = (char *)malloc(sizeof(char)*8);
	sleep_time = (argc == 2) ? atoi(argv[1]) : 5;

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
	printf("Using device:\t   %6s\n", device);

	/* ----- fill thread args ----- */
	t_args = (THREAD_ARGS *)malloc(sizeof(THREAD_ARGS));
	t_args->time = sleep_time;
	t_args->nd   = nd;

	/* ----- open file for saving data ----- */
	data_out = fopen("timing_data.txt", "w+b");

	/* ----- spawn thread ----- */
	if( (pthread_create( &timer, NULL, sniff_timer, (void *)t_args)) != 0)
	{
		fprintf(stderr, "Error: could not create thread!\nExiting...\n");
		exit(-1);
	}

	/* ----- start loop ----- */
	status = pcap_loop( nd, 0, proc_pkt, "Herp derp" );

	/* ----- wait for thread to rejoin ----- */
	pthread_join( timer, NULL );

	/* ----- close the file ----- */
	if( fclose(data_out) == EOF )
	{
		fprintf(stderr, "Error: something went wrong when closing the file!\nExiting...\n");
		exit(-1);
	}
}
