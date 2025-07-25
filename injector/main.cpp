#include "injector/AbstractInjector.hpp"

int main() {
    AbstractInjector* injector = create();
    injector->inject(0, nullptr);
    return 0;
}
