# -*- makefile -*-
JUCE_TARGET := CabbagePlugin
JUCE_TARGET_LV2 := $(JUCE_TARGET).so
JUCE_OUTDIR_LV2 := $(JUCE_OUTDIR)/$(JUCE_TARGET).lv2
JUCE_CFLAGS_LV2 := $(JUCE_CFLAGS_VST)
JUCE_CPPFLAGS_LV2 := $(JUCE_CPPFLAGS_VST) -DJucePlugin_Build_LV2=1
JUCE_LDFLAGS_LV2 := $(JUCE_LDFLAGS_VST)

all : LV2
LV2 : $(JUCE_OUTDIR_LV2)/$(JUCE_TARGET_LV2)

OBJECTS_LV2 := \
  $(JUCE_OBJDIR)/juce_audio_plugin_client_LV2_.o

$(JUCE_OBJDIR)/juce_audio_plugin_client_LV2_.o: ../../JuceLibraryCode/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_plugin_client_LV2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_LV2) $(JUCE_CFLAGS_LV2) -o "$@" -c "$<"

$(JUCE_OUTDIR_LV2)/$(JUCE_TARGET_LV2) : check-pkg-config $(OBJECTS_LV2) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@echo Linking "$(JUCE_TARGET) - LV2"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR_LV2)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR_LV2)/$(JUCE_TARGET_LV2) $(OBJECTS_LV2) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_LV2) $(RESOURCES) $(TARGET_ARCH)
	../../LV2/generate-lv2-ttl.py $(JUCE_OUTDIR_LV2)/$(JUCE_TARGET_LV2)

-include $(OBJECTS_LV2:%.o=%.d)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama