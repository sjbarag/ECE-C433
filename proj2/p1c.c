#include "p1cd_common.h"

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
}
