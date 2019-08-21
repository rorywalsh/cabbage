/*
  ==============================================================================

    CabbageInternalPluginFormat.cpp
    Created: 27 Nov 2018 4:04:26pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageInternalPluginFormat.h"


std::unique_ptr<AudioProcessor> InternalCabbagePluginFormat::createCabbagePlugin(const String file)
{
		std::unique_ptr < AudioProcessor> processor;
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
	double /*initialSampleRate*/, int /*initialBufferSize*/,
	PluginCreationCallback callback)
{
	/*if (auto p = createInstance(desc.name))
		callback(std::move(p), {});
	else
		callback(nullptr, NEEDS_TRANS("Invalid internal plugin name"));*/

	std::unique_ptr < AudioProcessor > p(createCabbagePlugin(desc.fileOrIdentifier));
	std::unique_ptr <AudioPluginInstance> p1(dynamic_cast<AudioPluginInstance*>(p.get()));

	//p-> = AudioProcessorGraph::NodeID(desc.uid);
	callback(std::move(p1), {});
}

bool InternalCabbagePluginFormat::requiresUnblockedMessageThreadDuringCreation(const PluginDescription&) const noexcept
{
	return false;
}
bool requiresUnblockedMessageThreadDuringCreation(const PluginDescription&)
{
	return false;
}
