#include "capabilities.hpp"

#include "../globals.hpp"

namespace hot_spotter::capabilities {

    bool setCapabilities() {
        jvmtiCapabilities capa;
        jvmtiError err = jvmTi->GetPotentialCapabilities(&capa);
        if (err == JVMTI_ERROR_NONE) {
            err = jvmTi->AddCapabilities(&capa);
        }

        return err == JVMTI_ERROR_NONE;
    }
}
