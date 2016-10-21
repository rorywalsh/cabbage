/*
  ==============================================================================

    CabbagePluginWrapper.cpp
    Created: 21 Oct 2016 7:09:19pm
    Author:  rory

  ==============================================================================
*/

#include "CabbagePluginWrapper.h"

CabbagePluginWrapper::CabbagePluginWrapper(ValueTree settingsToUse,
						bool takeOwnershipOfSettings,
						const String& preferredDefaultDeviceName,
						const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)

	: settings (settingsToUse)
{
	createPlugin(File());
}

CabbagePluginWrapper::~CabbagePluginWrapper()
{
   #if JUCE_IOS || JUCE_ANDROID
	stopTimer();
   #endif

	deletePlugin();
	shutDownAudioDevices();
}

//==============================================================================
void CabbagePluginWrapper::createPlugin(File inputFile)
{

	AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
	processor = createCsoundPluginFilter(inputFile);			
	AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);
			
	jassert (processor != nullptr); // Your createPluginFilter() function must return a valid object!

	processor->disableNonMainBuses();
	processor->setRateAndBufferSizeDetails(44100, 512);
}

String CabbagePluginWrapper::getCsoundOutput()
{
	return processor->getCsoundOutput();
}

void CabbagePluginWrapper::deletePlugin()
{
	stopPlaying();
	processor->editorBeingDeleted(processor->getActiveEditor());
	processor = nullptr;
}

//==============================================================================
void CabbagePluginWrapper::startPlaying()
{
	player.setProcessor (processor);
}

void CabbagePluginWrapper::stopPlaying()
{
	player.setProcessor (nullptr);
}

//==============================================================================

AudioDeviceSelectorComponent* CabbagePluginWrapper::getAudioDeviceSelector()
{
	return new AudioDeviceSelectorComponent (deviceManager,
														  processor->getTotalNumInputChannels(),
														  processor->getTotalNumInputChannels(),
														  processor->getTotalNumOutputChannels(),
														  processor->getTotalNumOutputChannels(),
														  true, false,
														  true, false);
}


void CabbagePluginWrapper::setXmlAudioSettings(XmlElement* xmlSettingsString)
{
	xmlSettings = xmlSettingsString;
	setupAudioDevices ( String(), nullptr);
	startPlaying();
}

void CabbagePluginWrapper::reloadAudioDeviceState (const String& preferredDefaultDeviceName,
							 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
{
	ScopedPointer<XmlElement> savedState;
	
	//String xmlAudioState = CabbageSettings::get(settings, "AudioSettings", "audioSetup");

	

	savedState = xmlSettings;

	deviceManager.initialise (processor->getTotalNumInputChannels(),
							  processor->getTotalNumOutputChannels(),
							  savedState,
							  true,
							  preferredDefaultDeviceName,
							  preferredSetupOptions);
}

void CabbagePluginWrapper::setupAudioDevices (const String& preferredDefaultDeviceName,
						const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
{
	deviceManager.addAudioCallback (&player);
	deviceManager.addMidiInputCallback (String(), &player);
	reloadAudioDeviceState (preferredDefaultDeviceName, preferredSetupOptions);
}

bool CabbagePluginWrapper::isAudioDeviceOk()
{
	return (deviceManager.getCurrentAudioDevice() ? true : false);
}

String CabbagePluginWrapper::getDeviceManagerSettings()
{
	if(deviceManager.getCurrentAudioDevice())
	{
		ScopedPointer<XmlElement> xml (deviceManager.createStateXml());
		return xml->createDocument("");
	}
	else return String::empty;
}

AudioProcessor* CabbagePluginWrapper::getProcessor()
{
	return processor;
}

void CabbagePluginWrapper::restartPlugin(File inputFile)
{
	stopPlaying();
	deletePlugin();
	createPlugin(inputFile);
	startPlaying();
}
