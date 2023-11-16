cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-x86_64)

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib)
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++11")

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-arm64)

    include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include)
    link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
        include/queueType.h
        include/stackType.h
        include/Player.h
        src/Player.cpp
        src/main.cpp)
add_definitions(-DGLEW_STATIC)
add_executable(cmpe126project ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(cmpe126project GLEW glfw3 glfw GL)
elseif (APPLE)
    target_link_libraries(cmpe126project glew glfw3)
endif ()
