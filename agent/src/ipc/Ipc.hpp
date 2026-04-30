#pragma once
#include <zmq.hpp>

class Ipc {
public:
    Ipc();
    ~Ipc();

    void listen();
private:
    zmq::context_t context;
    zmq::socket_t socket;
};
