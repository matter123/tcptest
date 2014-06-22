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
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <netdb.h>
namespace server {
	static int sock;
	addrinfo *info;
	int Init(const char *addr,int port) {
		addrinfo hints, *res;
		memset(&hints,0,sizeof(addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = 0;
		hints.ai_canonname = NULL;
		hints.ai_addrlen = 0;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;
		char buf[6];
		sprintf(buf,"%d",port);
		printf("%s:%s\n",addr,buf);
		int retv=getaddrinfo(addr,buf,&hints,&res);
		if(retv) {
			std::cout<<gai_strerror(retv)<<std::endl;
			freeaddrinfo(res);
			errno=EINVAL;
			return errno;
		}
		info=res;
		while(info) {
			if((sock=socket(info->ai_family,info->ai_socktype,info->ai_protocol))==-1) {
				printf("%s\n",strerror(errno));
				info=info->ai_next;
				continue;
			}
			if(bind(sock,info->ai_addr,info->ai_addrlen)) {
				printf("%s\n",strerror(errno));
				close(sock);
				info=info->ai_next;
				continue;
			}
			break;
		}
		if(!info) {
			errno=EINVAL;
			return errno;
		}
		freeaddrinfo(res);
		if(listen(sock,20)) {
			return errno;
		}
		std::cout<<std::flush;
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
			std::cout<<strerror(errno)<<std::endl;
			return NULL;
		}
		tcpclient *client=new tcpclient(c_sock,c_info);
		std::cout<<"hello client"<<std::endl;
		return client;
	}
	void quit_client(tcpclient *client) {
		client->end();
		delete client;
		std::cout<<"bye bye client"<<std::endl;
	}
}