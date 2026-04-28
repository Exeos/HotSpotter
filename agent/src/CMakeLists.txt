target_sources(agent PRIVATE
        agent.cpp
        agent.hpp
        logger/logger.hpp
        logger/logger.inl
        jvm/attacher/attacher.hpp
        jvm/capabilities/capabilities.cpp
        jvm/capabilities/capabilities.hpp
)

if (WIN32)
    target_sources(agent PRIVATE
            entry/entry_win.cpp
            logger/impl/logger_win.cpp
            jvm/attacher/impl/attacher_win.cpp
    )
elseif (UNIX)
    target_sources(agent PRIVATE
            entry/entry_linux.cpp
            logger/impl/logger_linux.cpp
            jvm/attacher/impl/attacher_linux.cpp
    )
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()