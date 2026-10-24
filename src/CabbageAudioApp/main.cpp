/*
 * Copyright (c) 2024 Rory Walsh
 *
 * This file is part of Cabbage3
 *
 * Cabbage3 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cabbage3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cabbage3.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CabbageAudioApp.h"
#include <thread>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

// Logs binary path, compile stamp and file mtime (UTC ISO + local).
// Uses logDebug so the frontend only shows it when verbose logging is enabled.
static void logBinaryIdentity(const char *argv0)
{
    try
    {
        const std::string exePath = argv0 ? argv0 : "<unknown>";
        std::string mtimeUtc = "<unknown>";
        std::string mtimeLocal = "<unknown>";
        long long fileSize = -1;

        std::error_code ec;
        if (!exePath.empty() && std::filesystem::exists(exePath, ec))
        {
            fileSize = static_cast<long long>(std::filesystem::file_size(exePath, ec));
            auto ftime = std::filesystem::last_write_time(exePath, ec);
            if (!ec)
            {
                const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now());
                const std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
                std::tm utcTm{};
                std::tm localTm{};
#ifdef _WIN32
                gmtime_s(&utcTm, &tt);
                localtime_s(&localTm, &tt);
#else
                gmtime_r(&tt, &utcTm);
                localtime_r(&tt, &localTm);
#endif
                char utcBuf[32]{};
                char localBuf[32]{};
                std::strftime(utcBuf, sizeof(utcBuf), "%Y-%m-%dT%H:%M:%SZ", &utcTm);
                std::strftime(localBuf, sizeof(localBuf), "%Y-%m-%d %H:%M:%S", &localTm);
                mtimeUtc = utcBuf;
                mtimeLocal = localBuf;
            }
        }

        lattice::logDebug << "CabbageApp binary: " << exePath << " | built " << __DATE__ << " " << __TIME__
                          << " | mtime UTC " << mtimeUtc << " | mtime local " << mtimeLocal << " | " << fileSize
                          << " bytes";
    }
    catch (...)
    {
        // Never block startup for a diagnostic log
    }
}

// Static pointer to the CabbageAudioApp instance
static CabbageAudioApp* appInstance = nullptr;
volatile sig_atomic_t terminateRequested = 0;


void signalHandler(int signal)
{
    lattice::logError << "Received signal " << signal << ". Cleaning up...";
    if (appInstance) {
        try {
            terminateRequested = 1;
            // Send both commands needed for clean shutdown
            appInstance->addMessageToQueue(CabbageAudioApp::CommandType::StopAudio);
            appInstance->addMessageToQueue(CabbageAudioApp::CommandType::KillProcessor);
        } catch (...) {
            std::cerr << "Error during cleanup" << std::endl;
        }
    }
}

#ifdef _WIN32
// Console handler for Windows
BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        if (appInstance) {
            std::cout << "Received console event. Cleaning up..." << std::endl;
            terminateRequested = 1;
            appInstance->addMessageToQueue(CabbageAudioApp::CommandType::StopAudio);
            // Don't wait here - let the main thread handle the cleanup
            // This prevents the race condition between signal handler and main thread
        }
        // Don't call exit here - let the main thread exit naturally
        // This allows proper cleanup of resources
    }
    return TRUE;
}
#endif

int main(int argc, char* argv[]) {
    // Set up signal handling
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGABRT, signalHandler);

#ifdef _WIN32
    SetConsoleCtrlHandler(consoleHandler, TRUE);
#endif
    
    logBinaryIdentity(argc > 0 ? argv[0] : nullptr);

    // Enable console (stdout) logging. The standalone app is launched by the
    // VS Code frontend, which reads backend stdout; plugins leave this off.
    lattice::Logger::getInstance().setConsoleLogging(true);

    // Create an instance of CabbageAudioApp
    appInstance = new CabbageAudioApp(argc, argv);

    // Scan audio devices and initialize Cabbage
    appInstance->scanAudioDevices();
    appInstance->initialiseCabbage();
    appInstance->initialiseStdioConnection();

    // Re-register signal handlers after Csound initialization, since Csound
    // installs its own signal handlers that override ours
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGABRT, signalHandler);

   
    // Keep the program running until termination is requested
    while (!terminateRequested)
    {
        appInstance->onIdle();

        // Re-register signal handlers if Csound was (re)initialized
        // (Csound installs its own handlers that override ours)
        if (appInstance->needsSignalHandlerReset.exchange(false))
        {
            std::signal(SIGTERM, signalHandler);
            std::signal(SIGINT, signalHandler);
            std::signal(SIGABRT, signalHandler);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Sleep to avoid busy-waiting
    }

    // Clean up
    delete appInstance;
    appInstance = nullptr;

    return 0;
}
