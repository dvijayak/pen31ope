cmake_minimum_required(VERSION 3.7)
project(3dlib)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeMods/")
find_package(SDL2 REQUIRED)

message("${SDL2} ${SDL2_INCLUDE_DIRS} ${SDL2_LIBS}")

include_directories(${SDL2_INCLUDE_DIRS} "3rdParty" "Common" "Core")
add_executable(3dlib
    main.cpp
    Game.cpp
    Common/Chrono.cpp
    Core/IRenderer.cpp
    Core/SDLRenderer.cpp
    )
target_link_libraries(3dlib ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
# Thanks to https://github.com/preshing/CMakeDemo/blob/master/CMakeLists.txt#L21
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET 3dlib POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:3dlib>)
    endforeach()
endif()