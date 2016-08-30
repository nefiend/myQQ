//============================================================================
// Name        : qqserver.cpp
// Author      : XiaoFan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "pub.h"
#include "work.h"

int main(int arg, char *args[])
{
	if(arg<2)//如果没有参数，main函数返回
	{
		printf("useage:qqsever port\n");
		return -1;
	}

	int iport = atoi(args[1]);//将第二个参数转化为整数
	if(iport == 0)
	{
		printf("port %d is invalid\n",iport);
		return -1;
	}

	setdaemon();//将程序设置为daemon状态

	work w(iport);

	printf("qqserver begin\n");

	signal1(SIGINT,catch_Signal);
	w.run();

	while(1)
	{
		sleep(1);
	}
	printf("qqserver end\n");
	return 0;
}
