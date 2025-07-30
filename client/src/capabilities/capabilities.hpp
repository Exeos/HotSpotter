#pragma once
#include "jvmti.h"

namespace hot_spotter::capabilities {
    extern jvmtiCapabilities capa;
    bool setCapabilities();
}
