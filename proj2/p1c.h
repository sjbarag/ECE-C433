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

/* pcap_loop callback function */
void proc_pkt(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);

/* pthread function to cancel the pcap_loop after a set amount of time
 * in_args must be cast to type THREAD_ARGS, defined above */
void *sniff_timer(void *in_args);

/* --- misc --- */
int sleep_time;                   /* time to sniff */
int status = 0;                   /* return for various functions */
int got_device = 0;               /* flag */

/* --- pcap specific stuff --- */
pcap_if_t *all_devs;              /* linked list of all devices that can be opened
                                   *     with pcap_open_live and friends */
pcap_if_t *devs_it;               /* iterator for all_devs, to presesrve
                                   *     all_devs' HEAD */
pcap_addr_t *temp_addr;           /* temporary storage for an interface's
                                   *     address info */
pcap_t *nd;                       /* network descriptor */
char *device;                     /* device name.  8 chars is arbitrary */
int pkt_count = 0;                /* number of packets sniffed */
char errbuf[PCAP_ERRBUF_SIZE];    /* error buffer */

/* --- pthread stuff --- */
pthread_t timer;                  /* threadID for the timer */
THREAD_ARGS *t_args;              /* thread arguments structure */
