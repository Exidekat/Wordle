cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        link_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-mingw-w64)
        link_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-x86_64)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
        link_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022)
    endif()
elseif (APPLE)
    #link_directories(/opt/homebrew/Cellar/glfw/3.3.8/lib)
    #link_directories(/opt/homebrew/Cellar/libpng/1.6.40/lib)
    #link_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-arm64)

endif ()
link_directories(${PROJECT_SOURCE_DIR}/Dependencies/freetype/libs)

include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/freetype/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/include)

#add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glad/)
#add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glfw/)
add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/freetype/)
add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glm/)

find_package(Freetype REQUIRED)
#find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_definitions(-DGLEW_STATIC)
add_executable(cmpe126project
        Dependencies/glad/src/gl.c
        include/queueType.h
        include/stackType.h
        include/player.h
        include/render.h
        include/wordle.h
        src/player.cpp
        src/render.cpp
        src/main.cpp
        src/update.cpp
        src/wordManager.cpp)
target_link_libraries(cmpe126project glfw Freetype)