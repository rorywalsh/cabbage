# Minimum CMake version required
cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0149 NEW)

# Set the version of the Cabbage project
set(CABBAGE_VERSION 0.0.100)
# Build options
option(CABBAGE_ENABLE_TESTS "Build tests in addition to main app" OFF)
option(USER_LIB "Output AU plugins directly to ~/Library/Audio/Plug-Ins/Components without Debug subdirectory" OFF)
# CABBAGE_PRO_OPTION_PLACEHOLDER
option(CABBAGE_USE_STATIC_RUNTIME "Use static MSVC runtime library (MT) instead of dynamic (MD)" OFF)

# Enforce Xcode generator on macOS
if(APPLE)
    set(VST3_FORMATS CLAP VST3)
    set(AU_FORMATS CLAP AUV2)
    # CABBAGE_PRO_ARCH_PLACEHOLDER
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
    if(NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
        # Newer Xcode SDKs reject old deployment targets (Xcode 26+ requires
        # a minimum of 12.0; older toolchains still accept 10.15). Probe the
        # active toolchain so the default works everywhere; an explicit
        # -DCMAKE_OSX_DEPLOYMENT_TARGET always wins (see guard above).
        # To raise the floor for a future Xcode, adjust the threshold below.
        set(CABBAGE_XCODE_MAJOR 0)
        execute_process(
            COMMAND xcodebuild -version
            OUTPUT_VARIABLE CABBAGE_XCODE_VERSION_OUTPUT
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(CABBAGE_XCODE_VERSION_OUTPUT MATCHES "Xcode ([0-9]+)")
            set(CABBAGE_XCODE_MAJOR "${CMAKE_MATCH_1}")
        endif()
        unset(CABBAGE_XCODE_VERSION_OUTPUT)
        if(CABBAGE_XCODE_MAJOR GREATER_EQUAL 26)
            set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS deployment target")
        else()
            set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS deployment target")
        endif()
        unset(CABBAGE_XCODE_MAJOR)
    endif()
    if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
        message(FATAL_ERROR "Xcode generator is required on macOS. Please use: cmake -G Xcode ...")
    endif()
else()
    set(VST3_FORMATS CLAP VST3)
    set(AU_FORMATS)  # Empty for non-Apple platforms
endif()




# Define common base directory for all dependency downloads and builds
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps" CACHE PATH "Base directory for downloaded dependencies")


# ----------------------------
# Set MSVC runtime for Windows builds
# ----------------------------
if (NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
  if (WIN32)
    if(CABBAGE_USE_STATIC_RUNTIME)
      message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY not defined. Setting to static link")
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC Runtime Library" FORCE)
    else()
      message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY not defined. Setting to dynamic link")
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC Runtime Library" FORCE)
    endif()
  endif()
endif()



# -----------------------------
# Set compiler standard and build options for CLAP wrapper
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Optional: use standard-compliant mode

option(BUILD_TARGETS "Build targets after configuration" OFF)
option(CABBAGE_ENABLE_TESTS "Build Cabbage unit tests" OFF)

# Initialize PRO_EXTRA_LIBS variable
set(PRO_EXTRA_LIBS "")
# ----------------------------
# If no specific build target is provided, build all targets
# ----------------------------
if(NOT DEFINED CABBAGE_BUILD_TARGET)

    set(CABBAGE_BUILD_TARGETS CabbageApp CabbagePluginEffect CabbagePluginSynth CabbagePluginEffectARA CabbagePluginSynthARA CabbagePluginEffectAUv2 CabbagePluginSynthAUv2 CabbagePluginMidiFxAUv2 CabbageTests)
    message(STATUS "No CABBAGE_BUILD_TARGET specified. Building all targets.")



    # Configure and build each target separately
    foreach(target IN LISTS CABBAGE_BUILD_TARGETS)
        # Skip AU targets on non-Apple platforms
        if((target STREQUAL "CabbagePluginEffectAUv2" OR target STREQUAL "CabbagePluginSynthAUv2") AND NOT APPLE)
            message(STATUS "Skipping ${target} on non-Apple platform")
            continue()
        endif()
        # Skip tests unless explicitly enabled
        if(target STREQUAL "CabbageTests" AND NOT CABBAGE_ENABLE_TESTS)
            message(STATUS "Skipping ${target} (tests not enabled)")
            continue()
        endif()

        message(STATUS "Configuring build for target: ${target}")

        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${target})

        # Only forward ObjC compiler cache vars when building on macOS
        set(_OBJC_COMPILER_ARGS)
        if(APPLE)
            list(APPEND _OBJC_COMPILER_ARGS
                -DCMAKE_OBJC_COMPILER=${CMAKE_OBJC_COMPILER}
                -DCMAKE_OBJCXX_COMPILER=${CMAKE_OBJCXX_COMPILER}
                -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
            )
        endif()

        # Enable ARA SDK for ARA targets
        set(_ARA_ARGS)
        if(target STREQUAL "CabbagePluginEffectARA" OR target STREQUAL "CabbagePluginSynthARA")
            list(APPEND _ARA_ARGS -DLATTICE_ENABLE_ARA=ON)
        endif()

        # Run CMake configuration for the current target - this needs to be a little verbose for GH actions
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -DCABBAGE_BUILD_TARGET=${target}
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DFETCHCONTENT_BASE_DIR=${FETCHCONTENT_BASE_DIR}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                ${_OBJC_COMPILER_ARGS}
                ${_ARA_ARGS}
                -DCMAKE_CXX_STANDARD=20
                -DCMAKE_CXX_STANDARD_REQUIRED=ON
                # CABBAGE_PRO_CMAKE_ARGS_PLACEHOLDER
                -S ${CMAKE_SOURCE_DIR}
                -G ${CMAKE_GENERATOR}
                -B ${CMAKE_BINARY_DIR}/${target}
            RESULT_VARIABLE result
        )

        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to configure ${target}")
        endif()

        # Build the target
        if(BUILD_ALL_TARGETS AND BUILD_ALL_TARGETS STREQUAL "True" OR BUILD_ALL_TARGETS)
            message(STATUS "Building target: ${target}")

            if(NOT CMAKE_BUILD_TYPE)
                set(CMAKE_BUILD_TYPE "Release")
            endif()

            execute_process(
                COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/${target} --config ${CMAKE_BUILD_TYPE}
                RESULT_VARIABLE result
            )
            if(NOT result EQUAL 0)
                message(FATAL_ERROR "Failed to build ${target}")
            endif()
        endif()

    endforeach()

    # Exit after all builds are done
    return()
endif()

# ----------------------------
# Determine languages used based on platform
# ----------------------------
if(APPLE)
    set(BUILD_LANGUAGES CXX OBJCXX)
else()
    set(BUILD_LANGUAGES C CXX)
endif()

# ----------------------------
# Setup project according to specified build target
# ----------------------------
message(STATUS "Setting build target to ${CABBAGE_BUILD_TARGET}")
if (CabbageApp STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbageApp LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
elseif (CabbagePluginEffect STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginEffect LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_VST_TYPE CabbagePluginEffectVST3)
    set(FORMATS ${VST3_FORMATS})
elseif (CabbagePluginSynth STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginSynth LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_VST_TYPE CabbagePluginSynthVST3)
    set(FORMATS ${VST3_FORMATS})
elseif (CabbagePluginEffectARA STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginEffectARA LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_VST_TYPE CabbagePluginEffectARAVST3)
    set(FORMATS ${VST3_FORMATS})
elseif (CabbagePluginSynthARA STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginSynthARA LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_VST_TYPE CabbagePluginSynthARAVST3)
    set(FORMATS ${VST3_FORMATS})
elseif (CabbagePluginEffectAUv2 STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginEffectAUv2 LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_AU_INSTRUMENT_TYPE aufx)
    set(FORMATS ${AU_FORMATS})
elseif (CabbagePluginSynthAUv2 STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginSynthAUv2 LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_AU_INSTRUMENT_TYPE aumf)
    set(FORMATS ${AU_FORMATS})
elseif (CabbagePluginMidiFxAUv2 STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbagePluginMidiFxAUv2 LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
    set(CABBAGE_AU_INSTRUMENT_TYPE aumi)
    set(FORMATS ${AU_FORMATS})
elseif (CabbageTests STREQUAL "${CABBAGE_BUILD_TARGET}")
    project(CabbageTests LANGUAGES ${BUILD_LANGUAGES} VERSION 0.1)
endif()



# ----------------------------
# Include dependencies and modules
# ----------------------------
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

# Enable ARA SDK fetch for ARA targets (must be set before lattice Dependencies.cmake)
if (CabbagePluginEffectARA STREQUAL "${CABBAGE_BUILD_TARGET}" OR CabbagePluginSynthARA STREQUAL "${CABBAGE_BUILD_TARGET}")
    set(LATTICE_ENABLE_ARA ON CACHE BOOL "Enable ARA support" FORCE)
endif()

include(${lattice_SOURCE_DIR}/cmake/Dependencies.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(CSOUND REQUIRED)

set(LATTICE_ROOT_DIR "${lattice_SOURCE_DIR}")
include(${LATTICE_ROOT_DIR}/cmake/lattice.cmake)

# ----------------------------
# Set project name and output name
# ----------------------------
set(PROJECT_NAME ${CABBAGE_BUILD_TARGET})
set(CLAP_WRAPPER_OUTPUT_NAME ${PROJECT_NAME})
add_compile_definitions(${CABBAGE_BUILD_TARGET})

# ----------------------------
# Define opcode-related source files
# ----------------------------
set(CABBAGE_OPCODE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageGetOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageGetOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageSetOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageSetOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageProfilerOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageProfilerOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageCreateOpcode.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageCreateOpcode.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageStateOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageStateOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageFileOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageFileOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageSendMessageOpcode.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageSendMessageOpcode.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageGetWidgetIdsOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageGetWidgetIdsOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageJsonOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageJsonOpcodes.cpp"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageARAOpcodes.h"
    "${CMAKE_SOURCE_DIR}/src/opcodes/CabbageARAOpcodes.cpp"
)

# ----------------------------
# Define core source files
# ----------------------------
set(CABBAGE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/CabbageProcessor.cpp"
    "${CMAKE_SOURCE_DIR}/src/CabbageARAProcessor.cpp"
    "${CMAKE_SOURCE_DIR}/src/CabbageProcessor.h"
    "${CMAKE_SOURCE_DIR}/src/Cabbage.cpp"
    "${CMAKE_SOURCE_DIR}/src/Cabbage.h"
    "${CMAKE_SOURCE_DIR}/src/CabbageColours.h"
    "${CMAKE_SOURCE_DIR}/src/CabbageParser.cpp"
    "${CMAKE_SOURCE_DIR}/src/CabbageParser.h"
    "${CMAKE_SOURCE_DIR}/src/CabbageUtils.cpp"
    "${CMAKE_SOURCE_DIR}/src/CabbageUtils.h"
    "${CMAKE_SOURCE_DIR}/src/CabbagePluginInfo.h"
)

# Add platform-specific file for browseForFile
if(APPLE)
    list(APPEND CABBAGE_SOURCES "${CMAKE_SOURCE_DIR}/src/CabbageUtils_mac.mm")
elseif(WIN32)
    list(APPEND CABBAGE_SOURCES "${CMAKE_SOURCE_DIR}/src/CabbageUtils_win.cpp")
else()
    list(APPEND CABBAGE_SOURCES "${CMAKE_SOURCE_DIR}/src/CabbageUtils_linux.cpp")
endif()

# CABBAGE_PRO_SOURCES_PLACEHOLDER

# ----------------------------
# Combine all source files
# ----------------------------
set(SOURCE_FILES
    ${CABBAGE_SOURCES}
    ${CABBAGE_OPCODE_SOURCES}
    ${LATTICE_SOURCE_FILES}
)

# ----------------------------
# Organize source files into groups
# ----------------------------
source_group("Cabbage Source" FILES ${CABBAGE_SOURCES})
source_group("Cabbage Opcodes" FILES ${CABBAGE_OPCODE_SOURCES})
source_group("Lattice Source" FILES ${LATTICE_SOURCE_FILES})

# ----------------------------
# Set include directories for all targets
# ----------------------------
include_directories(
    ${lattice_SOURCE_DIR}/src
    ${choc_SOURCE_DIR}
    ${catch2_SOURCE_DIR}
    ${concurrentqueue_SOURCE_DIR}
    ${LATTICE_INCLUDE_DIRS}
    ${CSOUND_INCLUDE_DIR}
)

# ----------------------------
# Build Executable Target (App)
# ----------------------------
# In order to debug CabbageApp without vscode - start with it the following command:
# --file"path_to_file.csd" --portNumber=NNNN --startTestServer=True

if (CabbageApp STREQUAL "${CABBAGE_BUILD_TARGET}")
    set(CABBAGE_SERVICE_APP_SOURCES
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/main.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioApp.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioApp.h"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioRecorder.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioRecorder.h"
    )
    list(APPEND SOURCE_FILES ${CABBAGE_SERVICE_APP_SOURCES})
    source_group("Cabbage Service App" FILES ${CABBAGE_SERVICE_APP_SOURCES})

    add_executable(${PROJECT_NAME}  ${SOURCE_FILES})

    # Set include paths for external dependencies
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${rtaudio_SOURCE_DIR}
        ${rtmidi_SOURCE_DIR}
    )


    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLUGIN_INFO_HEADER="${CMAKE_SOURCE_DIR}/src/CabbagePluginInfo.h"
        PRODUCT_NAME="${PROJECT_NAME}"
        USE_DOUBLE=1
        "${CABBAGE_BUILD_TARGET}"
    )

    # CABBAGE_PRO_DEFINITIONS_PLACEHOLDER

    if(WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    endif()

    # Link libraries
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/rtaudio-build)
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/rtmidi-build)

    if (APPLE)
        # CoreMIDI: CabbageAudioApp.cpp probes MIDIClientCreate directly (see
        # coreMidiClientAvailable) because the vendored RtMidi turns a failed
        # client creation into std::terminate() instead of a catchable
        # exception (its CoreMIDI singleton creator is declared throw()).
        target_link_libraries(${PROJECT_NAME} PRIVATE rtaudio rtmidi ${CSOUND_FRAMEWORK} "-framework Cocoa" "-framework CoreMIDI" ${PRO_EXTRA_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE rtaudio rtmidi ${CSOUND_LIBRARY} ${PRO_EXTRA_LIBS})
    endif()

    # Use C++17 for the executable
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    if(LINUX AND TARGET WebViewProcess)
        add_dependencies(${PROJECT_NAME} WebViewProcess)
    endif()

endif()

# ----------------------------
# Build Test Target (CabbageTests)
# ----------------------------
if (CabbageTests STREQUAL "${CABBAGE_BUILD_TARGET}")
    enable_testing()

    # Create test-specific source list (exclude platform-specific files)
    set(CABBAGE_TEST_SOURCES
        "${CMAKE_SOURCE_DIR}/src/CabbageProcessor.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageARAProcessor.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageProcessor.h"
        "${CMAKE_SOURCE_DIR}/src/Cabbage.cpp"
        "${CMAKE_SOURCE_DIR}/src/Cabbage.h"
        "${CMAKE_SOURCE_DIR}/src/CabbageColours.h"
        "${CMAKE_SOURCE_DIR}/src/CabbageParser.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageParser.h"
        "${CMAKE_SOURCE_DIR}/src/CabbageUtils.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageUtils.h"
        "${CMAKE_SOURCE_DIR}/src/CabbagePluginInfo.h"
    )

    # Create test executable
    add_executable(${PROJECT_NAME}
        "${CMAKE_SOURCE_DIR}/tests/CabbageAppTests.cpp"
        "${CMAKE_SOURCE_DIR}/tests/JsoncParsingTests.cpp"
        "${CMAKE_SOURCE_DIR}/tests/TestCsdFiles.h"
        ${CABBAGE_TEST_SOURCES}
        ${CABBAGE_OPCODE_SOURCES}
        ${LATTICE_SOURCE_FILES}
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioApp.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioApp.h"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioRecorder.cpp"
        "${CMAKE_SOURCE_DIR}/src/CabbageAudioApp/CabbageAudioRecorder.h"
        "${CMAKE_SOURCE_DIR}/tests/FileBrowseStub.cpp"
    )
    
    # Set include paths for test target
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${rtaudio_SOURCE_DIR}
        ${rtmidi_SOURCE_DIR}
    )

    if(LINUX)
        # LatticeClapPlugin.cpp includes the generated
        # LinuxWebviewProcess/webview_binary.h, produced by the WebViewProcess
        # target's POST_BUILD step. The lattice helper that orders this for
        # plugin targets is never called for CabbageTests, so order it here.
        add_dependencies(${PROJECT_NAME} WebViewProcess)
    endif()
    
    # Set compile definitions for test target
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLUGIN_INFO_HEADER="${CMAKE_SOURCE_DIR}/src/CabbagePluginInfo.h"
        PRODUCT_NAME="CabbageApp"
        USE_DOUBLE=1
        CabbageTests
        CabbageApp
    )

    # Optional: absolute path to a vscabbage checkout's src/ dir, used by
    # tests to bootstrap the shared settings file. Pass with
    # -DTEST_VSCABBAGE_DIR=<path>. CI clones vscabbage next to cabbage3.
    # Normalised to forward slashes: a raw Windows path would otherwise
    # lose its backslashes to escape processing in the string literal.
    if(DEFINED TEST_VSCABBAGE_DIR)
        file(TO_CMAKE_PATH "${TEST_VSCABBAGE_DIR}" TEST_VSCABBAGE_DIR_NORM)
        target_compile_definitions(${PROJECT_NAME} PRIVATE TEST_VSCABBAGE_DIR="${TEST_VSCABBAGE_DIR_NORM}")
    endif()

    # CABBAGE_PRO_DEFINITIONS_PLACEHOLDER

    if(WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    endif()

    # Link libraries for test target
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/rtaudio-build)
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/rtmidi-build)
    
    if (APPLE)
        # Ensure Cocoa is linked for UI code (NSOpenPanel etc.)
        target_link_libraries(${PROJECT_NAME} PRIVATE rtaudio rtmidi ${CSOUND_FRAMEWORK} "-framework Cocoa" ${PRO_EXTRA_LIBS} Catch2::Catch2WithMain)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE rtaudio rtmidi ${CSOUND_LIBRARY} ${PRO_EXTRA_LIBS} Catch2::Catch2WithMain)
    endif()
    
    # Set C++ standard for test target
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    # Register each Catch2 TEST_CASE as its own ctest test so that a hung
    # case is reported (and timed out via ctest --timeout) individually
    # instead of stalling the whole binary behind one buffered log.
    include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
    catch_discover_tests(${PROJECT_NAME}
        EXTRA_ARGS "--reporter compact"
    )
