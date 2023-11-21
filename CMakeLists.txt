cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    endif()

    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-x86_64)
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++11")

    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-arm64) 
endif ()

include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLAD/include)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
        Dependencies/GLAD/src/gl.c

        include/queueType.h
        include/stackType.h
        include/Player.h
        src/Player.cpp
        src/main.cpp)
add_definitions(-DGLEW_STATIC)
add_executable(cmpe126project ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(cmpe126project glfw3)
elseif (APPLE)
    target_link_libraries(cmpe126project glfw3)
endif ()
