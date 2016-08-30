/*
 * work.cpp
 *
 *  Created on: 2016年8月18日
 *      Author: lenovo
 */

#include "work.h"
#include "pub.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>

work::work(int port)
{
	memset(socket_client, 0, sizeof(socket_client));
	listen_st = socket_create(port); //建立server端socket
	if (listen_st == 0)
	{
		exit(-1); //socket 建立失败，退出;
	}

}

work::~work()
{
	if (listen_st)
		close (listen_st);
}

void work::broadcast_user_status() //向socket_client[]数组中所有client广播用户状态消息
{
	struct msg_t msg;
	memset(&msg, 0, sizeof(msg));
	msg.head[0] = 1; //设置消息类型为用户状态消息

	for (int i = 0; i < CLIENTCOUNT; i++) //设置用户状态消息体
	{
		if (socket_client[i] != 0)
		{
			msg.body[i] = '1'; //设置相应userid状态为在线
		} else
		{
			msg.body[i] = '0'; //设置相应userid状态为离线
		}
	}

	for (int i = 0; i < CLIENTCOUNT; i++) //向socket_client数组中每个client广播用户状态消息
	{
		if (socket_client[i] != 0)
		{
			send(socket_client[i], &msg, strlen(msg.body) + sizeof(msg.head),
					0);
			printf("%d:broadcast, %s\n", i, msg.body);
		}
	}
}

//将accept的客户端连接安装到socket_client[]数组中，index为client登录时的UserID,st为client socket
void work::fix_socket_client(int index, int st)
{
	//比如userid 5这个用户登陆了，index = 5；
	//socket_client[5]是不是为0，如果为0，代表userid5这个用户还没有登陆，如果不等于0，代表userid5这个用户已经登录了
	if (socket_client[index] != 0) //同一个userid没有下线，却又在另一个终端登录，拒绝登录
	{
		printf("%d:userid already login\n", index);
		struct msg_t msg;
		memset(&msg, 0, sizeof(msg));
		msg.head[0] = 2; //系统消息
		msg.head[1] = 3; //userid已经登录
		msg.head[2] = 0; //暂时保留，填0
		msg.head[3] = 0; //暂时保留，填0
		send(st, (char *) &msg, sizeof(msg.head), 0); //给client端socket下发系统消息
		close(st);
	} else
	{
		//如果socket_client[index] 等于0，将client端socket赋予socket_client[index]

		//socket_client[5] = st;
		socket_client[index] = st;
	}
}

void work::sendmsg(const msg_t *msg, ssize_t msglen)
{
	if ((msg->head[2] < 0) || (msg->head[2] >= CLIENTCOUNT)) //没有这个目标用户
	{
		printf("%d:have not this userid\n", msg->head[2]);
	} else
	{
		if (socket_client[msg->head[2]] == 0) //目的user不在线
		{
			printf("%d:userid not online\n", msg->head[2]);
		} else
		{
			//给client端socket下发send消息
			send(socket_client[msg->head[2]], (const char *) msg, msglen, 0);
			printf("send message:%dto%d-%s\n", msg->head[1], msg->head[2],
					msg->body);
		}
	}
}

int work::auth_passwd(int userid, const char *passwd) //验证用户密码
{
	if (strncmp(passwd, "123456", 6) == 0) //如果密码等于“123456” 验证通过
		return 1;
	else
		return 0;
}

void work::loginmsg(int st, int o_userid, const char *passwd) //处理login消息
{
	struct msg_t msg;
	memset(&msg, 0, sizeof(msg));
	msg.head[0] = 2; //系统消息
	msg.head[2] = 0; //暂时保留
	msg.head[3] = 0; //暂时保留

	if ((o_userid < 0) || (o_userid >= CLIENTCOUNT)) //无效的useid
	{
		printf("login failed, %d:invalid userid\n", o_userid);
		msg.head[1] = 1; //无效userid
		send(st, (const char*) &msg, sizeof(msg.head), 0);
		close(st);
		return;
	}
	if (!auth_passwd(o_userid, passwd)) //验证用户登陆Userid和密码
	{
		printf("login failed, userid=%d,passwd=%s:invalid passwd\n", o_userid,
				passwd);
		msg.head[1] = 2; //无效密码
		//给client端socket下发系统消息
		send(st, (const char*) &msg, sizeof(msg.head), 0);
		close(st);		//验证失败，关闭client_socket，函数返回
		return;
	}
	printf("%d:login success\n", o_userid);
	fix_socket_client(o_userid, st); //将登录密码验证通过的客户端client安装到socket_client[]的数组中
	broadcast_user_status(); //向socket_client数组中所有socket广播用户状态消息

}

