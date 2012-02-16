/* ECE-C433 Project 2
 * Part 1b
 * Author: Sean Barag
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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

	/* temporary storage for an interface's address info */
	pcap_addr_t *temp_addr;

	char *addr;
	char *nm;

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
		devs_it = all_devs;
		printf("Name\tIP address\tNetmask\n------\t---------------\t---------------\n");
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
						if( temp_addr->addr->sa_family == AF_INET && temp_addr->netmask->sa_family == AF_INET )
						{
							/* It seems you can't have two calls to inet_ntoa
							 * in a single printf, or you'll get the first
							 * returned value printed twice.  WEIRD. */
							printf("%6s\t%15s",
							       devs_it->name, // name
							       inet_ntoa( ( (struct sockaddr_in *)temp_addr->addr)->sin_addr ) ); // IP
							printf("\t%15s\n",
							       inet_ntoa( ( (struct sockaddr_in *)temp_addr->netmask)->sin_addr ) ); // netmask
						}
					}
					temp_addr = temp_addr->next;
				}
			}
			devs_it = devs_it->next;
		}
	}

	/* free the list of devices */
	pcap_freealldevs(all_devs);
}
