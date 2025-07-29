#pragma once
#include "jvmti.h"

namespace hot_spotter::hooks {
    extern jvmtiEventCallbacks event_callbacks;
    bool initHooks();
}
