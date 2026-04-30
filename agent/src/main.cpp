#include <string>
#include <chrono>
#include <thread>

#include <zmq.hpp>

#include "ipc/Ipc.hpp"
#include "ipc/packet/Packet.hpp"

// this is only for testing. agent will enter via src/entry/entry_[platform].cpp
int main()
{
    Ipc ipc{};

    ipc.listen();

    return 0;
}
