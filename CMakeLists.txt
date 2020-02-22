cmake_minimum_required(VERSION 3.16)
project(pen31ope)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeMods/")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS}
    "3rdParty"
    "Common"
    "Core"
    "Geometry"
    "Math"
    "Scene"
)
add_executable(pen31ope
    main.cpp
    Game.cpp
    Common/Chrono.cpp
    Core/SDLRenderer.cpp
    Geometry/Mesh.cpp
    Scene/Object3D.cpp
    Scene/Object3DFactory.cpp
    )
target_link_libraries(pen31ope ${SDL2_LIBS} ${SDL2_Image_LIBS})

# Assets
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})

# Copy SDL2 DLLs to output folder on Windows
# Thanks to https://github.com/preshing/CMakeDemo/blob/master/CMakeLists.txt#L21
if(WIN32)
    foreach(DLL ${SDL2_DLLS} ${SDL2_Image_DLLS})
        add_custom_command(TARGET pen31ope POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:pen31ope>)
    endforeach()
endif()

