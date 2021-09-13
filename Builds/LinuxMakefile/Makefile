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
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DDUMMYDEF=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=2.8.4" "-DJUCE_APP_VERSION_HEX=0x20804" $(shell pkg-config --cflags alsa freetype2 libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules -I"/usr/local/include/csound" -I"/usr/include/csound" -I"$(HOME)/SDKs/VST_SDK/VST3_SDK" $(CPPFLAGS)

  JUCE_CPPFLAGS_VST :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := CabbagePlugin.so
  JUCE_VSTDESTDIR := $(HOME)/.vst
  JUCE_COPYCMD_VST := $(JUCE_OUTDIR)/CabbagePlugin.so $(JUCE_VSTDESTDIR)

  JUCE_CPPFLAGS_VST3 :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := CabbagePlugin.vst3
  VST3_PLATFORM_ARCH := $(shell $(CXX) make_helpers/arch_detection.cpp 2>&1 | tr '\n' ' ' | sed "s/.*JUCE_ARCH \([a-zA-Z0-9_-]*\).*/\1/")
  JUCE_VST3SUBDIR := Contents/$(VST3_PLATFORM_ARCH)-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/CabbagePlugin.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_STANDALONE_PLUGIN :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_STANDALONE_PLUGIN := CabbagePlugin

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0" "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := CabbagePlugin.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L"/usr/local/lib" $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wall -lrt -ldl -lpthread -lGL -lcsound64 -lsndfile $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DDUMMYDEF=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=2.8.4" "-DJUCE_APP_VERSION_HEX=0x20804" $(shell pkg-config --cflags alsa freetype2 libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules -I"/usr/local/include/csound" -I"/usr/include/csound" -I"$(HOME)/SDKs/VST_SDK/VST3_SDK" $(CPPFLAGS)

  JUCE_CPPFLAGS_VST :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := CabbagePlugin.so
  JUCE_VSTDESTDIR := $(HOME)/.vst
  JUCE_COPYCMD_VST := $(JUCE_OUTDIR)/CabbagePlugin.so $(JUCE_VSTDESTDIR)

  JUCE_CPPFLAGS_VST3 :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := CabbagePlugin.vst3
  VST3_PLATFORM_ARCH := $(shell $(CXX) make_helpers/arch_detection.cpp 2>&1 | tr '\n' ' ' | sed "s/.*JUCE_ARCH \([a-zA-Z0-9_-]*\).*/\1/")
  JUCE_VST3SUBDIR := Contents/$(VST3_PLATFORM_ARCH)-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/CabbagePlugin.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_STANDALONE_PLUGIN :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_STANDALONE_PLUGIN := CabbagePlugin

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0" "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := CabbagePlugin.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L"/usr/local/lib" $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wall -lrt -ldl -lpthread -lGL -lcsound64 -lsndfile $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_ALL := \

OBJECTS_VST := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_dd551e08.o \

OBJECTS_VST3 := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_dd633589.o \

OBJECTS_STANDALONE_PLUGIN := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_1a871192.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/CabbageIdentifierOpcodes_427b34ee.o \
  $(JUCE_OBJDIR)/CabbagePluginEditor_4348213a.o \
  $(JUCE_OBJDIR)/CabbagePluginProcessor_8a6343af.o \
  $(JUCE_OBJDIR)/CsoundPluginEditor_3f0e809d.o \
  $(JUCE_OBJDIR)/CsoundPluginProcessor_dc39856c.o \
  $(JUCE_OBJDIR)/GenericCabbageEditor_50c74848.o \
  $(JUCE_OBJDIR)/GenericCabbagePluginProcessor_7befc9ee.o \
  $(JUCE_OBJDIR)/CabbageBinaryData_812f3f1.o \
  $(JUCE_OBJDIR)/CabbageGenericPluginLookAndFeel_f8a44125.o \
  $(JUCE_OBJDIR)/CabbageLookAndFeel2_1e182065.o \
  $(JUCE_OBJDIR)/FlatButtonLookAndFeel_fa1eb0e3.o \
  $(JUCE_OBJDIR)/PropertyPanelLookAndFeel_10c1c0e9.o \
  $(JUCE_OBJDIR)/CabbageColourProperty_a06c91be.o \
  $(JUCE_OBJDIR)/Soundfiler_587b384e.o \
  $(JUCE_OBJDIR)/TableManager_fa647506.o \
  $(JUCE_OBJDIR)/CabbageForm_67c813e6.o \
  $(JUCE_OBJDIR)/CabbageForm_67c813e6.o \
  $(JUCE_OBJDIR)/CabbagePath_951fd2c7.o \
  $(JUCE_OBJDIR)/CabbageOptionButton_e328a0a9.o \
  $(JUCE_OBJDIR)/CabbageRackWidgets_485e2780.o \
  $(JUCE_OBJDIR)/CabbageKeyboardDisplay_b54deb83.o \
  $(JUCE_OBJDIR)/CabbageListBox_921ed2f5.o \
  $(JUCE_OBJDIR)/CabbageWidgetDataTextMethods_86f5a93f.o \
  $(JUCE_OBJDIR)/CabbageButton_effd6014.o \
  $(JUCE_OBJDIR)/CabbageCheckbox_8b068ce5.o \
  $(JUCE_OBJDIR)/CabbageComboBox_ae0bd57f.o \
  $(JUCE_OBJDIR)/CabbageCsoundConsole_f188e293.o \
  $(JUCE_OBJDIR)/CabbageNumberSlider_436c5cac.o \
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
  $(JUCE_OBJDIR)/CabbageXYPad_f88302da.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_e32edaee.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \

