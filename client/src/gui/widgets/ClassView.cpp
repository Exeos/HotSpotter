#include "ClassView.hpp"

#include <ranges>

#include "imgui.h"
#include "../gui.hpp"
#include "../../globals.hpp"
#include "../../utils/java_utils.hpp"

namespace hot_spotter::gui::widget {
    ClassView::ClassView(const std::string &className, jclass klass) {
        ClassView::className = className;

        classFieldsResult.error = jvmTi->
                GetClassFields(klass, &classFieldsResult.fieldCount, &classFieldsResult.fields);
        classFieldsResult.errorName = utils::java::getJvmtiErrorName(classFieldsResult.error);

        classMethodsResult.error = jvmTi->GetClassMethods(klass, &classMethodsResult.methodCount,
                                                          &classMethodsResult.methods);
        classMethodsResult.errorName = utils::java::getJvmtiErrorName(classMethodsResult.error);

        if (classFieldsResult.error == JVMTI_ERROR_NONE) {
            for (int i = 0; i < classFieldsResult.fieldCount; i++) {
                FieldInfo fieldInfo;
                jfieldID field = classFieldsResult.fields[i];

                fieldInfo.getName_Error.first = jvmTi->GetFieldName(klass, field, &fieldInfo.name,
                                                                    &fieldInfo.signature, &fieldInfo.genericSignature);
                fieldInfo.getName_Error.second = utils::java::getJvmtiErrorName(fieldInfo.getName_Error.first);

                fieldInfo.getModifiers_Error.first = jvmTi->GetFieldModifiers(klass, field, &fieldInfo.modifiers);
                fieldInfo.getModifiers_Error.second =
                        utils::java::getJvmtiErrorName(fieldInfo.getModifiers_Error.first);

                if (fieldInfo.getModifiers_Error.first == JVMTI_ERROR_NONE) {
                    std::vector<std::string> modifiersVector = utils::java::parseJavaModifiers(fieldInfo.modifiers);
                    std::string modifiersList;

                    for (size_t i = 0; i < modifiersVector.size(); ++i) {
                        modifiersList += modifiersVector[i];
                        if (i != modifiersVector.size() - 1) {
                            modifiersList += " ";
                        }
                    }

                    fieldInfo.modifiersListed = modifiersList;
                }

                fields[field] = fieldInfo;
            }
        }

        if (classMethodsResult.error == JVMTI_ERROR_NONE) {
            for (int i = 0; i < classMethodsResult.methodCount; i++) {
                MethodInfo methodInfo;
                methodInfo.error = jvmTi->GetMethodName(classMethodsResult.methods[i], &methodInfo.name,
                                                        &methodInfo.signature,
                                                        &methodInfo.genericSignature);
                methodInfo.errorName = utils::java::getJvmtiErrorName(methodInfo.error);

                methods[classMethodsResult.methods[i]] = methodInfo;
            }
        }
    }

    void ClassView::Render() {
        ImGui::BeginChild("ClassView");
        ImGui::Text("%s", className.c_str());

        if (classFieldsResult.error != JVMTI_ERROR_NONE) {
            ImGui::Text("Failed to get class fields. Error: %s", classFieldsResult.errorName.c_str());
        } else {
            char header[128];
            std::snprintf(header, sizeof(header), "Fields (%d):", static_cast<int>(fields.size()));
            if (ImGui::CollapsingHeader(header)) {
                ImGui::Indent(20);
                for (FieldInfo &val: fields | std::views::values) {
                    RenderFieldCard(val);
                }
                ImGui::Unindent(20);
            }
        }

        if (classMethodsResult.error != JVMTI_ERROR_NONE) {
            ImGui::Text("Failed to get class Methods. Error: %s", classMethodsResult.errorName.c_str());
        } else {
            char header[128];
            std::snprintf(header, sizeof(header), "Methods (%d):", static_cast<int>(methods.size()));

            if (ImGui::CollapsingHeader(header)) {
                ImGui::Indent(20);
                for (MethodInfo &val: methods | std::views::values) {
                    RenderMethodCard(val);
                }
                ImGui::Unindent(20);
            }
        }

        ImGui::EndChild();
    }

    void ClassView::RenderFieldCard(const FieldInfo &fieldInfo) {
        char titleBuffer[512];
        if (fieldInfo.signature) {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s %s : %s", fieldInfo.modifiersListed.c_str(), fieldInfo.name,
                     fieldInfo.signature);
        } else {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s %s", fieldInfo.modifiersListed.c_str(), fieldInfo.name);
        }

        if (ImGui::CollapsingHeader(titleBuffer)) {
            ImGui::Indent(20);
            if (fieldInfo.getName_Error.first != JVMTI_ERROR_NONE) {
                ImGui::Text("Error Name: %s", fieldInfo.getName_Error.second.c_str());
            } else {
                ImGui::Text("Signature: %s", fieldInfo.signature);
                ImGui::Text("Generic Signature: %s", fieldInfo.genericSignature);
            }
            ImGui::Unindent(20);
        }
    }


    void ClassView::RenderMethodCard(const MethodInfo &methodInfo) {
        char titleBuffer[512];
        if (methodInfo.signature) {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s : %s", methodInfo.name, methodInfo.signature);
        } else {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s", methodInfo.name);
        }

        if (ImGui::CollapsingHeader(titleBuffer)) {
            ImGui::Indent(20);
            if (methodInfo.error != JVMTI_ERROR_NONE) {
                ImGui::Text("Error Name: %s", methodInfo.errorName.c_str());
            } else {
                ImGui::Text("Signature: %s", methodInfo.signature);
                ImGui::Text("Generic Signature: %s", methodInfo.genericSignature);
            }
            ImGui::Unindent(20);
        }
    }

    // memory clean up.
    ClassView::~ClassView() {
        if (classFieldsResult.fields) {
            // deallocate field's strings
            for (int i = 0; i < classFieldsResult.fieldCount; i++) {
                auto fieldInfo = fields.find(classFieldsResult.fields[i]);;
                if (fieldInfo == fields.end()) {
                    continue;
                }

                if (fieldInfo->second.name) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(fieldInfo->second.name));
                }

                if (fieldInfo->second.signature) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(fieldInfo->second.signature));
                }

                if (fieldInfo->second.genericSignature) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(fieldInfo->second.genericSignature));
                }
            }

            // deallocate jfieldID arr
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(classFieldsResult.fields));
        }
        if (classMethodsResult.methods) {
            // deallocate method's strings
            for (int i = 0; i < classMethodsResult.methodCount; i++) {
                auto methodInfo = methods.find(classMethodsResult.methods[i]);;
                if (methodInfo == methods.end()) {
                    continue;
                }

                if (methodInfo->second.name) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodInfo->second.name));
                }

                if (methodInfo->second.signature) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodInfo->second.signature));
                }

                if (methodInfo->second.genericSignature) {
                    jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodInfo->second.genericSignature));
                }
            }

            // deallocate jmethodID arr
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(classMethodsResult.methods));
        }
    }
}
