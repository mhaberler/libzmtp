#include "zmtp_classes.h"


unsigned short port = 4711;
char * uri= "127.0.0.1";
int count = 3;

int main (int argc, char *argv [])
{
    int rc;
    int flags;

    zmtp_dealer_t *dealer = zmtp_dealer_new ();
    assert(dealer != NULL);
    rc = zmtp_dealer_tcp_connect (dealer, uri, port);
    assert(rc == 0);
    for (int i = 0; i < count; i++) {
	zmtp_msg_t *tx = zmtp_msg_from_const_data (0, "hello", 6);
	assert (tx);
	rc = zmtp_dealer_send (dealer, tx);
	assert(rc == 0);
	zmtp_msg_destroy (&tx);
	size_t size;
	zmtp_msg_t *rx;
	do {
	    rx = zmtp_dealer_recv (dealer);
	    assert(rx != 0);
	    size = zmtp_msg_size (rx);
	    flags = zmtp_msg_flags (rx);
	    printf("got %zu: '%.*s' flags=%d\n",
		   size, (int) size,
		   zmtp_msg_data (rx), flags);
	} while (flags & ZMTP_MSG_MORE);
	zmtp_msg_destroy (&rx);
    }
    return 0;
}
