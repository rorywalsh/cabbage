# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DCabbage_IDE_Build=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 x11 xext xinerama libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules -I"/usr/local/include/csound" -I"/usr/include/csound" -I"$(HOME)/SDKs/VST_SDK/VST3_SDK" $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_APP := Cabbage

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L"/usr/local/lib" $(shell pkg-config --libs alsa freetype2 x11 xext xinerama libcurl) -ldl -lpthread -lrt -lcsound64 -lsndfile $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DCabbage_IDE_Build=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 x11 xext xinerama libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules -I"/usr/local/include/csound" -I"/usr/include/csound" -I"$(HOME)/SDKs/VST_SDK/VST3_SDK" $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_APP := Cabbage

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L"/usr/local/lib" $(shell pkg-config --libs alsa freetype2 x11 xext xinerama libcurl) -fvisibility=hidden -ldl -lpthread -lrt -lcsound64 -lsndfile $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/FileTab_96ff6bfb.o \
  $(JUCE_OBJDIR)/CabbageMainComponent_bb67f2ab.o \
  $(JUCE_OBJDIR)/CabbageDocumentWindow_3af539ae.o \
  $(JUCE_OBJDIR)/CabbageToolbarFactory_a8f7dbd2.o \
  $(JUCE_OBJDIR)/FilterGraph_7c0bb18a.o \
  $(JUCE_OBJDIR)/FilterIOConfiguration_2faeb40c.o \
  $(JUCE_OBJDIR)/InternalFilters_a9cb4bd2.o \
  $(JUCE_OBJDIR)/CabbageInternalPluginFormat_4ce4bea1.o \
  $(JUCE_OBJDIR)/CabbagePluginEditor_4348213a.o \
  $(JUCE_OBJDIR)/CabbagePluginProcessor_8a6343af.o \
  $(JUCE_OBJDIR)/CsoundPluginEditor_3f0e809d.o \
  $(JUCE_OBJDIR)/CsoundPluginProcessor_dc39856c.o \
  $(JUCE_OBJDIR)/GenericCabbageEditor_50c74848.o \
  $(JUCE_OBJDIR)/GenericCabbagePluginProcessor_7befc9ee.o \
  $(JUCE_OBJDIR)/GraphEditorPanel_279874cc.o \
  $(JUCE_OBJDIR)/CabbageBinaryData_812f3f1.o \
  $(JUCE_OBJDIR)/CabbageCodeEditor_72bb40f.o \
  $(JUCE_OBJDIR)/CabbageEditorContainer_522299a9.o \
  $(JUCE_OBJDIR)/JavascriptCodeTokeniser_f45b48a.o \
  $(JUCE_OBJDIR)/CabbagePropertiesPanel_cced81c0.o \
  $(JUCE_OBJDIR)/ComponentLayoutEditor_752a6c4e.o \
  $(JUCE_OBJDIR)/ComponentOverlay_554fade3.o \
  $(JUCE_OBJDIR)/CabbageGenericPluginLookAndFeel_f8a44125.o \
  $(JUCE_OBJDIR)/CabbageIDELookAndFeel_8b919965.o \
  $(JUCE_OBJDIR)/CabbageLookAndFeel2_1e182065.o \
  $(JUCE_OBJDIR)/FlatButtonLookAndFeel_fa1eb0e3.o \
  $(JUCE_OBJDIR)/PropertyPanelLookAndFeel_10c1c0e9.o \
  $(JUCE_OBJDIR)/CabbageSettings_d59022e1.o \
  $(JUCE_OBJDIR)/CabbageSettingsWindow_8ea691f1.o \
  $(JUCE_OBJDIR)/CabbagePluginList_2d332c6e.o \
  $(JUCE_OBJDIR)/CabbageExportPlugin_12abb504.o \
  $(JUCE_OBJDIR)/CabbageColourProperty_a06c91be.o \
  $(JUCE_OBJDIR)/CabbageNewProjectWindow_b55ea6.o \
  $(JUCE_OBJDIR)/CabbageSSHFileBrowser_34e09761.o \
  $(JUCE_OBJDIR)/CabbageKeyboardDisplay_b54deb83.o \
  $(JUCE_OBJDIR)/Soundfiler_587b384e.o \
  $(JUCE_OBJDIR)/TableManager_fa647506.o \
  $(JUCE_OBJDIR)/CabbageListBox_921ed2f5.o \
  $(JUCE_OBJDIR)/CabbageButton_effd6014.o \
  $(JUCE_OBJDIR)/CabbageNumberSlider_436c5cac.o \
  $(JUCE_OBJDIR)/CabbageCheckbox_8b068ce5.o \
  $(JUCE_OBJDIR)/CabbageComboBox_ae0bd57f.o \
  $(JUCE_OBJDIR)/CabbageCsoundConsole_f188e293.o \
  $(JUCE_OBJDIR)/CabbageCustomWidgets_aa56f1e6.o \
  $(JUCE_OBJDIR)/CabbageEncoder_eecf3044.o \
  $(JUCE_OBJDIR)/CabbageFileButton_5bde2830.o \
  $(JUCE_OBJDIR)/CabbageGenTable_bcbb7a00.o \
  $(JUCE_OBJDIR)/CabbageGroupBox_dd08cf6e.o \
  $(JUCE_OBJDIR)/CabbageImage_f3ad1dc3.o \
  $(JUCE_OBJDIR)/CabbageInfoButton_4ccbc0e2.o \
  $(JUCE_OBJDIR)/CabbageKeyboard_fa013309.o \
  $(JUCE_OBJDIR)/CabbageLabel_3cb7edc.o \
  $(JUCE_OBJDIR)/CabbageRangeSlider_b2c3b526.o \
  $(JUCE_OBJDIR)/CabbageSignalDisplay_29ee9ee2.o \
  $(JUCE_OBJDIR)/CabbageSlider_1c8d4243.o \
  $(JUCE_OBJDIR)/CabbageSoundfiler_7f844aa9.o \
  $(JUCE_OBJDIR)/CabbageEventSequencer_6bd39cb9.o \
  $(JUCE_OBJDIR)/CabbageTextBox_4d74cf86.o \
  $(JUCE_OBJDIR)/CabbageTextEditor_9f89b35c.o \
  $(JUCE_OBJDIR)/CabbageWidgetBase_e8de3417.o \
  $(JUCE_OBJDIR)/CabbageWidgetData_ba1d0870.o \
  $(JUCE_OBJDIR)/CabbageWidgetDataInitMethods_a1a6cd7c.o \
  $(JUCE_OBJDIR)/CabbageWidgetDataTextMethods_86f5a93f.o \
  $(JUCE_OBJDIR)/CabbageXYPad_f88302da.o \
  $(JUCE_OBJDIR)/Cabbage_6fb7db02.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \

