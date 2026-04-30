#include "Ipc.hpp"

#include <iostream>

#include "packet/impl/ConnectPacket.hpp"

Ipc::Ipc() {
    context = zmq::context_t{1};
    socket = zmq::socket_t{context, zmq::socket_type::rep};

    socket.bind("tcp://localhost:3247");
}

void Ipc::listen() {
    for (;;) {
        zmq::message_t request;
        socket.recv (request, zmq::recv_flags::none);

        if (request.size() == 0) {
            std::printf("Ignoring received data with size of 0");
            continue;
        }

        auto received = ipc::Packet::decode(static_cast<unsigned char*>(request.data()), request.size());

        if (received.has_value()) {
            auto response = handleRequest(received.value());
            if (response.value()) {
                auto encoded = response.value()->encode();
                socket.send(static_cast<void*>(encoded.first), encoded.second, 0);
            } else {
                std::printf("Failed to create response");
            }
            delete received.value();
        } else {
            std::printf("Failed to decode received data");
        }
    }
}

std::optional<ipc::Packet*> Ipc::handleRequest(ipc::Packet *request) {
    switch (request->id) {
        case 0: {
            ipc::ConnectPacket* response = new ipc::ConnectPacket{};
            return {response};
        }
        default:
            return {};
    }
}
