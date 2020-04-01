cmake_minimum_required(VERSION 3.16)
project(pen31ope)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeMods/")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(Lua REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${LUA_INCLUDE_DIR}
    "3rdParty"
    "Common"
    "Core"
    "Geometry"
    "Math"
    "Scene"
    "Settings"
)
add_executable(pen31ope
    main.cpp
    Game.cpp
    Common/Chrono.cpp
    Core/SDLRenderer.cpp
    Geometry/Mesh.cpp
    Geometry/SDLTextureLoader.cpp
    Math/Matrix.cpp
    Scene/Camera.cpp
    Scene/Object3D.cpp
    Scene/Object3DFactory.cpp
    Settings/LuaAppSettingsFactory.cpp
    )
target_link_libraries(pen31ope ${SDL2_LIBS} ${SDL2_Image_LIBS} ${LUA_LIBRARIES})

# Assets
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})

# Scripts
file(GLOB GAME_SCRIPTS scripts/*)
if(WIN32)
    file(COPY ${GAME_SCRIPTS} DESTINATION ${CMAKE_BINARY_DIR})
else()
    foreach(SCRIPT ${GAME_SCRIPTS})
        get_filename_component(SCRIPT_FILE ${SCRIPT} NAME)
        message(STATUS ${SCRIPT})
        message(STATUS ${SCRIPT_FILE})
        message(STATUS "${CMAKE_BINARY_DIR}/${SCRIPT}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SCRIPT} "${CMAKE_BINARY_DIR}/${SCRIPT_FILE}")
    endforeach()
endif()

# Copy SDL2 DLLs to output folder on Windows
# Thanks to https://github.com/preshing/CMakeDemo/blob/master/CMakeLists.txt#L21
if(WIN32)
    foreach(DLL ${SDL2_DLLS} ${SDL2_Image_DLLS})
        add_custom_command(TARGET pen31ope POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:pen31ope>)
    endforeach()
endif()

