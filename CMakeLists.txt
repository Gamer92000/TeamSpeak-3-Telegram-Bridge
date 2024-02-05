cmake_minimum_required(VERSION 3.10)

project("TelegramBridge")

set(CMAKE_CXX_STANDARD 20)

set(QT_VERSION 5)

if(MSVC)
    set(CMAKE_GENERATOR_TOOLSET "v140")
    set(CMAKE_VS_PLATFORM_TOOLSET "v140")

    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    set(CMAKE_C_FLAGS_DEBUG "/fsanitize=address")

    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2")

    add_definitions(-D_USE_MATH_DEFINES)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

message("Looking for Qt...")

#if(${QT_VERSION} EQUAL 5)
#    list(APPEND CMAKE_PREFIX_PATH "H:/Qt/5.15.2/msvc2019_64")
#else()
#    list(APPEND CMAKE_PREFIX_PATH "H:/Qt/6.6.0/msvc2019_64")
#    add_definitions(-DQT_ADDITIONAL_PACKAGES_PREFIX_PATH="H:/Qt/6.6.0/msvc2019_64")
#endif()

find_package(Qt${QT_VERSION} REQUIRED Widgets Network)

include_directories("ts3client-pluginsdk/include")

set(sources
    src/config.ui
    src/config.cpp
    src/plugin.cpp
    src/update.ui
    src/update.cpp
    src/WebComm.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${sources})

if(MSVC)
    target_compile_options(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        /MT
    )
else()
    target_compile_options(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        -fPIC
        -static-libgcc
        -static-libstdc++
    )
endif()

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::Network
)