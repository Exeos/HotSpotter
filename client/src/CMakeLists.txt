target_sources(client PRIVATE
        globals.hpp
        hot_spotter.hpp
        hot_spotter.cpp
        attacher/Attacher.hpp
        capabilities/capabilities.hpp
        capabilities/capabilities.cpp
        class_dumper/class_dumper.hpp
        class_dumper/class_dumper.cpp
        gui/Window.hpp
        gui/MainWindow.hpp
        gui/MainWindow.cpp
        gui/components/FileTree.hpp
        hooks/hooks.hpp
        hooks/hooks.cpp
        logger/logger.inl
        logger/logger.hpp
)

if(WIN32)
    target_sources(client PRIVATE
            attacher/impl/WindowsAttacher.cpp
            logger/impl/logger_windows.cpp
            platform/entry_windows.cpp
    )
elseif(UNIX)
    target_sources(client PRIVATE
            attacher/impl/LinuxAttacher.cpp
            logger/impl/logger_linux.cpp
            platform/entry_linux.cpp
    )
else()
endif()
