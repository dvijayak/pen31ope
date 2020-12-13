#------------------------------------------------------------------------------
# Usage: find_package(SDL2_ttf [REQUIRED])
#
# Sets variables:
#     SDL2_ttf_LIBS
#     SDL2_ttf_DLLS
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
   # Search for SDL2_ttf in 3rdParty/SDL2_ttf/Windows
   find_path(SDL2_ttf_ROOT "include/SDL_ttf.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/SDL2_ttf/Windows" NO_DEFAULT_PATH)
   if(SDL2_ttf_ROOT)
      set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} "${SDL2_ttf_ROOT}/include")
      if("${CMAKE_GENERATOR}" MATCHES "Win32" OR "${CMAKE_GENERATOR_PLATFORM}" MATCHES "x86" OR "${CMAKE_GENERATOR_PLATFORM}" MATCHES "Win32")
            set(SDL2_ttf_LIBS "${SDL2_ttf_ROOT}/lib/x86/SDL2_ttf.lib")
            file(GLOB SDL2_ttf_DLLS "${SDL2_ttf_ROOT}/lib/x86/*.dll")
      else()
            set(SDL2_ttf_LIBS "${SDL2_ttf_ROOT}/lib/x64/SDL2_ttf.lib")
            file(GLOB SDL2_ttf_DLLS "${SDL2_ttf_ROOT}/lib/x64/*.dll")
      endif()
   endif()

    mark_as_advanced(SDL2_ttf_ROOT)
    find_package_handle_standard_args(SDL2_ttf DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_ttf_LIBS SDL2_ttf_DLLS)
else()
    FIND_LIBRARY(SDL2_ttf_LIBRARY_TEMP
        NAMES SDL2_ttf
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
    )

    FIND_PATH(SDL2_ttf_INCLUDE_DIRS SDL_ttf.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
    )
    SET(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2_ttf_INCLUDE_DIRS})

    IF(SDL2_ttf_LIBRARY_TEMP)
        # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa.
        # CMake doesn't display the -framework Cocoa string in the UI even
        # though it actually is there if I modify a pre-used variable.
        # I think it has something to do with the CACHE STRING.
        # So I use a temporary variable until the end so I can set the
        # "real" variable in one-shot.
        IF(APPLE)
            SET(SDL2_ttf_LIBRARY_TEMP ${SDL2_ttf_LIBRARY_TEMP} "-framework Cocoa")
        ENDIF(APPLE)

        # Set the final string here so the GUI reflects the final state.
        SET(SDL2_ttf_LIBS ${SDL2_ttf_LIBRARY_TEMP} CACHE STRING "Where the SDL2_ttf Library can be found")
        # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
        SET(SDL2_ttf_LIBRARY_TEMP "${SDL2_ttf_LIBRARY_TEMP}" CACHE INTERNAL "")
    ENDIF(SDL2_ttf_LIBRARY_TEMP)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_ttf REQUIRED_VARS SDL2_ttf_LIBS SDL2_INCLUDE_DIRS)

endif()