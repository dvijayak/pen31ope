cmake_minimum_required(VERSION 3.7)
project(pen31ope)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeMods/")
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS}
    "3rdParty"
    "Common"
    "Core"
    "Geometry"
    "Math"
)
add_executable(pen31ope
    main.cpp
    Game.cpp
    Common/Chrono.cpp
    Core/IRenderer.cpp
    Core/SDLRenderer.cpp
    Geometry/Mesh.cpp
    Math/Vector3.cpp
    )
target_link_libraries(pen31ope ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
# Thanks to https://github.com/preshing/CMakeDemo/blob/master/CMakeLists.txt#L21
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET pen31ope POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:pen31ope>)
    endforeach()
endif()