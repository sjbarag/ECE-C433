#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pcap/pcap.h>


void main()
{
	/* check for root uid :) */
	if( getuid() != 0 )
	{
		fprintf(stderr, "Error: You must this application as root, either via sudo or su.\nExiting...\n");
		exit(-1);
	}

	/* return for various functions */
	int status = 0;

	/* error buffer */
	char errbuf[PCAP_ERRBUF_SIZE];

	/* linked list of all devices that can be opened with pcap_open_live and
	 * friends */
	pcap_if_t *all_devs;

	/* iterator for all_devs.  I want to maintain that list's HEAD */
	pcap_if_t *devs_it;

	/* populate all_devs */
	status = pcap_findalldevs(&all_devs, errbuf);

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
		printf("Name\tDescription\n------\t-----------\n");
		devs_it = all_devs;
		while( devs_it != NULL )
		{
			printf("%6s\t%s\n", devs_it->name, devs_it->description);
			devs_it = devs_it->next;
		}
	}

	/* free the list of devices */
	pcap_freealldevs(all_devs);
}
