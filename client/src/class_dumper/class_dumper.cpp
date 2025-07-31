#include "class_dumper.hpp"

#include "jvmti.h"
#include "../globals.hpp"
#include "../logger/logger.hpp"

namespace hot_spotter::class_dumper {

    void cleanup(jclass* classArr, jint classCount) {
        for (int i = 0; i < classCount; i++) {
            jniEnv->DeleteGlobalRef(classArr[i]);
        }

        jvmTi->Deallocate(reinterpret_cast<unsigned char *>(classArr));
    }

    bool dump() {
        jclass* classArr;
        jint classCount;
        if (jvmtiError error = jvmTi->GetLoadedClasses(&classCount, &classArr); error != JVMTI_ERROR_NONE) {
            Logger::Log("Failed to get loaded classes");
            return false;
        }

        // convert local references to global references
        for (int i = 0; i < classCount; i++) {
            classArr[i] = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(classArr[i]));
        }

        std::vector<jclass> modifiable;
        for (int i = 0; i < classCount; i++) {
            jboolean isModifiable = false;
            if (jvmTi->IsModifiableClass(classArr[i], &isModifiable) == JVMTI_ERROR_NONE && isModifiable) {
                modifiable.push_back(classArr[i]);
            }
        }

        if (!modifiable.empty()) {
            if (jvmTi->RetransformClasses(modifiable.size(), modifiable.data()) != JVMTI_ERROR_NONE) {
                Logger::Log("RetransformClasses failed");
                cleanup(classArr, classCount);
                return false;
            }
        }

        cleanup(classArr, classCount);
        return true;
    }
}
