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
#include "tcpclient.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
namespace server{
    void tcpclient::begin() {
    	connect(this->sock,reinterpret_cast<const struct sockaddr *>
    		(&(this->info)),sizeof(sockaddr_in));
    }
    ssize_t tcpclient::read(void *buf, size_t nbyte, off_t offset) {
    	return pread(this->sock,buf,nbyte,offset);
    }
    ssize_t tcpclient::read(void *buf, size_t nbyte) {
    	return ::read(this->sock,buf,nbyte);
    }
    ssize_t tcpclient::write(const void *buf, size_t nbyte, off_t offset) {
    	return pwrite(this->sock,buf,nbyte,offset);
    }
    ssize_t tcpclient::write(const void *buf, size_t nbyte) {
    	return ::write(this->sock,buf,nbyte);
    }
    void tcpclient::end() {
    	close(this->sock);
    }
    std::streamsize tcpclient::xsputn (const char* s, std::streamsize n) {
    	return write((void *)s,n);
    }
    tcpclient::tcpclient(int _sock,sockaddr_in _info) {
    	this->sock=_sock;
    	this->info=_info;
    }
}