.PHONY: clean all strip

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : $(OBJECTS_APP) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 x11 xext xinerama libcurl
	@echo Linking "Cabbage - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/FileTab_96ff6bfb.o: ../../Source/Application/FileTab.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FileTab.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageMainComponent_bb67f2ab.o: ../../Source/Application/CabbageMainComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageMainComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageDocumentWindow_3af539ae.o: ../../Source/Application/CabbageDocumentWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageDocumentWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageToolbarFactory_a8f7dbd2.o: ../../Source/Application/CabbageToolbarFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageToolbarFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FilterGraph_7c0bb18a.o: ../../Source/Audio/Filters/FilterGraph.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FilterGraph.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FilterIOConfiguration_2faeb40c.o: ../../Source/Audio/Filters/FilterIOConfiguration.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FilterIOConfiguration.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InternalFilters_a9cb4bd2.o: ../../Source/Audio/Filters/InternalFilters.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InternalFilters.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageInternalPluginFormat_4ce4bea1.o: ../../Source/Audio/Plugins/CabbageInternalPluginFormat.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageInternalPluginFormat.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePluginEditor_4348213a.o: ../../Source/Audio/Plugins/CabbagePluginEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePluginProcessor_8a6343af.o: ../../Source/Audio/Plugins/CabbagePluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CsoundPluginEditor_3f0e809d.o: ../../Source/Audio/Plugins/CsoundPluginEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CsoundPluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CsoundPluginProcessor_dc39856c.o: ../../Source/Audio/Plugins/CsoundPluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CsoundPluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericCabbageEditor_50c74848.o: ../../Source/Audio/Plugins/GenericCabbageEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericCabbageEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericCabbagePluginProcessor_7befc9ee.o: ../../Source/Audio/Plugins/GenericCabbagePluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericCabbagePluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GraphEditorPanel_279874cc.o: ../../Source/Audio/UI/GraphEditorPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GraphEditorPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageBinaryData_812f3f1.o: ../../Source/BinaryData/CabbageBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCodeEditor_72bb40f.o: ../../Source/CodeEditor/CabbageCodeEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCodeEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageEditorContainer_522299a9.o: ../../Source/CodeEditor/CabbageEditorContainer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageEditorContainer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JavascriptCodeTokeniser_f45b48a.o: ../../Source/CodeEditor/JavascriptCodeTokeniser.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JavascriptCodeTokeniser.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePropertiesPanel_cced81c0.o: ../../Source/GUIEditor/CabbagePropertiesPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePropertiesPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentLayoutEditor_752a6c4e.o: ../../Source/GUIEditor/ComponentLayoutEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentLayoutEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentOverlay_554fade3.o: ../../Source/GUIEditor/ComponentOverlay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentOverlay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGenericPluginLookAndFeel_f8a44125.o: ../../Source/LookAndFeel/CabbageGenericPluginLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGenericPluginLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageIDELookAndFeel_8b919965.o: ../../Source/LookAndFeel/CabbageIDELookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageIDELookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageLookAndFeel2_1e182065.o: ../../Source/LookAndFeel/CabbageLookAndFeel2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageLookAndFeel2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FlatButtonLookAndFeel_fa1eb0e3.o: ../../Source/LookAndFeel/FlatButtonLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FlatButtonLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PropertyPanelLookAndFeel_10c1c0e9.o: ../../Source/LookAndFeel/PropertyPanelLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PropertyPanelLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSettings_d59022e1.o: ../../Source/Settings/CabbageSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSettingsWindow_8ea691f1.o: ../../Source/Settings/CabbageSettingsWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSettingsWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePluginList_2d332c6e.o: ../../Source/Utilities/CabbagePluginList.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePluginList.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageExportPlugin_12abb504.o: ../../Source/Utilities/CabbageExportPlugin.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageExportPlugin.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageColourProperty_a06c91be.o: ../../Source/Utilities/CabbageColourProperty.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageColourProperty.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageNewProjectWindow_b55ea6.o: ../../Source/Utilities/CabbageNewProjectWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageNewProjectWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSSHFileBrowser_34e09761.o: ../../Source/Utilities/CabbageSSHFileBrowser.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSSHFileBrowser.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageKeyboardDisplay_b54deb83.o: ../../Source/Widgets/CabbageKeyboardDisplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageKeyboardDisplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Soundfiler_587b384e.o: ../../Source/Widgets/Legacy/Soundfiler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Soundfiler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TableManager_fa647506.o: ../../Source/Widgets/Legacy/TableManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TableManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageListBox_921ed2f5.o: ../../Source/Widgets/CabbageListBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageListBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageButton_effd6014.o: ../../Source/Widgets/CabbageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageNumberSlider_436c5cac.o: ../../Source/Widgets/CabbageNumberSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageNumberSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCheckbox_8b068ce5.o: ../../Source/Widgets/CabbageCheckbox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCheckbox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageComboBox_ae0bd57f.o: ../../Source/Widgets/CabbageComboBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageComboBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCsoundConsole_f188e293.o: ../../Source/Widgets/CabbageCsoundConsole.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCsoundConsole.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCustomWidgets_aa56f1e6.o: ../../Source/Widgets/CabbageCustomWidgets.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCustomWidgets.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageEncoder_eecf3044.o: ../../Source/Widgets/CabbageEncoder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageEncoder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageFileButton_5bde2830.o: ../../Source/Widgets/CabbageFileButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageFileButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGenTable_bcbb7a00.o: ../../Source/Widgets/CabbageGenTable.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGenTable.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGroupBox_dd08cf6e.o: ../../Source/Widgets/CabbageGroupBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGroupBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageImage_f3ad1dc3.o: ../../Source/Widgets/CabbageImage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageImage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageInfoButton_4ccbc0e2.o: ../../Source/Widgets/CabbageInfoButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageInfoButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageKeyboard_fa013309.o: ../../Source/Widgets/CabbageKeyboard.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageKeyboard.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageLabel_3cb7edc.o: ../../Source/Widgets/CabbageLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageRangeSlider_b2c3b526.o: ../../Source/Widgets/CabbageRangeSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageRangeSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSignalDisplay_29ee9ee2.o: ../../Source/Widgets/CabbageSignalDisplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSignalDisplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSlider_1c8d4243.o: ../../Source/Widgets/CabbageSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSoundfiler_7f844aa9.o: ../../Source/Widgets/CabbageSoundfiler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSoundfiler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageEventSequencer_6bd39cb9.o: ../../Source/Widgets/CabbageEventSequencer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageEventSequencer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageTextBox_4d74cf86.o: ../../Source/Widgets/CabbageTextBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageTextBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageTextEditor_9f89b35c.o: ../../Source/Widgets/CabbageTextEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageTextEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetBase_e8de3417.o: ../../Source/Widgets/CabbageWidgetBase.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetBase.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetData_ba1d0870.o: ../../Source/Widgets/CabbageWidgetData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetDataInitMethods_a1a6cd7c.o: ../../Source/Widgets/CabbageWidgetDataInitMethods.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetDataInitMethods.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetDataTextMethods_86f5a93f.o: ../../Source/Widgets/CabbageWidgetDataTextMethods.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetDataTextMethods.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageXYPad_f88302da.o: ../../Source/Widgets/CabbageXYPad.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageXYPad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Cabbage_6fb7db02.o: ../../Source/Cabbage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Cabbage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

clean:
	@echo Cleaning Cabbage
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Cabbage
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
