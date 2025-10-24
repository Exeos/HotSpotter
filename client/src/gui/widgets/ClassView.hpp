#pragma once

#include <map>
#include <string>

#include "jni.h"
#include "jni_md.h"
#include "jvmti.h"
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
    struct ClassFieldResult {
        jvmtiError error = JVMTI_ERROR_NONE;
        std::string errorName;
        jint fieldCount = -1;
        jfieldID *fields = nullptr;
    };

    struct ClassMethodResult {
        jvmtiError error = JVMTI_ERROR_NONE;
        std::string errorName;
        jint methodCount = -1;
        jmethodID *methods = nullptr;
    };

    struct FieldInfo {
        std::pair<jvmtiError, std::string> getName_Error;
        std::pair<jvmtiError, std::string> getModifiers_Error;

        char *name;
        char *signature;
        char *genericSignature;

        jint modifiers;
        std::string modifiersListed;
    };

    struct MethodInfo {
        jvmtiError error = JVMTI_ERROR_NONE;
        std::string errorName;
        char *name;
        char *signature;
        char *genericSignature;
    };

    class ClassView final : public Widget {
    public:
        ClassView(const std::string &className, jclass klass);

        ~ClassView() override;

        void Render() override;

        static void RenderFieldCard(const FieldInfo &fieldInfo);

        static void RenderMethodCard(const MethodInfo &methodInfo);

    private:
        std::string className;
        ClassFieldResult classFieldsResult;
        ClassMethodResult classMethodsResult;

        std::map<jfieldID, FieldInfo> fields;
        std::map<jmethodID, MethodInfo> methods;
    };
} // namespace hot_spotter::gui::widget
