/*************************************************************************
  > File Name: test.cpp
  > Author: lxf
  > Mail: blueskytoday@126.com 
  > Created Time: Mon 27 Apr 2015 11:25:53 AM CST
 ************************************************************************/

#include<iostream>
#include <cstring>
#include <sys/epoll.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

const int MAX_EVENTS = 500;

void recv_data(int fd, int events, void *arg);
void send_data(int fd, int events, void *arg);

struct myevent_s{
	int fd;
	void (*call_back)(int fd, int events, void *args);
	int events;
	void *arg;
	int status;
	char buff[128];
	int len, s_offset;
	long last_active;
};

void event_set(myevent_s *ev, int fd, void (*call_back)(int, int, void*), void *arg){
	ev->fd = fd;
	ev->call_back = call_back;
	ev->events = 0;
	ev->arg = arg;
	ev->status = 0;
	bzero(ev->buff, sizeof(ev->buff));
	ev->len = 0;
	ev->last_active = time(NULL);
}

void event_add(int epollfd, int events, myevent_s *ev){
	struct epoll_event epv = { 0, { 0 } };
	int op;
	epv.data.ptr = ev;
	epv.events = ev->events = events;
	if (ev->status == 1){
		op = EPOLL_CTL_MOD;
	} else {
		op = EPOLL_CTL_ADD;
		ev->status = 1;
	}
	if (epoll_ctl(epollfd, op, ev->fd, &epv) < 0){
		printf("Event Add failed[fd=%d], events[%d]\n", ev->fd, events);
	} else {
		printf("Event Add OK[fd=%d], op=%d, events[%d]\n", ev->fd, op, events);
	}
}

void event_del(int epollfd, myevent_s *ev){
	struct epoll_event epv = { 0, { 0 } };
	if (ev->status != 1){
		return;
	}
	epv.data.ptr = ev;
	ev->status = 0;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, ev->fd, &epv);
}

int g_epollfd;
myevent_s g_events[MAX_EVENTS+1];

void accept_conn(int fd, int events, void *arg){
	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr_in);
	int nfd, i;
	if ((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1){
		if (errno != EAGAIN && errno != EINTR){
		}
		printf("%s: accept, %d", __func__, errno);
		return;
	}
	do{
		for (i=0; i<MAX_EVENTS; i++){
			if (g_events[i].status == 0){
				break;
			}
		}
		if (i == MAX_EVENTS){
			printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);
			break;
		}
		int iret = 0;
		if ((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0){
			printf("%s:fcntl nonblocking failed:%d", __func__, iret);
			break;
		}
		event_set(&g_events[i], nfd, recv_data, &g_events[i]);
		event_add(g_epollfd, EPOLLIN, &g_events[i]);
	}while(0);
	printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),
			ntohs(sin.sin_port), g_events[i].last_active, i);
}

void recv_data(int fd, int events, void *arg){
	struct myevent_s *ev = (struct myevent_s *)arg;
	int len;
	len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);
	event_del(g_epollfd, ev);
	if (len > 0){
		ev->len += len;
		ev->buff[len] = '\0';
		printf("recv[%d]:%s\n", fd, ev->buff);
		event_set(ev, fd, send_data, ev);
		event_add(g_epollfd, EPOLLOUT, ev);
	} else if (len == 0){
		close(ev->fd);
		printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_events);
	} else {
		close(ev->fd);
		printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
	}
}

void send_data(int fd, int events, void *arg){
	struct myevent_s *ev = (struct myevent_s*)arg;
	int len;
	len = send(fd, ev->buff+ev->s_offset, ev->len-ev->s_offset, 0);
	if (len > 0){
		printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
		ev->s_offset += len;
		if (ev->s_offset == ev->len){
			event_del(g_epollfd, ev);
			event_set(ev, fd, recv_data, ev);
			event_add(g_epollfd, EPOLLIN, ev);
		}
	} else {
		close(ev->fd);
		event_del(g_epollfd, ev);
		printf("send[fd=%d] error[%d]\n", fd, errno);
	}
}

void init_listen_socket(int epollfd, short port){
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(listenfd, F_SETFL, O_NONBLOCK);
	printf("server listen fd=%d\n", listenfd);
	event_set(&g_events[MAX_EVENTS], listenfd, accept_conn, &g_events[MAX_EVENTS]);
	event_add(epollfd, EPOLLIN, &g_events[MAX_EVENTS]);
	sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	bind(listenfd, (const sockaddr*)&sin, sizeof(sin));
	listen(listenfd, 5);
}

int main(int argc, char *argv[]){
	unsigned short port = 12345;
	if (argc == 2){
		port = atoi(argv[1]);
	}
	//create epoll
	g_epollfd = epoll_create(MAX_EVENTS);
	if (g_epollfd <= 0){
		printf("create epoll failed:%d.\n", g_epollfd);
	}
	init_listen_socket(g_epollfd, port);
	//event loop
	struct epoll_event events[MAX_EVENTS];
	printf("server running:port[%d]\n", port);
	int check_pos = 0;
	while(1){
		long now =time(NULL);
		for (int i=0; i<100; i++, check_pos++){
			if (check_pos == MAX_EVENTS) check_pos = 0;
			if (g_events[check_pos].status != 1) continue;
			long duration = now - g_events[check_pos].last_active;
			if (duration >= 60){
				close(g_events[check_pos].fd);
				printf("[fd=%d] timeout[%d--%d].\n", g_events[check_pos].fd, g_events[check_pos].last_active, now);
				event_del(g_epollfd, &g_events[check_pos]);
			}
		}
		int fds = epoll_wait(g_epollfd, events, MAX_EVENTS, 1000);
		if (fds < 0){
			printf("epoll_wait error, exit.\n");
			break;
		}
		for (int i=0; i<fds; i++){
			myevent_s *ev = (struct myevent_s*)events[i].data.ptr;
			if ((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)){
				ev->call_back(ev->fd, events[i].events, ev->arg);
			}
			if ((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)){
				ev->call_back(ev->fd, events[i].events, ev->arg);
			}
		}
	}
	return 0;
}

