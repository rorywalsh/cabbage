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

#pragma once

#include "lattice/LatticeAraProcessor.h"
#include "Cabbage.h"
#include "CabbageARADataPool.h"
#include <set>
#include <shared_mutex>

#if LATTICE_HAS_ARA
class CabbageProcessor : public lattice::AraProcessor<CabbageProcessor>
#else
class CabbageProcessor : public lattice::Processor
#endif
{

  public:
    CabbageProcessor(std::string csdFile = "", std::string config = "");

    // Destructor to clean up resources
    ~CabbageProcessor();

    // Csound API functions for deailing with midi input
    static int OpenMidiInputDevice(CSOUND *csnd, void **userData, const char *devName);
    static int OpenMidiOutputDevice(CSOUND *csnd, void **userData, const char *devName);
    static int ReadMidiData(CSOUND *csound, void *userData, unsigned char *mbuf, int nbytes);
    static int WriteMidiData(CSOUND *csound, void *userData, const unsigned char *mbuf, int nbytes);

    // Add parameters based on automatable widgets
    void addParameters();

    // Set up channel config
    void addChannels(const std::string &config = "");

    // Process method to handle audio processing
    void process(float **inputs, float **outputs, std::size_t blockSize) override;

    // Set a parameter value
    void setParameter(int paramId, double value) override;

    // Called whenever the webview sends a message
    void onMessageFromWebView(const nlohmann::json &j) override;

    // Called whenever the plugin webview is ready
    void onWebViewIsReady() override;

    // Called whenever the UI iwdgets need updating
    void updateUI();

    // Called at least once before the processing starts
    void prepareToPlay(double sampleRate, uint32_t minFrameCount, uint32_t maxFrameCount) override;

    int getSampleRate() { return sampleRate; }

#ifdef CabbageApp
    // Update the actual channel counts based on available hardware (for standalone mode only).
    // This allows the processor to use fewer channels than configured if the audio device
    // doesn't support the requested number of channels.
    void setActualChannelCounts(int inputs, int outputs);
#endif

    // Triggered by CabbageApp whenever websocket connection is established
    void setCabbageIsReady();

    // Scans populate configs on all engine widgets and refreshes them via
    // runPopulateAsync. Single trigger shared by the plugin updateUI() path
    // and the CabbageApp (re)compile path, so both behave identically.
    void triggerInitialPopulate();

    // Plugin load/save state
    void loadPluginState(nlohmann::json state) override;
    nlohmann::json savePluginState() override;

    cabbage::Engine &getCabbageEngine() { return cabbage; }

#ifdef CabbageApp
    std::function<void(CabbageOpcodeData)> hostCallback = nullptr;
    // Invokes hostCallback guarded: a corrupt payload (e.g. non-UTF8 bytes
    // that make json::dump throw) is logged with its channel instead of
    // propagating through the idle thread and killing the process.
    void invokeHostCallback(const CabbageOpcodeData &data);
    // Logs the channel plus a hex preview of any non-UTF8 strings in a
    // payload that failed to serialize, so CI pinpoints which extraction
    // produced the bad bytes.
    static void logCorruptOpcodePayload(const CabbageOpcodeData &data);
#endif

    // Process opcode data and return the updated widget JSON if applicable
    std::optional<nlohmann::json> processOpcodeData(const CabbageOpcodeData &data);

    // Adds a MIDI note event to the midi event queue
    void addNoteEventFromJson(const nlohmann::json &j);
    void suspendProcessing() { processingEnabled.store(false); }
    void stopIdleThread();
    bool isIdleThreadRunning() { return isIdleRunning.load(std::memory_order_acquire); }
    void startOnIdle();  // Public for CabbageApp initialization

#if LATTICE_HAS_ARA || defined(CabbageApp)
#if LATTICE_HAS_ARA
    static lattice::AraPluginInfo getStaticAraInfo() noexcept;
    void araAudioSourceContentUpdated(ARA::PlugIn::AudioSource* source, ARA::ContentUpdateScopes scopes) override;
    void araDidEnableSamplesAccess(ARA::PlugIn::AudioSource* source, bool enable) override;
    void araPlaybackRegionPropertiesUpdated(ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araBeginEditing() override;
    void araEndEditing() override;
    void araDidNotifyModelUpdates() override;
    void araDocumentPropertiesUpdated(ARA::PlugIn::Document* document) override;
    void araMusicalContextPropertiesUpdated(ARA::PlugIn::MusicalContext* musicalContext) override;
    void araRegionSequencePropertiesUpdated(ARA::PlugIn::RegionSequence* regionSequence) override;
    void araAudioSourcePropertiesUpdated(ARA::PlugIn::AudioSource* audioSource) override;
    void araAudioModificationPropertiesUpdated(ARA::PlugIn::AudioModification* audioModification) override;
    void araPlaybackRegionAddedToRegionSequence(ARA::PlugIn::RegionSequence* regionSequence,
                                                 ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araPlaybackRegionRemovedFromRegionSequence(ARA::PlugIn::RegionSequence* regionSequence,
                                                      ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araMusicalContextAddedToDocument(ARA::PlugIn::Document* document,
                                           ARA::PlugIn::MusicalContext* musicalContext) override;
    void araMusicalContextRemovedFromDocument(ARA::PlugIn::Document* document,
                                               ARA::PlugIn::MusicalContext* musicalContext) override;
    void araRegionSequenceAddedToDocument(ARA::PlugIn::Document* document,
                                           ARA::PlugIn::RegionSequence* regionSequence) override;
    void araRegionSequenceRemovedFromDocument(ARA::PlugIn::Document* document,
                                               ARA::PlugIn::RegionSequence* regionSequence) override;
    void araAudioSourceAddedToDocument(ARA::PlugIn::Document* document,
                                        ARA::PlugIn::AudioSource* audioSource) override;
    void araAudioSourceRemovedFromDocument(ARA::PlugIn::Document* document,
                                            ARA::PlugIn::AudioSource* audioSource) override;
    void araDocumentWillDestroy(ARA::PlugIn::Document* document) override;
    void araDocumentPropertiesWillUpdate(ARA::PlugIn::Document* document) override;
    void araMusicalContextPropertiesWillUpdate(ARA::PlugIn::MusicalContext* musicalContext) override;
    void araRegionSequenceAddedToMusicalContext(ARA::PlugIn::MusicalContext* musicalContext,
                                                 ARA::PlugIn::RegionSequence* regionSequence) override;
    void araRegionSequenceRemovedFromMusicalContext(ARA::PlugIn::MusicalContext* musicalContext,
                                                     ARA::PlugIn::RegionSequence* regionSequence) override;
    void araMusicalContextWillDestroy(ARA::PlugIn::MusicalContext* musicalContext) override;
    void araRegionSequencePropertiesWillUpdate(ARA::PlugIn::RegionSequence* regionSequence) override;
    void araRegionSequenceWillDestroy(ARA::PlugIn::RegionSequence* regionSequence) override;
    void araAudioSourcePropertiesWillUpdate(ARA::PlugIn::AudioSource* audioSource) override;
    void araAudioSourceDeactivatedForUndo(ARA::PlugIn::AudioSource* audioSource, bool deactivate) override;
    void araAudioSourceReactivatedFromUndo(ARA::PlugIn::AudioSource* audioSource, bool deactivate) override;
    void araAudioModificationAddedToAudioSource(ARA::PlugIn::AudioSource* audioSource,
                                                 ARA::PlugIn::AudioModification* audioModification) override;
    void araAudioModificationRemovedFromAudioSource(ARA::PlugIn::AudioSource* audioSource,
                                                     ARA::PlugIn::AudioModification* audioModification) override;
    void araAudioSourceWillDestroy(ARA::PlugIn::AudioSource* audioSource) override;
    void araAudioModificationPropertiesWillUpdate(ARA::PlugIn::AudioModification* audioModification) override;
    void araAudioModificationDeactivatedForUndo(ARA::PlugIn::AudioModification* audioModification,
                                                 bool deactivate) override;
    void araAudioModificationReactivatedFromUndo(ARA::PlugIn::AudioModification* audioModification,
                                                  bool deactivate) override;
    void araPlaybackRegionAddedToAudioModification(ARA::PlugIn::AudioModification* audioModification,
                                                    ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araPlaybackRegionRemovedFromAudioModification(ARA::PlugIn::AudioModification* audioModification,
                                                        ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araAudioModificationWillDestroy(ARA::PlugIn::AudioModification* audioModification) override;
    void araPlaybackRegionPropertiesWillUpdate(ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araPlaybackRegionWillDestroy(ARA::PlugIn::PlaybackRegion* playbackRegion) override;
    void araNotifySelection(const ARA::PlugIn::ViewSelection* selection) override;
    void araNotifyHideRegionSequences(
        const std::vector<ARA::PlugIn::RegionSequence*>& hiddenSequences) override;
#endif

    // Public accessors for ARA opcodes
    int getAraCurrentSourceIndex() const { return araCurrentSourceIndex; }
    int getAraUpdateCounter() const { return araUpdateCounter; }
    std::string getAraLastEventType();
#endif // LATTICE_HAS_ARA || CabbageApp

private:
#if LATTICE_HAS_ARA
    void startAraWorker();
    void stopAraWorker();
    void runAraWorker();
    void performAraAnalysis(ARA::PlugIn::AudioSource* source);
    void enqueueAraSource(ARA::PlugIn::AudioSource* source);
    std::condition_variable araCv;
    std::deque<ARA::PlugIn::AudioSource*> araPendingSources;
    std::set<ARA::PlugIn::AudioSource*>  araAccessibleSources; // sources with sample access enabled
    std::set<ARA::PlugIn::AudioSource*>  araAnalysedSources;   // sources already enqueued for analysis
    std::atomic<bool>       araWorkerRunning{false};
    std::thread             araWorkerThread;
#endif
#if LATTICE_HAS_ARA || defined(CabbageApp)
    int araCurrentSourceIndex = -1;  // pool index of this instance's own source
    int araUpdateCounter = 0;        // incremented each analysis
    std::string araLastEventType;    // most recent ARA lifecycle event name
    std::mutex araMutex;
#ifdef CabbageApp
    void performAraAnalysisFromFile(const std::string& filePath);
    std::thread araTestThread;
#endif
#endif // LATTICE_HAS_ARA || CabbageApp
    void onIdle();
    void onIdleScheduler();
    // Runs a populate directory scan for a widget channel and delivers the
    // resulting items through the opcode queue (populate re-triggering
    // suppressed). Internal worker behind triggerInitialPopulate() and the
    // runtime cabbageSet refresh path. Thread-safe to call: the scan runs on
    // a worker thread, state updates + UI delivery via the queue.
    void runPopulateAsync(const std::string &widgetChannel, nlohmann::json populateConfig);
    void stopOnIdle();
    void initialiseAudioEngine();
    void initialiseAraCompanion();
    void updateWidgetData(const CabbageOpcodeData &data);
    void addParametersForWidget(nlohmann::json &w);
    void configureLogger(const nlohmann::json& json);
    void openFileDialog(const std::string &channel, const std::string &directory, const std::string &filters,
                        bool openAtLastKnownLocation);
    std::string generateErrorPageHtml(const std::string &errors);
    std::atomic<bool> processingEnabled{false};  // Start disabled, enabled in prepareToPlay()
    std::shared_mutex audioEngineMutex;           // Shared: process() readers; Exclusive: prepareToPlay() writer
    bool uiIsOpen = false;
    bool allowDequeuing = false;
    int sampleRate = 44100;
    std::string channelConfig;       // Channel config string stored for SR-change reinit
    std::string activeChannelConfig;  // Effective config last passed to addChannels(); used for change detection
    cabbage::Engine cabbage;
    bool matchingNumInputsOutputs = true;
    int csndIndex = 0;
    int pos = 0;
    int totalNumOutputs = 0;
    int totalNumInputs = 0;
    int actualNumInputs = 0;   // hardware input channels (0 = unset → use totalNumInputs)
    int actualNumOutputs = 0;  // hardware output channels (0 = unset → use totalNumOutputs)
    // Standalone mono-input handling: when the device has fewer input channels
    // than the instrument, "duplicate" (default) repeats the last channel into
    // the missing ones (mono → stereo); "zero" leaves the missing channels silent.
    bool zeroExtraInputChannels = false;
    std::atomic<bool> isIdleRunning;
    std::thread idleThread;
    int idleCounter = 0;
    std::vector<std::string> parameterChannelIds;  // Maps parameter index → channel ID for fast lookup during automation
    std::vector<lattice::Parameter> webviewMessageQueue;
    bool hasCompileErrors = false;
    std::string compileErrorHtml;
    std::string cabzTempDir;  // Temp directory for extracted .cabz archive (Pro builds only)

#if LATTICE_HAS_ARA
    std::mutex  araSourcesMutex; // protects araAccessibleSources
#endif

    std::string errorPageHtml =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\" />\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
        "  <title>Bouncing Cabbage</title>\n"
        "  <style>\n"
        "    body {\n"
        "      margin: 0;\n"
        "      height: 100vh;\n"
        "      display: flex;\n"
        "      justify-content: center;\n"
        "      align-items: center;\n"
        "      background-color: #3CB1F2;\n"
        "    }\n"
        "\n"
        "    svg {\n"
        "      width: 200px;\n"
        "      height: auto;\n"
        "      animation: bounce 1.0s ease-in-out infinite;\n"
        "    }\n"
        "\n"
        "    @keyframes bounce {\n"
        "      0%, 100% {\n"
        "        transform: translateY(0);\n"
        "      }\n"
        "      30% {\n"
        "        transform: translateY(-40px);\n"
        "      }\n"
        "      50% {\n"
        "        transform: translateY(0);\n"
        "      }\n"
        "      70% {\n"
        "        transform: translateY(-15px);\n"
        "      }\n"
        "      85% {\n"
        "        transform: translateY(0);\n"
        "      }\n"
        "    }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "\n"
        "  <!-- Your SVG starts here -->\n"
        "  <?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        "  <svg\n"
        "     xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
        "     xmlns:cc=\"http://creativecommons.org/ns#\"\n"
        "     xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
        "     xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
        "     xmlns=\"http://www.w3.org/2000/svg\"\n"
        "     width=\"420\"\n"
        "     height=\"400\"\n"
        "     viewBox=\"0 0 335.99977 319.99982\"\n"
        "     id=\"svg2\"\n"
        "     version=\"1.1\">\n"
        "    <path\n"
        "       id=\"Selection #1\"\n"
        "       d=\"m 200.37937,24.833718 c 53.94784,10.344682 89.98007,47.887315 109.69256,95.327092 0,0 "
        "10.52672,30.68879 10.52672,30.68879 0,0 8.0459,11.5083 8.0459,11.5083 5.72603,9.28336 5.95399,13.91225 "
        "5.8333,24.29531 -0.28159,21.82741 -18.8006,37.91344 -41.57053,37.03114 -33.29664,-1.27869 -48.23521,-39.49905 "
        "-29.78326,-63.88384 8.78345,-11.6234 20.81209,-15.30604 35.14719,-15.34441 0,0 -12.48456,-33.2462 "
        "-12.48456,-33.2462 -13.67803,-28.476641 -34.36941,-49.869297 -62.61057,-65.725176 -13.54393,-7.59548 "
        "-30.2258,-12.671921 -45.59347,-15.600142 -7.22791,-1.380996 -18.63967,-0.792793 -22.79674,-6.904979 "
        "18.07648,-1.393782 27.42314,-1.636736 45.59346,1.854115 z M 62.298194,72.848908 c -7.817932,7.915154 "
        "-14.563087,17.10901 -19.658833,26.852704 -6.543998,12.505678 -12.2566,32.082578 -13.678038,46.033218 0,0 "
        "-1.327575,17.90177 -1.327575,17.90177 -0.134107,4.55217 1.340982,6.96892 0,11.50832 -9.158925,-21.37987 "
        "-12.632076,-41.76235 0,-62.65631 5.726008,-8.61844 9.909878,-12.23717 17.191422,-19.180502 -8.059312,2.57018 "
        "-13.289165,4.296424 -20.061124,9.615822 C 6.5534767,117.2198 1.2297598,144.48168 9.7316079,164.91533 "
        "16.543811,181.34659 24.643345,182.10103 30.074333,193.0467 0.0899247,176.34686 -4.9119466,132.46189 "
        "16.020829,107.3738 c 4.210686,-5.05087 9.400295,-9.002037 15.394497,-11.98141 4.130239,-2.058713 "
        "9.42712,-3.094458 12.565028,-6.303996 0,0 10.553539,-14.960785 10.553539,-14.960785 C 63.451439,62.990124 "
        "65.597026,62.491431 75.667815,53.668402 75.305748,60.816334 67.206202,67.887544 62.298194,72.848908 z M "
        "319.67338,235.09037 c -24.70095,19.39787 -61.69871,14.51325 -80.49931,-10.07617 -10.80833,-14.11685 "
        "-13.90599,-33.02882 -9.9501,-49.86928 4.10341,-17.4415 15.52862,-33.46357 32.84071,-41.04629 4.46548,-1.9564 "
        "18.96153,-6.90497 23.14539,-3.72101 2.0517,1.56001 2.0517,4.16856 2.33332,6.40629 -7.79114,0.66493 "
        "-13.51713,2.6597 -20.11477,6.71317 -14.61674,8.96368 -25.37143,24.97302 -25.37143,41.87744 0,25.56119 "
        "26.22965,51.51881 53.53212,49.63913 20.15498,-1.3938 25.35799,-8.8486 40.22951,-18.95034 -4.46547,7.91515 "
        "-8.81026,13.27291 -16.14544,19.02706 z m 17.48645,-21.58446 c 0,0 -1.34101,1.27869 -1.34101,1.27869 0,0 "
        "0,-1.27869 0,-1.27869 0,0 1.34101,0 1.34101,0 z\"\n"
        "       style=\"fill:#ffffff;stroke:#ffffff;stroke-width:1.30947196\" />\n"
        "    <path\n"
        "       d=\"m 187.74022,296.72683 c 0,0 -2.87883,0 -2.87883,0 0,0 -7.19709,0.45789 -7.19709,0.45789 0,0 "
        "-3.83843,-0.41496 -3.83843,-0.41496 -28.29415,-1.93169 -59.30398,-10.50266 -82.52658,-27.11041 "
        "-32.23334,-23.04193 -48.69547,-58.56592 -50.39878,-97.41909 0,0 -0.4606,-5.24655 -0.4606,-5.24655 "
        "-0.17274,-14.84777 2.16391,-34.07403 6.88519,-48.17297 4.33745,-12.93993 10.08552,-24.916385 "
        "18.49172,-35.771995 8.29584,-10.717298 14.76361,-14.313574 21.52409,-27.663685 0,0 14.12546,-27.186724 "
        "14.12546,-27.186724 1.08917,-1.774291 4.48618,-8.384958 5.82484,-9.191019 1.36266,-0.825141 2.61976,0.300485 "
        "3.68012,1.092238 1.70811,1.268714 4.04955,3.391187 6.23748,3.596279 2.52858,0.233711 4.37582,-1.779061 "
        "5.73847,-3.605817 0.83485,-1.111319 1.70811,-2.914226 3.39702,-2.303718 1.06037,0.381568 2.9652,2.408649 "
        "3.81926,3.257637 2.57175,2.551737 8.21907,8.971618 11.51533,9.744293 2.61494,0.61528 4.90841,-1.006385 "
        "6.71248,-1.321179 2.69171,-0.46742 4.48138,2.589893 5.82964,4.454807 0,0 5.89682,8.585281 5.89682,8.585281 "
        "0,0 6.34782,8.585278 6.34782,8.585278 0,0 9.43778,14.308804 9.43778,14.308804 8.58851,12.806378 "
        "16.42374,23.566595 20.74679,38.63375 2.58135,8.99547 2.7109,15.14349 2.60534,24.32497 -0.0624,5.04624 "
        "-2.01519,15.36765 -3.15712,20.50928 -3.59855,16.20235 -9.09712,32.00879 -10.32063,48.64994 0,0 "
        "-0.43661,4.76958 -0.43661,4.76958 0,0 0,6.67745 0,6.67745 0.0288,17.99092 8.46856,39.35874 18.08387,54.37343 "
        "0,0 12.56611,17.64753 12.56611,17.64753 0,0 6.77485,8.58529 6.77485,8.58529 -7.92639,3.45795 "
        "-26.32693,7.04947 -35.02581,7.15439 z\"\n"
        "       style=\"fill:#93d200;fill-opacity:1;stroke:#000000;stroke-width:0.47838068;stroke-opacity:0\" />\n"
        "  </svg>\n"
        "  <!-- SVG ends here -->\n"
        "\n"
        "</body>\n"
        "</html>\n"
        "";

};
