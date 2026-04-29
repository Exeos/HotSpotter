#include <thread>

#include "../agent.hpp"

__attribute__((constructor))
void so_entry() {
    std::thread(agent::init).detach();
}
