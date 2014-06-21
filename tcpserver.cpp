/*
    Copyright 2014 Matthew Fosdick

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include "tcpserver.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <cctype>
#include <cstdlib>
#include <iostream>
namespace server {
	static int sock;
	static sockaddr_in info;
	int Init(const char *ip,int port) {
		if((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
			return errno;
		}
		memset(&info,0,sizeof(sockaddr_in));
		info.sin_family=AF_INET;
		info.sin_port=htons(port);
		errno=0;
		info.sin_addr.s_addr=htonl(parseIP(ip));
		if(errno) {
			return errno;
		}
		if(bind(sock,reinterpret_cast<const struct sockaddr *>(&info),
			sizeof(sockaddr_in))) {
			return errno;
		}
		if(listen(sock,20)) {
			return errno;
		}
		return 0;
	}
	uint32_t parseIP(const char *ip) {
		uint32_t ret=0;
		uint8_t temp=0;
		int shift=3;
		while(*ip) {
			if(isdigit(*ip)) {
				temp*=10;
				temp+=(*ip-'0');
			}else {
				if(*ip=='.') {
					ret|=(temp<<((shift--)*8));
					temp=0;
					if(shift==-1) {
						return ret;
					}
				}else{
					errno=EINVAL;
					return ret;
				}
			}
			ip++;
		}
		ret|=temp;
		return ret;
	}
	tcpclient *accept() {
		unsigned int temp;
		sockaddr_in c_info;
		int c_sock=accept(sock,reinterpret_cast<sockaddr *>(&c_info),
		&temp);
		if(c_sock==-1) {
			return NULL;
		}
		tcpclient *client=new tcpclient(c_sock,c_info);
		return client;
	}
	void quit_client(tcpclient *client) {
		client->end();
		delete client;
	}
}