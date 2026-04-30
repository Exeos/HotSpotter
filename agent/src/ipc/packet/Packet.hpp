#pragma once

#include <optional>
#include <utility>

namespace ipc {
    class Packet {
    public:
        Packet(const unsigned char id);
        virtual ~Packet() = default;

        // returns data and size
        virtual std::pair<unsigned char*, size_t> encode() = 0;
        static std::optional<Packet*> decode(const unsigned char *packetBytes, size_t size);
        unsigned char id;
    };
}