endif()

# ----------------------------
# Build Plugin Library Target
# ----------------------------
if (NOT (CabbageApp STREQUAL "${CABBAGE_BUILD_TARGET}") AND NOT (CabbageTests STREQUAL "${CABBAGE_BUILD_TARGET}"))
    add_library(${PROJECT_NAME}-impl STATIC ${SOURCE_FILES})

    target_compile_definitions(${PROJECT_NAME}-impl PRIVATE
        PLUGIN_INFO_HEADER="${CMAKE_SOURCE_DIR}/src/CabbagePluginInfo.h"
        PRODUCT_NAME="${PROJECT_NAME}"
        "${CABBAGE_BUILD_TARGET}"
        "${CABBAGE_VST_TYPE}"
    )

    # Enable ARA for ARA plugin targets
    if (CabbagePluginEffectARA STREQUAL "${CABBAGE_BUILD_TARGET}" OR CabbagePluginSynthARA STREQUAL "${CABBAGE_BUILD_TARGET}")
        target_compile_definitions(${PROJECT_NAME}-impl PRIVATE
            LATTICE_ENABLE_ARA=1
            LATTICE_HAS_ARA=1
            LATTICE_PLUGIN_PROVIDES_ARA_FACTORY=1
        )
        # ARA SDK library sources are not merged into LATTICE_SOURCE_FILES automatically
        if(LATTICE_ARA_LIBRARY_SOURCE_FILES)
            target_sources(${PROJECT_NAME}-impl PRIVATE ${LATTICE_ARA_LIBRARY_SOURCE_FILES})
        endif()
    endif()

    # CABBAGE_PRO_PLUGIN_DEFINITIONS_PLACEHOLDER

    # Platform-specific options
    if (WIN32)
        target_compile_definitions(${PROJECT_NAME}-impl PRIVATE _CRT_SECURE_NO_WARNINGS)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "Multithreaded")
    else()
        target_compile_options(${PROJECT_NAME}-impl PRIVATE -Wall -Wextra -pedantic)
    endif()

    # On Linux, WebViewProcess must finish building (and generate webview_binary.h)
    # before LatticeClapPlugin.cpp can be compiled
    if(LINUX AND TARGET WebViewProcess)
        add_dependencies(${PROJECT_NAME}-impl WebViewProcess)
    endif()


    if (APPLE)
        # Add WebKit and Cocoa frameworks for choc::ui::WebView support
        target_link_libraries(${PROJECT_NAME}-impl PUBLIC clap clap-helpers "-framework Cocoa" "-framework WebKit" ${CSOUND_FRAMEWORK} ${PRO_EXTRA_LIBS})
    elseif(WIN32)
        # On Windows, link against CSOUND and any extra Pro libraries
        target_link_libraries(${PROJECT_NAME}-impl PUBLIC ${CSOUND_LIBRARY} ${PRO_EXTRA_LIBS} ${LATTICE_LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME}-impl PUBLIC ${CSOUND_LIBRARY} ${PRO_EXTRA_LIBS})
    endif()

    # Generate plugin formats
    make_clapfirst_plugins(
        TARGET_NAME ${PROJECT_NAME}
        IMPL_TARGET ${PROJECT_NAME}-impl
        OUTPUT_NAME "${PROJECT_NAME}"
        ENTRY_SOURCE ${LATTICE_ROOT_DIR}/src/lattice/clap/Factory.cpp
        BUNDLE_IDENTIFER "org.yourproject.${PROJECT_NAME}"
        BUNDLE_VERSION ${PROJECT_VERSION}
        COPY_AFTER_BUILD ${COPY_AFTER_BUILD}
        PLUGIN_FORMATS ${FORMATS}
        ASSET_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_assets
        AUV2_INSTRUMENT_TYPE "${CABBAGE_AU_INSTRUMENT_TYPE}"
        # STANDALONE_CONFIGURATIONS
        #     standalone "${PROJECT_NAME}" "your.reversed.domain.name.${PROJECT_NAME}"
    )

    # Set Reaper as the debug executable for plugin targets
    if(APPLE AND CMAKE_GENERATOR STREQUAL "Xcode")
        # Adjust this path if Reaper is installed elsewhere
        set(REAPER_PATH "/Applications/REAPER.app")
        
        # Apply to CLAP target
        if(TARGET ${PROJECT_NAME}_clap)
            set_target_properties(${PROJECT_NAME}_clap PROPERTIES
                XCODE_SCHEME_EXECUTABLE "${REAPER_PATH}"
            )
        endif()
        
        # Apply to VST3 target
        if(TARGET ${PROJECT_NAME}_vst3)
            set_target_properties(${PROJECT_NAME}_vst3 PROPERTIES
                XCODE_SCHEME_EXECUTABLE "${REAPER_PATH}"
            )
        endif()
        
        # Apply to AUv2 target (if it exists)
        if(TARGET ${PROJECT_NAME}_auv2)
            set_target_properties(${PROJECT_NAME}_auv2 PROPERTIES
                XCODE_SCHEME_EXECUTABLE "${REAPER_PATH}"
            )
        endif()
    elseif(WIN32)
        # Adjust this path if Reaper is installed elsewhere
        set(REAPER_PATH "C:/Program Files/REAPER (x64)/Reaper.exe")
        
        # Apply to CLAP target
        if(TARGET ${PROJECT_NAME}_clap)
            set_target_properties(${PROJECT_NAME}_clap PROPERTIES
                VS_DEBUGGER_COMMAND "${REAPER_PATH}"
            )
        endif()
        
        # Apply to VST3 target
        if(TARGET ${PROJECT_NAME}_vst3)
            set_target_properties(${PROJECT_NAME}_vst3 PROPERTIES
                VS_DEBUGGER_COMMAND "${REAPER_PATH}"
            )
        endif()

    endif()

    if(TARGET ${PROJECT_NAME}_auv2)
        # If USER_LIB flag is set, override output directories to place plugins directly 
        # in ~/Library/Audio/Plug-Ins/Components without Debug/Release subdirectories
        if(USER_LIB)
            set_target_properties(${PROJECT_NAME}_auv2 PROPERTIES
                LIBRARY_OUTPUT_DIRECTORY_DEBUG /Users/rwalsh/Library/Audio/Plug-Ins/Components
                LIBRARY_OUTPUT_DIRECTORY_RELEASE /Users/rwalsh/Library/Audio/Plug-Ins/Components
                LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO /Users/rwalsh/Library/Audio/Plug-Ins/Components
                LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL /Users/rwalsh/Library/Audio/Plug-Ins/Components
            )
        endif()
    endif()

endif()