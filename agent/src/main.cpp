#include <string>
#include <chrono>
#include <thread>

#include <zmq.hpp>

#include "ipc/Ipc.hpp"
#include "ipc/packet/Packet.hpp"

int main()
{
    Ipc ipc{};

    ipc.listen();

    return 0;
}
