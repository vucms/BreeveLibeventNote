Info:  libevent 2.1.x
Auth: breeve
Ref:  Doc 

Seems like these:
#include <event2/event.h>

struct event_base *mybase = event_base_new();
struct event *ev = event_new(mybase, fd, event_cb, arg);
int ret = event_add(ev, &tv);
ret = event_base_dispatch();

struct event_base event_base_new (void);
struct event* event_new(struct event_base* base, evutil_socket_t fd, event_callback_fn cb, void *arg);
int event_add( struct event * ev , const struct timeval * timeout );
int event_base_dispatch(struct event_base *base);


