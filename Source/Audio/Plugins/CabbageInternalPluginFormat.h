/*
  ==============================================================================

    CabbageInternalPluginFormat.h
    Created: 27 Nov 2018 4:04:26pm
    Author:  rory

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


#include "CsoundPluginEditor.h"
#include "CsoundPluginProcessor.h"
#include "GenericCabbagePluginProcessor.h"
#include "CabbagePluginProcessor.h"
#include "CabbagePluginEditor.h"
//=================================================================================================
class InternalCabbagePluginFormat : public AudioPluginFormat
{
public:
	//==============================================================================
	InternalCabbagePluginFormat() {};
	~InternalCabbagePluginFormat() {}

	//==============================================================================
	PluginDescription cabbagePluginDesc;

	void getAllTypes(OwnedArray<PluginDescription>&) {};

	//==============================================================================
	String getName() const override { return "Cabbage"; }
	bool fileMightContainThisPluginType(const String&) override { return true; }
	FileSearchPath getDefaultLocationsToSearch() override { return {}; }
	bool canScanForPlugins() const override { return false; }
	void findAllTypesForFile(OwnedArray <PluginDescription>&, const String&) override {}
	bool doesPluginStillExist(const PluginDescription&) override { return true; }
	String getNameOfPluginFromIdentifier(const String& fileOrIdentifier) override { return fileOrIdentifier; }
	bool pluginNeedsRescanning(const PluginDescription&) override { return false; }
	StringArray searchPathsForPlugins(const FileSearchPath&, bool, bool) override { return {}; }
	

	static const PluginDescription getPluginDescriptor(String name, AudioProcessorGraph::NodeID nodeId, String inputFile)
	{
		PluginDescription descript;
		descript.fileOrIdentifier = inputFile;
		descript.descriptiveName = "Cabbage:" + inputFile;
		descript.name = name;
		descript.numInputChannels = 2;
		descript.numOutputChannels = 2;
		descript.isInstrument = true;
		descript.uid = nodeId.uid;

		descript.manufacturerName = "CabbageAudio";
		descript.pluginFormatName = "Cabbage";

		return descript;
	}

	GenericCabbagePluginProcessor* createGenericPluginFilter(File inputFile)
	{
		const int numChannels = CabbageUtilities::getHeaderInfo(inputFile.loadFileAsString(), "nchnls");
		return new GenericCabbagePluginProcessor(inputFile, numChannels, numChannels);
	}

	CabbagePluginProcessor* createCabbagePluginFilter(File inputFile)
	{
		const int numChannels = CabbageUtilities::getHeaderInfo(inputFile.loadFileAsString(), "nchnls");
		return new CabbagePluginProcessor(inputFile, numChannels, numChannels);
	}

private:
	//==============================================================================
	void createPluginInstance(const PluginDescription&, double initialSampleRate, int initialBufferSize,
		void* userData, PluginCreationCallback) override;
	AudioProcessor* createCabbagePlugin(const String file);

	bool requiresUnblockedMessageThreadDuringCreation(const PluginDescription&) const noexcept override;
};