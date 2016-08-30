/*
 * work.h
 *
 *  Created on: 2016年8月18日
 *      Author: lenovo
 */

#ifndef WORK_H_
#define WORK_H_

#include <sys/types.h>
#include <sys/socket.h>

#define CLIENTCOUNT 256
#define MSGBODYBUF 1024
struct msg_t
{
	unsigned char head[4];
	char body[MSGBODYBUF];
};

class work
{
public:
	work(int port);
	~work();


	void run();

private:
	void broadcast_user_status(); //向socket_client[]数组中所有client广播用户状态消息
	void fix_socket_client(int index, int st); //将accept的客户端连接安装到socket_client[10]的数组中
	void sendmsg(const struct msg_t *msg, ssize_t msglen);
	int auth_passwd(int userid,const char *passwd);
	void loginmsg(int st,int o_userid,const char *passwd);//处理login消息
	int setnonblocking(int st);
	int socket_accept();
	int socket_recv(int st);
	int user_logout(int st);

	int listen_st;
	int socket_client[CLIENTCOUNT];


};



#endif /* WORK_H_ */
