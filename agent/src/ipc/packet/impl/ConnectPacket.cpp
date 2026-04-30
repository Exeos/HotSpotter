#include "ConnectPacket.hpp"

namespace ipc {
    std::pair<unsigned char*, size_t> ConnectPacket::encode() {
        return std::pair<unsigned char*, size_t>{new unsigned char[1]{}, 1};
    }
}
