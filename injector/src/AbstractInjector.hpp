#pragma once

class AbstractInjector {
public:
    virtual void inject(int pid, const char *payload) = 0;
    virtual ~AbstractInjector() = default;
};

AbstractInjector* create();