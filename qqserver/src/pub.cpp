/*
 * pub.cpp
 *
 *  Created on: 2016年8月18日
 *      Author: lenovo
 */

#include "pub.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

void catch_Signal(int Sign)
{
	switch (Sign)
	{
	case SIGINT:
		//printf("SIGINT Signal\n");
		exit(0);
		break;
	case SIGPIPE:	//如果一个非阻塞的socket已经关闭，在这个socket上调用send函数，会触发一个SIGPIPE消息
		//printf("SIGALRM Signal\n");
		//alarm(sec);
		break;
	}
}

int signal1(int signo,void(*func)(int))
{
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	return sigaction(signo,&act,&oact);
}


void setdaemon()
{
	pid_t pid, sid;
	pid = fork(); //fork一个子进程出来
	if (pid < 0)
	{
		printf("fork failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		exit(EXIT_FAILURE); //父进程退出
	}
	if ((sid = setsid()) < 0) //子进程调用setsid()函数，江金城和控制台脱离关系
	{
		printf("setsid failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int socket_create(int port)
{
	int st = socket(AF_INET, SOCK_STREAM, 0); //创建TCP socket
	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return 0;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind port %d failed %s\n", port, strerror(errno));
		return 0;
	}
	if (listen(st, 300) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return 0;
	}
	return st;
}



