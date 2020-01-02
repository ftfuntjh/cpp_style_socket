//
// Created by 曹高阳 on 2020-01-02.
//
#include "socket/Socket.hpp"
#include <iostream>

using namespace std;
using namespace network;

int main(int argc, char *argv[]) {
    auto socket = Socket::create(AF_INET, SOCK_STREAM, 0);
    socket.setOption(option::SocketDebug{1});
    socket.setOption(option::SocketKeepAlive{1});
    auto type = option::SocketType{};
    socket.getOption(type);
    cout << "socket type: " << type.value << endl;
    return 0;
}
