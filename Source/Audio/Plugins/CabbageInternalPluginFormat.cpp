/*
  ==============================================================================

    CabbageInternalPluginFormat.cpp
    Created: 27 Nov 2018 4:04:26pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageInternalPluginFormat.h"


AudioProcessor* InternalCabbagePluginFormat::createCabbagePlugin(const String file)
{
		AudioProcessor* processor;
		bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(file));

		if (isCabbageFile)
			processor = createCabbagePluginFilter(File(file));
		else
			processor = createGenericPluginFilter(File(file));

		AudioProcessor::setTypeOfNextNewPlugin(AudioProcessor::wrapperType_Undefined);
		jassert(processor != nullptr);
		processor->disableNonMainBuses();
		processor->setRateAndBufferSizeDetails(44100, 512);

		return processor;
}

void InternalCabbagePluginFormat::createPluginInstance(const PluginDescription& desc,
	double /*initialSampleRate*/,
	int /*initialBufferSize*/,
	void* userData,
	PluginCreationCallback callback)
{
	
	auto* p = (CabbagePluginProcessor*)createCabbagePlugin(desc.fileOrIdentifier);
	p->nodeId = AudioProcessorGraph::NodeID(desc.uid);
	callback(userData, (AudioPluginInstance*)p, p == nullptr ? NEEDS_TRANS("Invalid internal filter name") : String());
}

bool InternalCabbagePluginFormat::requiresUnblockedMessageThreadDuringCreation(const PluginDescription&) const noexcept
{
	return false;
}
bool requiresUnblockedMessageThreadDuringCreation(const PluginDescription&)
{
	return false;
}
