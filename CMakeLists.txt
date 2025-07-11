cmake_minimum_required(VERSION 3.22)
project(cmpe126project)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
    message(FATAL_ERROR "Your compiler is outdated! Update GCC >= 7.0 to build this project.")
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
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

execute_process(COMMAND cmake -B ${PROJECT_SOURCE_DIR}/extern/freetype/build -D CMAKE_POLICY_VERSION_MINIMUM=3.5 -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release ${PROJECT_SOURCE_DIR}/extern/freetype)
#execute_process(COMMAND sudo cmake --build ${PROJECT_SOURCE_DIR}/extern/freetype/build --target install) # installs freetype to machine
execute_process(COMMAND cmake --build ${PROJECT_SOURCE_DIR}/extern/freetype/build) # does not install to machine
link_directories(${PROJECT_SOURCE_DIR}/extern/freetype/build) # freetype does not play fair with add_subdirectory.
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/glm/)
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/glfw/)

find_package(Freetype)

include_directories(${PROJECT_SOURCE_DIR}/extern/freetype/include)
include_directories(${PROJECT_SOURCE_DIR}/extern/glm/glm)
include_directories(${PROJECT_SOURCE_DIR}/extern/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(Wordle
        src/glad/gl.c
        src/render.cpp
        src/main.cpp
        src/update.cpp
        src/wordManager.cpp)
      target_link_libraries(Wordle glfw freetype)

# Add this post-build command to copy the Freetype DLL to the executable directory
add_custom_command(TARGET Wordle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/extern/freetype/build/libfreetype.dll"
    $<TARGET_FILE_DIR:Wordle>)
