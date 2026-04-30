#include "Packet.hpp"

#include <cstdlib>
#include "impl/ConnectPacket.hpp"

namespace ipc {
    Packet::Packet(const unsigned char id) {
        Packet::id = id;
    }

    std::optional<Packet*> Packet::decode(const unsigned char *packetBytes, size_t size) {
        if (size == 0) {
            return {};
        }

        auto payload = static_cast<unsigned char *>(malloc(size));

        switch (packetBytes[0]) {
            case 0: {
                ConnectPacket* p = new ConnectPacket{};

                free(payload);
                return {p};
            }
            default:
                free(payload);
                return {};
        }
    }
}