int work::setnonblocking(int st) //设置socket为非阻塞
{
	int opts = fcntl(st, F_GETFL);
	if (opts < 0)
	{
		printf("fcntl failed %s\n", strerror(errno));
		return 0;
	}
	opts = opts | O_NONBLOCK;
	if (fcntl(st, F_SETFL, opts) < 0)
	{
		printf("fcntl failed %s\n", strerror(errno));
		return 0;
	}
	return 1;

}

int work::socket_accept()
{
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	int client_st = accept(listen_st, (struct sockaddr *) &client_addr, &len);

	if (client_st < 0)
	{
		printf("accept failed %s\n", strerror(errno));
	} else
	{
		printf("accept by %s\n", inet_ntoa(client_addr.sin_addr));
	}
	return client_st; //返回来自client端的socket描述符
}

int work::socket_recv(int st)
{
	struct msg_t msg;
	memset(&msg, 0, sizeof(msg));
	ssize_t rc = recv(st, (char*) &msg, sizeof(msg), 0); //接受来自client socket发送来的消息
	/*
	 printf("msg.head[0] = %d\n",msg.head[0]);
	 printf("msg.head[1] = %d\n",msg.head[1]);
	 printf("msg.head[2] = %d\n",msg.head[2]);
	 printf("msg.head[3] = %d\n",msg.head[3]);
	 printf("msg.body = %s\n",msg.body);
	 */
	if (rc <= 0)
	{
		if (rc < 0)
			printf("recv failed %s\n", strerror(errno));
	} else //接收成功
	{
		switch (msg.head[0])
		{
		case 0: //send消息
			sendmsg(&msg, rc);
			break;
		case 1:			//login消息
			loginmsg(st, msg.head[1], msg.body);
			break;
		default:			//无法识别的消息
			printf("login fail, it's not login message,%s\n",
					(const char*) &msg);
			msg.head[0] = 2;			//系统消息
			msg.head[1] = 0;			//无法识别的消息
			msg.head[2] = 0;			//暂时保留，填0
			msg.head[3] = 0;			//暂时保留，填0
			send(st, (const char*) &msg, sizeof(msg.head), 0);//改client端socket下发系统消息
			return 0;
		}
	}

	return rc;
}

int work::user_logout(int st)			//client_socket连接断开
{
	for (int i = 0; i < CLIENTCOUNT; i++)
	{
		if (socket_client[i] == st)		//找到socket_client[]数组中与相等的client_socket
		{
			printf("userid=%d,socket disconnect\n", i);
			close(socket_client[i]);			//关闭socket_client[]数组中相应的socket
			socket_client[i] = 0;//将数组socket_client[]中相应的元素初始化为0，以便该userid下次还可以连接、
			//broadcast_user_status();//向socket_client数组中所有socket广播用户状态信息
			return 0;
		}
	}
}

void work::run()
{
//epoll模型基本都是这样的，我们需要处理的时候，是实现的具体业务逻辑
//声明epoll_event结构结构体的变量，ev用于注册事件，数组用于回传要处理的事件
	struct epoll_event ev, events[CLIENTCOUNT];
	setnonblocking(listen_st); //设置服务端listen的socket为非阻塞
	int epfd = epoll_create(CLIENTCOUNT);
	ev.data.fd = listen_st; //将listen_st放入poll
	ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_st, &ev); //注册epoll事件

	int st = 0;
	while (1)
	{
		//epoll_wait阻塞，直到epoll当中的socket有指定的消息产生
		int nfds = epoll_wait(epfd, events, CLIENTCOUNT, -1);
		if (nfds == -1)
		{
			printf("epoll_wait failed %s\n", strerror(errno));
			break;
		}
		for (int i = 0; i < nfds; i++) //循环处理epoll_wati返回所有出事的socket
		{
			if (events[i].data.fd < 0)
				continue;

			if (events[i].data.fd == listen_st) //代表收到了来自客户端的connect
			{
				st = socket_accept();

				if (st >= 0)
				{
					setnonblocking(st);
					ev.data.fd = st;
					ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
					epoll_ctl(epfd, EPOLL_CTL_ADD, st, &ev);
					continue;
				}
			}

			if (events[i].events & EPOLLIN) //有来自client的数据
			{
				st = events[i].data.fd; //得到有数据的socket;
				if (socket_recv(st) <= 0)
				{
					user_logout(st);
					events[i].data.fd = -1;
				}
			}

			if (events[i].events & EPOLLERR) //有来自client的出错
			{
				st = events[i].data.fd; //得到有数据的socket;
				user_logout(st);
				events[i].data.fd = -1;
			}

			if (events[i].events & EPOLLHUP) //有来自client的关闭
			{
				st = events[i].data.fd; //得到有数据的socket;
				user_logout(st);
				events[i].data.fd = -1;
			}
		}
	}
	close(epfd);
}

