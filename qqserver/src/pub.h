/*
 * pub.h
 *
 *  Created on: 2016年8月18日
 *      Author: lenovo
 */

#ifndef PUB_H_
#define PUB_H_


void setdaemon();
int socket_create(int port);
int signal1(int signo,void(*func)(int));
void catch_Signal(int Sign);


#endif /* PUB_H_ */
