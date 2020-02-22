#------------------------------------------------------------------------------
# Usage: find_package(SDL2_Image [REQUIRED])
#
# Sets variables:
#     SDL2_Image_LIBS
#     SDL2_Image_DLLS
#
# Updates variables:
#     SDL2_INCLUDE_DIRS
#
# Expects the following variables to be set already:
#     SDL2_SEARCH_PATHS
#     SDL2_INCLUDE_DIRS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

if(WIN32)
    # TODO!!!
    # # Search for SDL2 Debug CMake build in 3rdParty/SDL2/Windows/build
    # find_path(SDL2_ROOT "include/SDL.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/SDL2/Windows" NO_DEFAULT_PATH)
    # if(SDL2_ROOT)
    #     if (EXISTS "${SDL2_ROOT}/build/Debug/SDL2.lib")
    #         set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")
    #         set(SDL2_LIBS "${SDL2_ROOT}/build/Debug/SDL2.lib")
    #         set(SDL2_DLLS "${SDL2_ROOT}/build/Debug/SDL2.dll")
    #         if(_SDL2_use_main)
    #             list(APPEND SDL2_LIBS "${SDL2_ROOT}/build/Debug/SDL2main.lib")
    #         endif()
    #     endif()
    # endif()
    # if(NOT SDL2_FOUND)
    #     # Search for SDL2 in 3rdParty/SDL2/Windows
    #     find_path(SDL2_ROOT "include/SDL.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/SDL2/Windows" NO_DEFAULT_PATH)
    #     if(SDL2_ROOT)
    #         set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")
    #         if("${CMAKE_GENERATOR}" MATCHES "Win64")
    #             set(SDL2_LIBS "${SDL2_ROOT}/lib/x64/SDL2.lib")
    #             set(SDL2_DLLS "${SDL2_ROOT}/lib/x64/SDL2.dll")
    #             if(_SDL2_use_main)
    #                 list(APPEND SDL2_LIBS "${SDL2_ROOT}/lib/x64/SDL2main.lib")
    #             endif()
    #         else()
    #             set(SDL2_LIBS "${SDL2_ROOT}/lib/x86/SDL2.lib")
    #             set(SDL2_DLLS "${SDL2_ROOT}/lib/x86/SDL2.dll")
    #             if(_SDL2_use_main)
    #                 list(APPEND SDL2_LIBS "${SDL2_ROOT}/lib/x86/SDL2main.lib")
    #             endif()
    #         endif()
    #     endif()
    # endif()

    # mark_as_advanced(SDL2_ROOT)
    # find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS SDL2_DLLS)
else()
    FIND_LIBRARY(SDL2_Image_LIBRARY_TEMP
        NAMES SDL2_Image
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
    )

    FIND_PATH(SDL2_Image_INCLUDE_DIRS SDL_image.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
    )
    SET(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2_Image_INCLUDE_DIRS})

    IF(SDL2_Image_LIBRARY_TEMP)
        # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa.
        # CMake doesn't display the -framework Cocoa string in the UI even
        # though it actually is there if I modify a pre-used variable.
        # I think it has something to do with the CACHE STRING.
        # So I use a temporary variable until the end so I can set the
        # "real" variable in one-shot.
        IF(APPLE)
            SET(SDL2_Image_LIBRARY_TEMP ${SDL2_Image_LIBRARY_TEMP} "-framework Cocoa")
        ENDIF(APPLE)

        # Set the final string here so the GUI reflects the final state.
        SET(SDL2_Image_LIBS ${SDL2_Image_LIBRARY_TEMP} CACHE STRING "Where the SDL2_Image Library can be found")
        # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
        SET(SDL2_Image_LIBRARY_TEMP "${SDL2_Image_LIBRARY_TEMP}" CACHE INTERNAL "")
    ENDIF(SDL2_Image_LIBRARY_TEMP)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_Image REQUIRED_VARS SDL2_Image_LIBS SDL2_INCLUDE_DIRS)

endif()