# Try to find the Csound library.
# Defines the following variables:
#  CSOUND_FOUND - System has the Csound library
#  CSOUND_INCLUDE_DIRS - The Csound include directories.
#  CSOUND_LIBRARIES - The libraries needed to use the Csound library.

message(STATUS "Using custom FindCSOUND.cmake")

# Prefer Csound's installed CMake package when available. The official Windows
# and macOS installers ship CsoundConfig.cmake, which knows the current library
# naming and layout (csound7_64/lib64 on Windows, CsoundLib64.framework on macOS).
set(_csound_config_hints
    "$ENV{ProgramFiles}/Csound7/cmake"
    "$ENV{ProgramW6432}/Csound7/cmake"
    "/Applications/Csound/CsoundLib64.framework/Resources/cmake"
    "$ENV{HOME}/Applications/Csound/CsoundLib64.framework/Resources/cmake"
    "/usr/local/share/csound"
    "/usr/share/csound"
)
foreach(_prefix IN LISTS CMAKE_PREFIX_PATH)
    list(APPEND _csound_config_hints "${_prefix}/share/csound")
endforeach()
find_package(Csound CONFIG QUIET HINTS ${_csound_config_hints})
unset(_csound_config_hints)
unset(_prefix)

if(TARGET Csound::Csound)
    message(STATUS "Using Csound CMake package from ${Csound_DIR} (Csound::Csound)")
    set(CSOUND_FOUND TRUE)
    set(CSOUND_LIBRARY Csound::Csound)
    set(CSOUND_LIBRARIES Csound::Csound)
    if(DEFINED Csound_VERSION)
        set(CSOUND_VERSION "${Csound_VERSION}")
    endif()

    if(APPLE)
        # Cabbage's Apple targets link ${CSOUND_FRAMEWORK}.
        set(CSOUND_FRAMEWORK Csound::Csound)
    endif()

    # Keep the legacy include variables populated for callers that use them
    # directly. The imported target also propagates these when linked.
    get_target_property(_csound_backing_targets Csound::Csound INTERFACE_LINK_LIBRARIES)
    if(_csound_backing_targets)
        list(GET _csound_backing_targets 0 _csound_backing_target)
        if(TARGET "${_csound_backing_target}")
            get_target_property(_csound_include_dirs "${_csound_backing_target}" INTERFACE_INCLUDE_DIRECTORIES)
            if(_csound_include_dirs)
                set(CSOUND_INCLUDE_DIR "${_csound_include_dirs}")
                set(CSOUND_INCLUDE_DIRS "${_csound_include_dirs}")
            endif()
            unset(_csound_include_dirs)
        endif()
        unset(_csound_backing_target)
    endif()
    unset(_csound_backing_targets)

    return()
endif()

if(APPLE)

    unset(CSOUND_INCLUDE_DIR CACHE)
    unset(CSOUND_FRAMEWORK CACHE)
    unset(CSOUND_LIBRARY CACHE)

    find_path(CSOUND_INCLUDE_DIR csound.h
        HINTS
            "$ENV{HOME}/Applications/Csound/CsoundLib64.framework/Headers"
            /Applications/Csound/CsoundLib64.framework/Headers
            ${CSOUND_INCLUDE_DIR_HINT}
        NO_DEFAULT_PATH
    )

    find_path(CSOUND_FRAMEWORK CsoundLib64
        HINTS
            "$ENV{HOME}/Applications/Csound/CsoundLib64.framework"
            /Applications/Csound/CsoundLib64.framework
            ${CSOUND_FRAMEWORK_DIR_HINT}
        NO_DEFAULT_PATH
    )

    find_library(CSOUND_LIBRARY
        NAMES CsoundLib64
        HINTS
            /Applications/Csound
            "$ENV{HOME}/Applications/Csound"
            /Applications/Csound/CsoundLib64.framework/
            "$ENV{HOME}/Applications/Csound/CsoundLib64.framework"
            ${CSOUND_LIBRARY_DIR_HINT}
        NO_DEFAULT_PATH
    )

elseif(WIN32)

    find_path(CSOUND_INCLUDE_DIR csound.h
        PATH_SUFFIXES csound
        HINTS
            "$ENV{ProgramFiles}/Csound7/include"
            "$ENV{ProgramW6432}/Csound7/include"
            ${CSOUND_INCLUDE_DIR_HINT}
    )

    # Csound 7 uses csound7_64.lib, and the official Windows build enables
    # USE_LIB64 so the import library is installed under lib64. Older builds
    # used csound64.lib in lib, so accept both names and locations.
    find_library(CSOUND_LIBRARY
        NAMES csound7_64 csound64
        PATH_SUFFIXES lib lib64 bin
        HINTS
            "$ENV{ProgramFiles}/Csound7"
            "$ENV{ProgramW6432}/Csound7"
            ${CSOUND_LIBRARY_DIR_HINT}
    )

else() # LINUX

    find_path(CSOUND_INCLUDE_DIR csound.h
        HINTS
            ${CMAKE_PREFIX_PATH}/include/csound
            ${CMAKE_PREFIX_PATH}/include
            /usr/local/include/csound
            /usr/include/csound
            ${CSOUND_INCLUDE_DIR_HINT}
    )

    find_library(CSOUND_LIBRARY
        NAMES csound64
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
            /usr/local/lib
            /usr/lib
    )

    find_library(LIBSNDFILE_LIBRARY NAMES sndfile libsndfile-1 libsndfile)

endif()


include(FindPackageHandleStandardArgs)

# Handle the QUIETLY and REQUIRED arguments and set CSOUND_FOUND to TRUE if all listed variables are TRUE.ß
find_package_handle_standard_args(CSOUND
    CSOUND_LIBRARY CSOUND_INCLUDE_DIR
)

mark_as_advanced(CSOUND_INCLUDE_DIR CSOUND_LIBRARY)


set(CSOUND_INCLUDE_DIRS ${CSOUND_INCLUDE_DIR})
set(CSOUND_LIBRARIES ${CSOUND_LIBRARY})

