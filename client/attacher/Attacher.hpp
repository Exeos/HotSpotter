#pragma once

class Attacher {
public:
    virtual bool attach(JavaVM*& vm, JNIEnv*& env, jvmtiEnv*& jvmtiEnv) = 0;
    virtual ~Attacher() = default;
};

Attacher* createAttacher();