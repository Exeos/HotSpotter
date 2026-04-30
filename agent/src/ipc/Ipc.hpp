#pragma once
#include <zmq.hpp>

#include "packet/Packet.hpp"

class Ipc {
public:
    Ipc();
    ~Ipc() = default;

    void listen();
private:
    std::optional<ipc::Packet*> handleRequest(ipc::Packet* request);
    zmq::context_t context;
    zmq::socket_t socket;
};
