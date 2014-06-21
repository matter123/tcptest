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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <iostream>
#include "tcpserver.h"
#include <errno.h>
#include <string.h>
int main(int argc, char **argv) {
    std::cout<<std::hex<<server::parseIP("127.0.0.1")<<std::endl;
    errno=0;
    if(server::Init("127.0.0.1",8080)) {
        std::cout<<"OOPS: "<<strerror(errno)<<std::endl;
        exit(1);
    }
    while(1) {
        server::tcpclient *client=server::accept();
        std::cout<<"I GOT A CONNECTION"<<std::endl;
        client->begin();
        std::iostream test(client);
        test<<"TCP IS AWSOME(sorta maybe)\n";
        client->end();
        server::quit_client(client);
    }
}