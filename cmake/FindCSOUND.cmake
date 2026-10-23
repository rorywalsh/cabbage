# Try to find the Csound library.
# Defines the following variables:
#  CSOUND_FOUND - System has the Csound library
#  CSOUND_INCLUDE_DIRS - The Csound include directories.
#  CSOUND_LIBRARIES - The libraries needed to use the Csound library.

message(STATUS "Using custom FindCSOUND.cmake")

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

