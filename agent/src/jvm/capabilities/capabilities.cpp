#include "capabilities.hpp"

#include "jvmti.h"
#include "../../agent.hpp"

namespace jvm::capabilities {

    bool setCapabilities() {
        jvmtiCapabilities capa;
        jvmtiError err = agent::jvmTi->GetPotentialCapabilities(&capa);
        if (err == JVMTI_ERROR_NONE) {
            err = agent::jvmTi->AddCapabilities(&capa);
        }

        return err == JVMTI_ERROR_NONE;
    }
}
