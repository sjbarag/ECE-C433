#include "p1cd_common.h"

/* pcap_loop callback function */
void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	/* printing the full timestamp.  Code from Mark Rushakoff's answer @
	 * http://stackoverflow.com/questions/1469495
	 * Note that this produces stamps in Unix time */
	printf("%ld.%06ld\n", h->ts.tv_sec, h->ts.tv_usec);
}
