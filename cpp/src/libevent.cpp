/*************************************************************************
    > File Name: libevent.cpp
    > Author: lxf
    > Mail: blueskytoday@126.com 
    > Created Time: Mon 27 Apr 2015 02:55:06 PM CST
 ************************************************************************/

#include<iostream>
//#include <event.h>
#include <cevent>
using namespace std;

void func(int sock, short event, void *arg){
	cout << "Here!" << endl;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	event_add((struct event*)arg, &tv);
}

int main(){
	event_init();
	struct event ev_time;
	evtimer_set(&ev_time, func, &ev_time);
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	event_add(&ev_time, &tv);
	event_dispatch();
	return 0;
}

