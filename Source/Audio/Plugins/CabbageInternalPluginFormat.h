/*
  ==============================================================================

    CabbageInternalPluginFormat.h
    Created: 27 Nov 2018 4:04:26pm
    Author:  rory

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


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
		descript.uniqueId = nodeId.uid;

		descript.manufacturerName = "CabbageAudio";
		descript.pluginFormatName = "Cabbage";

		return descript;
	}

	std::unique_ptr < GenericCabbagePluginProcessor> createGenericPluginFilter(File inputFile)
	{
		
		return std::unique_ptr < GenericCabbagePluginProcessor>(new GenericCabbagePluginProcessor(inputFile, CabbagePluginProcessor::readBusesPropertiesFromXml(inputFile)));
	}

	std::unique_ptr<CabbagePluginProcessor> createCabbagePluginFilter(File inputFile)
	{
		
		return std::unique_ptr<CabbagePluginProcessor>(new CabbagePluginProcessor(inputFile, CabbagePluginProcessor::readBusesPropertiesFromXml(inputFile)));
	}

private:
	//==============================================================================
	void createPluginInstance(const PluginDescription&, double initialSampleRate, int initialBufferSize,
		PluginCreationCallback) override;

	std::unique_ptr < AudioProcessor> createCabbagePlugin(const String file);

	bool requiresUnblockedMessageThreadDuringCreation(const PluginDescription&) const noexcept override;
};
