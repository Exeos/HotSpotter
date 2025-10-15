#include "../AbstractInjector.hpp"
#include <iostream>

class LinuxInjector final : public AbstractInjector {
public:
    void inject(int pid, const char *payload) override {
        std::cout << "pid: " << pid << " payload: " << payload;
    }
};

AbstractInjector* create() {
    return new LinuxInjector();
}