#include <thread>

#include "../hot_spotter.hpp"

__attribute__((constructor))
void so_entry() {
    std::thread(hot_spotter::init).detach();
}