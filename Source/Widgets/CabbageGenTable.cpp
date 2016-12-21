/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageGenTable.h"


#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageGenTable::CabbageGenTable(ValueTree wData, CabbagePluginEditor* owner)
	: widgetData(wData),
	owner(owner),
	fontcolour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
	file(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file)),
	zoom(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom)),
	scrubberPos(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::scrubberposition)),
	startpos(-1),
	endpos(-1),
	tooltipText(String::empty)
{
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	

	addAndMakeVisible(table);
	
	ampRanges = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::amprange);
	initialiseGenTable(wData);

}

//===============================================================================
void CabbageGenTable::changeListenerCallback(ChangeBroadcaster *source)
{
    GenTable* genTable = dynamic_cast<GenTable*>(source);
    if(genTable)
    {
     //   if((genTable->getCurrentHandle() && genTable->displayAsGrid()!=1))
     //       popupBubble->showAt(genTable->getCurrentHandle(), AttributedString(genTable->getCoordinates()), 1050);
        if(genTable->changeMessage == "updateFunctionTable")
            owner->updatefTableData(genTable);
    }
}

void CabbageGenTable::initialiseGenTable(ValueTree wData)
{
    int fileTable = 0;

    if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file).isNotEmpty())
    {
        table.addTable(44100,
                        Colours::findColourForName(CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::tablecolour)[0].toString(), Colours::white),
                        1,
                        ampRanges,
                        0, this);
        fileTable=1;
        table.setFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file));
    }


    tables = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::tablenumber);
    for(int y=0; y<tables.size(); y++)
    {
        int tableNumber = tables[y];
        tableValues.clear();
        tableValues = owner->getTableFloats(tableNumber);

        if(tableNumber>0 && tableValues.size()>0)
        {
            //Logger::writeToLog("Table Number:"+String(tableNumber));

            StringArray pFields = owner->getTableStatement(tableNumber);
            int genRoutine = pFields[4].getIntValue();

            if(owner->csdCompiledWithoutError())
            {
                table.addTable(44100,
                                Colours::findColourForName(CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::tablecolour)[0].toString(), Colours::white),
                                (tableValues.size()>=MAX_TABLE_SIZE ? 1 : genRoutine),
                                ampRanges,
                                tableNumber, this);

                if(abs(genRoutine)==1 || tableValues.size()>=MAX_TABLE_SIZE)
                {
                    tableBuffer.clear();
                    int channels = 1;//for now only works in mono;;
                    tableBuffer.setSize(channels, tableValues.size());
                    tableBuffer.addFrom(0, 0, tableValues.getRawDataPointer(), tableValues.size());
                    table.setWaveform(tableBuffer, tableNumber);
                }
                else
                {
                    //cUtils::showMessage(tableValues.size());
                    table.setWaveform(tableValues, tableNumber);
                    //only enable editing for gen05, 07, and 02
                    if(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom)!=0)
					{
						CabbageUtilities::debug(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom));
                        table.setZoomFactor(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom));
					}
                    table.enableEditMode(pFields, tableNumber);
                }

                table.setOutlineThickness(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));

                if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::drawmode).toLowerCase()=="vu")
                    table.setDrawMode("vu");
            }
        }
    }

    var tableConfigArray = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::tableconfig);
    if(fileTable==1)
        tableConfigArray.insert(0, 0);
    table.configTableSizes(tableConfigArray);
    table.bringTableToFront(0);

    if(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::startpos)>-1 && CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::endpos)>0)
        table.setRange(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::startpos), CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::endpos));


    //set grid colour and background colours for all tables
    if(fileTable==0)
        table.setGridColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::tablegridcolour)));
    else
        table.setGridColour(Colours::transparentBlack);

    table.setBackgroundColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::tablebackgroundcolour)));
    table.setFill(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::fill));

    //set VU gradients based on tablecolours, take only the first three colours.
    Array<Colour> gradient;
    for(int i=0; i<3; i++)
    {
        //cUtils::debug(cAttr.getStringArrayPropValue(CabbageIDs::tablecolour, i));
        gradient.add(Colours::red);
		gradient.add(Colours::white);
    }

    table.setVUGradient(gradient);

    if(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::active)!=1)
        table.toggleEditMode(false);	
}

//===============================================================================
void CabbageGenTable::resized()
{
	table.setBounds(0, 0, getWidth(), getHeight());
}

//===============================================================================
void CabbageGenTable::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::update)==1)
	{
		const int numberOfTables = tables.size();
		tableBuffer.clear();
		for(int y=0; y<numberOfTables; y++)
		{
			int tableNumber = tables[y];
			tableValues.clear();
			tableValues = owner->getTableFloats(tableNumber);

			
			if(table.getTableFromFtNumber(tableNumber)->tableSize>=MAX_TABLE_SIZE)
			{
				tableBuffer.clear();
				tableBuffer.addFrom(y, 0, tableValues.getRawDataPointer(), tableValues.size());
				table.setWaveform(tableBuffer, tableNumber);
			}
			else
			{		
				table.setWaveform(tableValues, tableNumber, false);
				StringArray pFields = owner->getTableStatement(tableNumber);
				table.enableEditMode(pFields, tableNumber);
			}
		}		
		
		
		CabbageWidgetData::setProperty(valueTree, CabbageIdentifierIds::update, 0); //reset value for further updates
		
	}
	else
	{
		if(scrubberPos!=CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::scrubberposition))
		{
			var scrubPosition = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::scrubberposition);
			if(scrubPosition.size()>1)
			{
				scrubberPos = scrubPosition[0];
				int tableNumber = scrubPosition[1];
				table.setScrubberPos(scrubberPos, tableNumber);
			}
		}

		if(!CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::active))
			table.toggleEditMode(false);
		else
			table.toggleEditMode(true);


		if(ampRanges!=CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::amprange))
		{
			ampRanges = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::amprange);
			table.setAmpRanges(ampRanges);
			if(ampRanges.size()>2)
				table.enableEditMode(StringArray(""), ampRanges[2]);
		}

		if(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::startpos)!=startpos 
			||  CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::endpos)!=endpos)
		{
			table.setRange(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::startpos), CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::endpos));
			endpos = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::endpos);
			startpos = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::startpos);
		}

		if(zoom!=CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::zoom))
			zoom = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::zoom);
		
		handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	
	}
}