.PHONY: clean all strip VST VST3 Standalone

all : VST VST3 Standalone

VST : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST)
VST3 : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3)
Standalone : $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN)


$(JUCE_OUTDIR)/$(JUCE_TARGET_VST) : $(OBJECTS_VST) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "CabbagePlugin - VST"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST) $(OBJECTS_VST) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST) $(RESOURCES) $(TARGET_ARCH)
	-$(V_AT)mkdir -p $(JUCE_VSTDESTDIR)
	-$(V_AT)cp -R $(JUCE_COPYCMD_VST)

$(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) : $(OBJECTS_VST3) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "CabbagePlugin - VST3"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)/$(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) $(OBJECTS_VST3) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST3) $(RESOURCES) $(TARGET_ARCH)
	-$(V_AT)mkdir -p $(JUCE_VST3DESTDIR)
	-$(V_AT)cp -R $(JUCE_COPYCMD_VST3)

$(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) : $(OBJECTS_STANDALONE_PLUGIN) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "CabbagePlugin - Standalone Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) $(OBJECTS_STANDALONE_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_STANDALONE_PLUGIN) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "CabbagePlugin - Shared Code"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(AR) -rcs $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(OBJECTS_SHARED_CODE)

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_dd551e08.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST) $(JUCE_CFLAGS_VST) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_dd633589.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST3) $(JUCE_CFLAGS_VST3) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_1a871192.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_Standalone.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_STANDALONE_PLUGIN) $(JUCE_CFLAGS_STANDALONE_PLUGIN) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageIdentifierOpcodes_427b34ee.o: ../../Source/Opcodes/CabbageIdentifierOpcodes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageIdentifierOpcodes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePluginEditor_4348213a.o: ../../Source/Audio/Plugins/CabbagePluginEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePluginProcessor_8a6343af.o: ../../Source/Audio/Plugins/CabbagePluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CsoundPluginEditor_3f0e809d.o: ../../Source/Audio/Plugins/CsoundPluginEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CsoundPluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CsoundPluginProcessor_dc39856c.o: ../../Source/Audio/Plugins/CsoundPluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CsoundPluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericCabbageEditor_50c74848.o: ../../Source/Audio/Plugins/GenericCabbageEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericCabbageEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericCabbagePluginProcessor_7befc9ee.o: ../../Source/Audio/Plugins/GenericCabbagePluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericCabbagePluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageBinaryData_812f3f1.o: ../../Source/BinaryData/CabbageBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGenericPluginLookAndFeel_f8a44125.o: ../../Source/LookAndFeel/CabbageGenericPluginLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGenericPluginLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageLookAndFeel2_1e182065.o: ../../Source/LookAndFeel/CabbageLookAndFeel2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageLookAndFeel2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FlatButtonLookAndFeel_fa1eb0e3.o: ../../Source/LookAndFeel/FlatButtonLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FlatButtonLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PropertyPanelLookAndFeel_10c1c0e9.o: ../../Source/LookAndFeel/PropertyPanelLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PropertyPanelLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageColourProperty_a06c91be.o: ../../Source/Utilities/CabbageColourProperty.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageColourProperty.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Soundfiler_587b384e.o: ../../Source/Widgets/Legacy/Soundfiler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Soundfiler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TableManager_fa647506.o: ../../Source/Widgets/Legacy/TableManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TableManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageForm_67c813e6.o: ../../Source/Widgets/CabbageForm.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageForm.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageForm_67c813e6.o: ../../Source/Widgets/CabbageForm.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageForm.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbagePath_951fd2c7.o: ../../Source/Widgets/CabbagePath.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbagePath.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageOptionButton_e328a0a9.o: ../../Source/Widgets/CabbageOptionButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageOptionButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageRackWidgets_485e2780.o: ../../Source/Widgets/CabbageRackWidgets.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageRackWidgets.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageKeyboardDisplay_b54deb83.o: ../../Source/Widgets/CabbageKeyboardDisplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageKeyboardDisplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageListBox_921ed2f5.o: ../../Source/Widgets/CabbageListBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageListBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetDataTextMethods_86f5a93f.o: ../../Source/Widgets/CabbageWidgetDataTextMethods.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetDataTextMethods.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageButton_effd6014.o: ../../Source/Widgets/CabbageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCheckbox_8b068ce5.o: ../../Source/Widgets/CabbageCheckbox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCheckbox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageComboBox_ae0bd57f.o: ../../Source/Widgets/CabbageComboBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageComboBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCsoundConsole_f188e293.o: ../../Source/Widgets/CabbageCsoundConsole.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCsoundConsole.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageNumberSlider_436c5cac.o: ../../Source/Widgets/CabbageNumberSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageNumberSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageCustomWidgets_aa56f1e6.o: ../../Source/Widgets/CabbageCustomWidgets.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageCustomWidgets.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageEncoder_eecf3044.o: ../../Source/Widgets/CabbageEncoder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageEncoder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageFileButton_5bde2830.o: ../../Source/Widgets/CabbageFileButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageFileButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGenTable_bcbb7a00.o: ../../Source/Widgets/CabbageGenTable.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGenTable.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageGroupBox_dd08cf6e.o: ../../Source/Widgets/CabbageGroupBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageGroupBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageImage_f3ad1dc3.o: ../../Source/Widgets/CabbageImage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageImage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageInfoButton_4ccbc0e2.o: ../../Source/Widgets/CabbageInfoButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageInfoButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageKeyboard_fa013309.o: ../../Source/Widgets/CabbageKeyboard.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageKeyboard.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageLabel_3cb7edc.o: ../../Source/Widgets/CabbageLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageRangeSlider_b2c3b526.o: ../../Source/Widgets/CabbageRangeSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageRangeSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSignalDisplay_29ee9ee2.o: ../../Source/Widgets/CabbageSignalDisplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSignalDisplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSlider_1c8d4243.o: ../../Source/Widgets/CabbageSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageSoundfiler_7f844aa9.o: ../../Source/Widgets/CabbageSoundfiler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageSoundfiler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageEventSequencer_6bd39cb9.o: ../../Source/Widgets/CabbageEventSequencer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageEventSequencer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageTextBox_4d74cf86.o: ../../Source/Widgets/CabbageTextBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageTextBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageTextEditor_9f89b35c.o: ../../Source/Widgets/CabbageTextEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageTextEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetBase_e8de3417.o: ../../Source/Widgets/CabbageWidgetBase.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetBase.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetData_ba1d0870.o: ../../Source/Widgets/CabbageWidgetData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageWidgetDataInitMethods_a1a6cd7c.o: ../../Source/Widgets/CabbageWidgetDataInitMethods.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageWidgetDataInitMethods.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CabbageXYPad_f88302da.o: ../../Source/Widgets/CabbageXYPad.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CabbageXYPad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_e32edaee.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o: ../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

clean:
	@echo Cleaning CabbagePlugin
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping CabbagePlugin
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST:%.o=%.d)
-include $(OBJECTS_VST3:%.o=%.d)
-include $(OBJECTS_STANDALONE_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
