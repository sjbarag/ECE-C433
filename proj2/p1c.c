#include "p1c.h"

/* pcap_loop callback function */
void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	++pkt_count;
	if( pkt_count == 1 ) /* print on first packet only */
	{
		printf("Number of Packets:\t");
	}
	else /* delete the old characters and overwrite */
	{
		if( pkt_count < 10 )
			printf("\b");
		else if( pkt_count < 100 )
			printf("\b\b");
		else if( pkt_count < 1000 )
			printf("\b\b\b");
		else if( pkt_count < 10000 )
			printf("\b\b\b\b");
	}

	fflush(stdout);
	printf("%d", pkt_count);

	/* log timing data */
	fprintf(data_out, "%ld.%06ld\n", h->ts.tv_sec, h->ts.tv_usec);
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
	status = pcap_loop( nd, 0, proc_pkt, NULL );

	/* ----- wait for thread to rejoin ----- */
	pthread_join( timer, NULL );

	/* ----- close the file ----- */
	if( fclose(data_out) == EOF )
	{
		fprintf(stderr, "Error: something went wrong when closing the file!\nExiting...\n");
		exit(-1);
	}
}
