/*
  ==============================================================================

    CabbagePluginProcessor.h
    Created: 24 Oct 2016 6:38:24pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEPLUGINPROCESSOR_H_INCLUDED
#define CABBAGEPLUGINPROCESSOR_H_INCLUDED

#include "CsoundPluginProcessor.h"
#include "../Widgets/CabbageWidgetData.h"
#include "../CabbageIds.h"
#include "CabbageAudioParameter.h"

class CabbagePluginProcessor  : public CsoundPluginProcessor
{
public:
	CabbagePluginProcessor(File inputFile);
	~CabbagePluginProcessor();	
	
	ValueTree cabbageWidgets;	

	void receiveChannelDataFromCsound();
	void parseCsdFile();
	void createParameters();

	//==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
	
private:
	File csdFile;

};



#endif  // CABBAGEPLUGINPROCESSOR_H_INCLUDED
