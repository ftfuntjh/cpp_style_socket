//
// Created by 曹高阳 on 2020-01-02.
//
#include "socket/Socket.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    auto socket = network::Socket::create(AF_INET, SOCK_STREAM, 0);
    socket.setOption(network::option::SocketDebug{1});
    auto type = network::option::SocketType{};
    socket.getOption(type);
    cout << type.value << endl;
    return 0;
}
