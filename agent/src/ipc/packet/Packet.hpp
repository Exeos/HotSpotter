#pragma once

namespace ipc {
    class Packet {
    public:
        Packet(const unsigned char id);
        virtual unsigned char* encode() = 0;
        Packet* decode(unsigned char* packetBytes)
    protected:
        unsigned char id;
    };
}