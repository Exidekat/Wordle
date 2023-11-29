cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Find and add any missing submodules
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/freetype/build")
        message(STATUS "Freetype submodule first time init.")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add --force https://github.com/freetype/freetype.git extern/freetype)
    endif()
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/glm/glm")
        message(STATUS "Glm submodule first time init.")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add --force https://github.com/g-truc/glm.git extern/glm)
    endif()
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/glfw/include")
        message(STATUS "Glfw submodule first time init.")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add --force https://github.com/glfw/glfw.git extern/glfw)
    endif()

    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule updates")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules.")
        endif()
    endif()
else()
    message(FATAL_ERROR "Git submodules were not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()

execute_process(COMMAND cmake -B ${PROJECT_SOURCE_DIR}/extern/freetype/build -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release ${PROJECT_SOURCE_DIR}/extern/freetype)
execute_process(COMMAND sudo cmake --build ${PROJECT_SOURCE_DIR}/extern/freetype/build --target install) # installs to machine

link_directories(${PROJECT_SOURCE_DIR}/extern/freetype/build) # freetype does not play fair with add_subdirectory.
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/glm/)
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/glfw/)

find_package(Freetype)
#find_package(glfw3)

include_directories(${PROJECT_SOURCE_DIR}/extern/freetype/include)
include_directories(${PROJECT_SOURCE_DIR}/extern/glm/glm)
include_directories(${PROJECT_SOURCE_DIR}/extern/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(cmpe126project
        src/glad/gl.c
        src/player.cpp
        src/render.cpp
        src/main.cpp
        src/update.cpp
        src/wordManager.cpp)
target_link_libraries(cmpe126project glfw freetype)