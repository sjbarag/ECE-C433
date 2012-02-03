#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

int main()
{
	struct pcap_if *found_devices;
	char errbuf[PCAP_ERRBUF_SIZE];
	int success = 0;

	success = pcap_findalldevs(&found_devices, errbuf);

	if( success < 0 )
	{
		printf("something went wrong. can't open device\n");
	}

	if( found_devices != NULL )
	{
		while( found_devices != NULL )
		{
			printf("%s\n", found_devices->name);

			found_devices = found_devices->next;
		}
	}
	return 0;
}
