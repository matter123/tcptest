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
#pragma once
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <iostream>
namespace server {
    class tcpclient:public std::streambuf {
    public:
        tcpclient(const char *addr,int port);
        tcpclient(int sock,sockaddr_in info);
        void begin();
        ssize_t read(void *buf, size_t nbyte, off_t offset);
        ssize_t read(void *buf, size_t nbyte);
        ssize_t write(const void *buf, size_t nbyte, off_t offset);
        ssize_t write(const void *buf, size_t nbyte);
        void end();
    protected:
        std::streamsize xsputn (const char* s, std::streamsize n);
    private:
        int sock;
        sockaddr_in info;
    };
}