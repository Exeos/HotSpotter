#pragma once
#include "../Packet.hpp"

namespace ipc {
    class ConnectPacket : public ipc::Packet {
    public:
        explicit ConnectPacket() : Packet(0) {}
        std::pair<unsigned char*, size_t> encode() override;
    };
}