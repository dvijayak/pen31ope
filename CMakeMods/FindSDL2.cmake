#------------------------------------------------------------------------------
# Usage: find_package(SDL2 [REQUIRED] [COMPONENTS main])
#
# Sets variables:
#     SDL2_INCLUDE_DIRS
#     SDL2_LIBS
#     SDL2_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

# Check if "main" was specified as a component
set(_SDL2_use_main FALSE)
foreach(_SDL2_component ${SDL2_FIND_COMPONENTS})
    if(_SDL2_component STREQUAL "main")
        set(_SDL2_use_main TRUE)
    else()
        message(WARNING "Unrecognized component \"${_SDL2_component}\"")
    endif()
endforeach()

if(WIN32)
    # Search for SDL2 Debug CMake build in 3rdParty/SDL2/Windows/build
    find_path(SDL2_ROOT "include/SDL.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/SDL2/Windows" NO_DEFAULT_PATH)
    if(SDL2_ROOT)
        if (EXISTS "${SDL2_ROOT}/build/Debug/SDL2.lib")
            set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")
            set(SDL2_LIBS "${SDL2_ROOT}/build/Debug/SDL2.lib")
            set(SDL2_DLLS "${SDL2_ROOT}/build/Debug/SDL2.dll")
            if(_SDL2_use_main)
                list(APPEND SDL2_LIBS "${SDL2_ROOT}/build/Debug/SDL2main.lib")
            endif()
        endif()
    endif()
    if(NOT SDL2_FOUND)
        # Search for SDL2 in 3rdParty/SDL2/Windows
        find_path(SDL2_ROOT "include/SDL.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/SDL2/Windows" NO_DEFAULT_PATH)
        if(SDL2_ROOT)
            set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")
            if("${CMAKE_GENERATOR}" MATCHES "Win32" OR "${CMAKE_GENERATOR_PLATFORM}" MATCHES "x86" OR "${CMAKE_GENERATOR_PLATFORM}" MATCHES "Win32")
                set(SDL2_LIBS "${SDL2_ROOT}/lib/x86/SDL2.lib")
                set(SDL2_DLLS "${SDL2_ROOT}/lib/x86/SDL2.dll")
                if(_SDL2_use_main)
                    list(APPEND SDL2_LIBS "${SDL2_ROOT}/lib/x86/SDL2main.lib")
                endif()
            else()
                set(SDL2_LIBS "${SDL2_ROOT}/lib/x64/SDL2.lib")
                set(SDL2_DLLS "${SDL2_ROOT}/lib/x64/SDL2.dll")
                if(_SDL2_use_main)
                    list(APPEND SDL2_LIBS "${SDL2_ROOT}/lib/x64/SDL2main.lib")
                endif()
            endif()
        endif()
    endif()

    mark_as_advanced(SDL2_ROOT)
    find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS SDL2_DLLS)
else()
    set(SDL2_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
    )

    FIND_PATH(SDL2_INCLUDE_DIRS SDL.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
    )

    FIND_LIBRARY(SDL2_LIBRARY_TEMP
        NAMES SDL2
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
    )

    IF(NOT SDL2_BUILDING_LIBRARY)
        IF(NOT ${SDL2_INCLUDE_DIRS} MATCHES ".framework")
            # Non-OS X framework versions expect you to also dynamically link to
            # SDL2main. This is mainly for Windows and OS X. Other (Unix) platforms
            # seem to provide SDL2main for compatibility even though they don't
            # necessarily need it.
            FIND_LIBRARY(SDL2MAIN_LIBRARY
                NAMES SDL2main
                HINTS
                $ENV{SDL2DIR}
                PATH_SUFFIXES lib64 lib
                PATHS ${SDL2_SEARCH_PATHS}
            )
        ENDIF(NOT ${SDL2_INCLUDE_DIRS} MATCHES ".framework")
    ENDIF(NOT SDL2_BUILDING_LIBRARY)

    # SDL2 may require threads on your system.
    # The Apple build may not need an explicit flag because one of the
    # frameworks may already provide it.
    # But for non-OSX systems, I will use the CMake Threads package.
    IF(NOT APPLE)
    FIND_PACKAGE(Threads)
    ENDIF(NOT APPLE)

    IF(SDL2_LIBRARY_TEMP)
        # For SDL2main
        IF(NOT SDL2_BUILDING_LIBRARY)
            IF(SDL2MAIN_LIBRARY)
                SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
            ENDIF(SDL2MAIN_LIBRARY)
        ENDIF(NOT SDL2_BUILDING_LIBRARY)

        # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa.
        # CMake doesn't display the -framework Cocoa string in the UI even
        # though it actually is there if I modify a pre-used variable.
        # I think it has something to do with the CACHE STRING.
        # So I use a temporary variable until the end so I can set the
        # "real" variable in one-shot.
        IF(APPLE)
            SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
        ENDIF(APPLE)

        # For threads, as mentioned Apple doesn't need this.
        # In fact, there seems to be a problem if I used the Threads package
        # and try using this line, so I'm just skipping it entirely for OS X.
        IF(NOT APPLE)
            SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
        ENDIF(NOT APPLE)

        # Set the final string here so the GUI reflects the final state.
        SET(SDL2_LIBS ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
        # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
        SET(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")
    ENDIF(SDL2_LIBRARY_TEMP)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBS SDL2_INCLUDE_DIRS)

endif()