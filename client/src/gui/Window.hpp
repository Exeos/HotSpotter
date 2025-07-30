#pragma once
#include <string>

#include "../logger/logger.inl"

namespace hot_spotter::gui {

    class Window {
    public:
        virtual ~Window() = default;
        virtual bool init()                          = 0;
        virtual void render()                        = 0;
        virtual void close()                        = 0;
        virtual const std::string& getName() const   = 0;
    };

    static void glfw_error_callback(int error, const char* description)
    {
        Logger::LogFormat("GLFW Error %d: %s\n", error, description);
    }

}
