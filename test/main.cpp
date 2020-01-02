//
// Created by 曹高阳 on 2020-01-02.
//
#include "socket/Socket.hpp"
#include <iostream>

using namespace std;
using namespace network;

int main(int argc, char *argv[]) {
    auto tcp = Socket::tcp();
    auto udp = Socket::udp();
    auto type = option::SocketType{};
    tcp.getOption(type);
    cout << "tcp socket type: " << type.value << endl;
    udp.getOption(type);
    cout << "udp socket type: " << type.value << endl;
    return 0;
}
