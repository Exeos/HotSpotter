#include "Ipc.hpp"

#include <iostream>

Ipc::Ipc() {
    context = zmq::context_t{1};
    socket = zmq::socket_t{context, zmq::socket_type::rep};

    socket.bind("tcp://localhost:3247");
}

Ipc::~Ipc() {

}

void Ipc::listen() {
    for (;;) {
        zmq::message_t request;
        socket.recv (request, zmq::recv_flags::none);

        if (request.size() == 0) {
            std::printf("Ignoring received data with size of 0");
            continue;
        }

        auto packetBytes = static_cast<unsigned char*>(request.data());

        switch (packetBytes[0]) {
            case 0x0:
                std::printf("Client connected");

                break;
            default:
                std::printf("Invalid Packet ID: %X", packetBytes[0]);
        }
    }
}
