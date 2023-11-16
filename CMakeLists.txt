cmake_minimum_required(VERSION 3.26)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR /Users/exide/cmpe126project)
set(APPLE 1)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2019)

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib)
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++11")

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-arm64)

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib)
endif ()

set(SOURCE_FILES main.cpp)
add_definitions(-DGLEW_STATIC)
add_executable(cmpe126project ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(cmpe126project glew32s glfw3 opengl32)
elseif (APPLE)
    target_link_libraries(cmpe126project glew glfw3)
endif ()