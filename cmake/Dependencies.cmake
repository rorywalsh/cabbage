include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2 
)

# ----------------------------
# Only need these libraries for CabbageServiceApp and CabbageTests
# ----------------------------
if (CabbageApp STREQUAL "${CABBAGE_BUILD_TARGET}" OR CabbageTests STREQUAL "${CABBAGE_BUILD_TARGET}")
    message(STATUS "Including RtAudio/RtMidi for ${CABBAGE_BUILD_TARGET}")

    # Use FetchContent to include RtAudio first
    FetchContent_Declare(
        rtaudio
        GIT_REPOSITORY https://github.com/thestk/rtaudio.git
        GIT_TAG 6.0.1
    )

    # Disable unnecessary backends on macOS
    if (APPLE)
        set(RTAUDIO_API_JACK OFF CACHE BOOL "Disable JACK support" FORCE)
        set(RTAUDIO_API_PULSE OFF CACHE BOOL "Disable PulseAudio support" FORCE)
        set(RTAUDIO_API_ALSA OFF CACHE BOOL "Disable ALSA support" FORCE)
        set(RTMIDI_API_JACK OFF CACHE BOOL "Disabling Jack for RtMidi" FORCE)
    endif()


    # On Windows always build all three backends. RtAudio 6.0.1 defaults to
    # WASAPI-only (DS/ASIO default OFF), which breaks DirectSound/ASIO selection.
    # ASIO sources are bundled with RtAudio, no external SDK is required.
    if (WIN32)
        set(RTAUDIO_API_WASAPI ON CACHE BOOL "Enable WASAPI support" FORCE)
        set(RTAUDIO_API_DS ON CACHE BOOL "Enable DirectSound support" FORCE)
        set(RTAUDIO_API_ASIO ON CACHE BOOL "Enable ASIO support" FORCE)
    endif()

    # Disable testing to avoid conflict with RtAudio
    set(RTMIDI_BUILD_TESTING OFF CACHE BOOL "Disable RtMidi tests" FORCE)
    set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Disable RtAudio tests" FORCE)

    # Build static libraries for rtaudio and rtmidi
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libraries" FORCE)
    set(RTAUDIO_BUILD_SHARED_LIBRARY OFF CACHE BOOL "Build static library for RtAudio" FORCE)
    set(RTMIDI_BUILD_SHARED_LIBRARY OFF CACHE BOOL "Build static library for RtMidi" FORCE)

    # Prevent RtAudio/RtMidi from forcing static runtime when building static libs
    set(RTAUDIO_STATIC_MSVCRT OFF CACHE BOOL "Use dynamic MSVC runtime for RtAudio" FORCE)
    set(RTMIDI_STATIC_MSVCRT OFF CACHE BOOL "Use dynamic MSVC runtime for RtMidi" FORCE)

    # Use FetchContent to include RtMidi
    FetchContent_Declare(
        rtmidi
        GIT_REPOSITORY https://github.com/thestk/rtmidi.git
        GIT_TAG master
    )

    # Make all dependencies available
    FetchContent_MakeAvailable(rtaudio rtmidi)

    if (WIN32)
        target_compile_options(rtmidi PRIVATE /std:c++20)
        target_compile_options(rtaudio PRIVATE /std:c++20)
    else()
        target_compile_features(rtmidi PRIVATE cxx_std_20)
        target_compile_features(rtaudio PRIVATE cxx_std_20)
    endif()

    # Only make catch2 available when building tests
    if(CabbageTests STREQUAL "${CABBAGE_BUILD_TARGET}")
        FetchContent_MakeAvailable(catch2)
    endif()

endif()

# Lattice and ConcurrentQueue Dependencies
set(LATTICE_BUILD_EXAMPLES OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED_LATTICE ON)
FetchContent_Declare(
    lattice
    GIT_REPOSITORY https://github.com/rorywalsh/lattice.git
    GIT_TAG 28af718a1d8f0d59fad0d6dc6ac95a0e6aaaa038
)

FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG master
)



# Include patch functionality
include(${CMAKE_SOURCE_DIR}/cmake/ChocPatches.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/ClapWrapperPatches.cmake)

# Make all dependencies available
FetchContent_MakeAvailable(lattice concurrentqueue)


# Apply patches after dependencies are downloaded
if(APPLE)
    apply_choc_patches()
endif()

# Apply clap-wrapper patches (needed for all platforms with VST3 support)
apply_clap_wrapper_patches()