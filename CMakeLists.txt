cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Add missing submodules
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/freetype")
        message(STATUS "Freetype submodule init")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add --force https://github.com/freetype/freetype.git extern/freetype)
    endif()
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/freetype/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

execute_process(COMMAND cmake -B ./extern/freetype/build
        -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release
        ./extern/freetype)
#execute_process(COMMAND cmake --build ./extern/freetype/build )
execute_process(COMMAND cmake --build ./extern/freetype/build --target install )

include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/include)

#add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glad/)
#add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glfw/)
add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/glm/)

include_directories(${PROJECT_SOURCE_DIR}/extern/freetype/include)
link_directories(${PROJECT_SOURCE_DIR}/extern/freetype/build)

find_package(freetype REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

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
target_link_libraries(cmpe126project glfw freetype)