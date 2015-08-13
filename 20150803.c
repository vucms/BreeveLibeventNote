libevent2.0-alpha 
date 2015.7.28

[BEGIN]
1.When U use epoll , this is seem like these:
create a sockfd ,then 
fd = socket();
then call getaddrinfo() ,get an struct addrinfo ,for ready to bind()
bind(fd, addrinfo->ai_addr);
listen(fd);
then create a epoll_event
struct epoll_event _myevents[MAX_EVENT_NUMBER]; like event_base in libevent
go on ,,,create a epollfd
call epoll_create(size); 
	   DESCRIPTION:
       epoll_create() creates an epoll "instance", requesting the kernel to allocate
       an event backing store dimensioned for size descriptors.  The size is not the
       maximum  size of the backing store but just a hint to the kernel about how to
       dimension internal structures.  (Nowadays, size is ignored; see NOTES below.)

       epoll_create() returns a file descriptor referring to the new epoll instance.
       This file descriptor is used for all the subsequent calls to the epoll inter-
       face.   When  no  longer required, the file descriptor returned by epoll_cre-
       ate() should be closed by using close(2).  When all file  descriptors  refer-
       ring  to an epoll instance have been closed, the kernel destroys the instance
       and releases the associated resources for re-use.
	       If flags is 0, then, other than the fact that the obsolete size  argument  is
       dropped,  epoll_create1() is the same as epoll_create().  The following value
       can be included in flags to obtain different behavior:

       EPOLL_CLOEXEC
              Set the close-on-exec (FD_CLOEXEC) flag on the  new  file  descriptor.
              See  the  description of the O_CLOEXEC flag in open(2) for reasons why
              this may be useful.

in this ,we should NOTE close-on-exec ,in others,,,
OK ,next call epoll_ctl(epollfd, op, fd, myevents); for add a fd or events to myevents ;
then ,call epoll_wait(epollfd, myevents, MAX_EVENT_NUMBER, timeout);
dispatch these events,,,Okay!

2.Otherwises ,the libevent seems like these--
U should create a event_base , ready for others events
myBase = event_base_new();
Of cause ,U can create a event ,some like signal time IObuffer or socket etc...
myEvent = event_new();
then, U can add these in myBase
and next ,U can call event_base_dispatch(myBase);

However ,there are some others funcations ,for ready to another usage ,could 
refer to official documents.

[REF]     
http://www.wangafu.net/~nickm/libevent-book/
[source code]
1.epoll/*Include realization of epoll  and  test coding*/
2.libevent/*Also*/





[QUESTION]
select(nfds, readfds, writefds, exceptfds, timeout);
? Why nfds is the highest-numbered file descriptor in any of the three sets,  plus 1

[EXPAND]
边缘触发和条件触发，或者叫非边缘触发
边缘触发就是 event 的状态改变，才会触发，而条件触发是只要满足条件就触发
举个读socket的例子假定经过长时间的沉默后，现在来了100个字节，这时无论
边缘触发和条件触发都会产生一个read ready notification通知应用程序可读。
应用程序读了50个字节，然后重新调用api等待io事件。这时条件触发的api会
因为还有50个字节可读从 而立即返回用户一个read ready notification。而边缘
触发的api会因为可读这个状态没有发生变化而陷入长期等待。 因此在使用
边缘触发的api时，要注意每次都要读到socket返回EWOULDBLOCK为止，否则这个
socket就算废了。而使用条件触发的api 时，如果应用程序不需要写就不要关注
socket可写的事件，否则就会无限次的立即返回一个write ready notification。
大家常用的select就是属于条件触发这一类，长期关注socket写事件会出现CPU100%的毛病
