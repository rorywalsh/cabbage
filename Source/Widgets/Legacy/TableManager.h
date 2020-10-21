/*
  Copyright (C) 20139 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef GENTABLE_H
#define GENTABLE_H

#define FIXED_WIDTH 15
#define MAX_TABLE_SIZE 44101

#include "../../CabbageCommonHeaders.h"
#include "../../LookAndFeel/CabbageLookAndFeel2.h"

class RoundButton;
class HandleViewer;
class HandleComponent;
class GenTable;

class TableManager : public Component,
    private ScrollBar::Listener,
    public ChangeListener
{
    double zoom = 0;
    int currentTableIndex = 0;
    var tableConfigList;
    int largestTable = 0;
    double scrubberPosition = 0;
    double scrubberFreq = 0;
    bool shouldShowTableButtons = false;
    bool shouldShowZoomButtons = false;
    int mainFooterHeight = 0;
    Colour gridColour = {};
    Colour backgroundColour = {};
    int tableIndex = 0;
    bool scrollbarEnabled = false;

public:
    TableManager();
    ~TableManager() {};
    void paint (Graphics& g)  override;
    void setGridColour (Colour col);
    void setOutlineThickness (float thickness);
    void setFill (bool fill );
    void setTableColours (var colours);
    void setBackgroundColour (Colour col);
    void repaintAllTables();
    void resized() override;
    void setZoomFactor (double zoom);
    void setDrawMode (String mode);
    void showScrollbar (bool show);
    void bringButtonsToFront();
    void setAmpRanges (var ampRange);
    void timerCallback();
    void updateScrollbars();
    void setRange (double start, double end);
    std::unique_ptr<DrawableRectangle> currentPositionMarker;
    double getLengthInSamples();
    void setScrubberPos (double pos, int tableNum);
    void scroll (double newRangeStart);
    void addTable (int sr, const Colour col, int gen, var ampRange, int ftnumber, ChangeListener* listener);
    void setWaveform (AudioSampleBuffer buffer, int ftNumber);
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void setWaveform (Array<float, CriticalSection> buffer, int ftNumber, bool updateRange = true);
    void setFile (const File file);
    void enableEditMode (StringArray pFields, int ftnumber);
    void toggleEditMode (bool enable);
    std::unique_ptr<RoundButton> zoomIn, zoomOut;
    OwnedArray<RoundButton> tableButtons;
    OwnedArray<GenTable> tables;
    void showZoomButtons (bool show);
    void showTableButtons (bool show);
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void bringTableToFront (int ftNumber);
    void configTableSizes (var tableConfig);
    void setVUGradient (Array<Colour> colour);
    GenTable* getLargestTable();
    GenTable* getTableFromFtNumber (int ftnumber);
};

//=================================================================
// display a sound file as a waveform..
//=================================================================
class GenTable : public Component,
    public ChangeBroadcaster,
    private ScrollBar::Listener,
    public ChangeListener
{
public:
    GenTable();
    ~GenTable();

    double getCurrentPlayPos()
    {
        return currentPlayPosition;
    }

    int getCurrentPlayPosInSamples()
    {
        return currentPlayPosition * sampleRate;
    }

    int getLoopLengthInSamples()
    {
        return loopLength * sampleRate;
    }

    void setColour (Colour col) { tableColour = col; repaint();    }
    void setScrubberPos (double pos);

    float timeToX (const double time) const
    {
        return getWidth() * (double) ((time - visibleRange.getStart()) / (visibleRange.getLength()));
    }

    double xToTime (const float x) const
    {
        return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
    }

    void setSampleRange (double pos, double end);
    void setZoomFactor (double amount);
    void setFile (const File& file);
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override;
    void setWaveform (AudioSampleBuffer buffer);
    void enableEditMode (StringArray pFields);
    juce::Point<int> tableTopAndHeight;
    void setWaveform (Array<float, CriticalSection> buffer, bool updateRange = true);
    void createImage (String filename);
    void addTable (int sr, const Colour col, int gen, var ampRange);
    static float ampToPixel (int height, Range<float> minMax, float sampleVal);
    static float pixelToAmp (int height, Range<float> minMax, float sampleVal);
    Array<double> getPfields();
    String changeMessage = {};
    Colour gridColour = {};
    int tableNumber = 0, tableSize = 0, genRoutine = 0, realGenRoutine = 0;
    void setRange (Range<double> newRange, bool isScrolling = false);
    Range<double> globalRange = {};
    bool isTableOnTop = false;
    std::unique_ptr<ScrollBar> scrollbar;
    void resized() override;
    Range<double> visibleRange = {};
    bool drawGrid = false;
    int scrollbarReduction = 0;
    void showScrollbar (bool show);
    int mainFooterHeight = 0, paintFooterHeight = 0, zoomButtonsOffset = 0;

    HandleViewer* getHandleViewer()
    {
        return handleViewer.get();
    }

    double quantiseSpace = 0;
    void setAmpRanges (var ampRange);
    void setXPosition (double pos);
    bool drawAsVUMeter = false;

    HandleComponent* getCurrentHandle()
    {
        if (currentHandle)
            return currentHandle;
        else
        {
            jassert (false);
            return nullptr;
        }

    }

    String getCoordinates()
    {
        return coordinates;
    }

    double getNewRangeStart()
    {
        return newRangeStart;
    }

    void shouldDrawGrid (bool val)
    {
        drawGrid = val;
    }

    int displayAsGrid()
    {
        return qsteps;
    }

    void setTraceThickness (float thickness)
    {
        traceThickness = thickness;
    }

    void shouldFillTable (bool fill)
    {
        shouldFill = fill;
    }

    void setBackgroundColour (Colour col)
    {
        backgroundColour = col;
    }

    void setVUGradient (ColourGradient grad)
    {
        vuGradient = grad;
    }

private:
    void drawBackgroundGrid();
    Image backgroundImage = {};
    bool shouldFill = false;
    float traceThickness = 0.f;
    bool paintCachedImage = false;
    String coordinates = {};
    double newRangeStart = 0.0;
    HandleComponent* currentHandle = nullptr;
    bool shouldScroll = false;
    int normalised = 0;
    int imgCount = 0;
    Colour fillColour = {};
    Colour backgroundColour = {};
    float currentWidth = 0.f;
    double zoom = 0.0;
    bool showScroll = false;
    double qsteps = 0.0;
    double numPixelsPerIndex = 0.0;
    ColourGradient gradient = {};
    StringArray pFields = {};
    std::unique_ptr<DrawableRectangle> currentPositionMarker;
    juce::Rectangle<int> thumbArea = {};
    juce::Rectangle<int> handleViewerRect = {};
    void paint (Graphics& g)  override;
    void mouseDown (const MouseEvent& e)  override;
    void mouseUp (const MouseEvent& e)  override;
    void mouseEnter (const MouseEvent& e)  override;
    void mouseDrag (const MouseEvent& e)  override;
    void mouseExit (const MouseEvent& e)  override;
    bool reDraw = false;
    double scrubberPosition = 0.0;
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    std::unique_ptr<HandleViewer> handleViewer;
    AudioFormatManager formatManager;
    double sampleRate = 44100;
    float regionWidth = 0;
    Image waveformImage = {};
    AudioThumbnailCache thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;
    Colour tableColour, fontcolour;
    int mouseDownX = 0, mouseUpX = 0;
    juce::Rectangle<int> localBounds = {};
    double loopLength = 0;
    double loopStart = 0;
    double currentPlayPosition = 0;
    bool drawWaveform = false;
    ColourGradient vuGradient;

    const Image drawGridImage (bool redraw, double width = 0.0, double height = 0.0, double offset = 0.0);

    Array<float, CriticalSection> waveformBuffer;
    double visibleLength = 0, visibleStart = 0, visibleEnd = 0, maxAmp = 0;
    Range<float> minMax;

    Range<float> findMinMax (Array<float, CriticalSection> buffer)
    {
        float min = buffer[0], max = buffer[0];

        for (int i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] > max)
                max = buffer[i];

            if (buffer[i] < min)
                min = buffer[i];
        }

        return Range<float> (min, max);
    }

};

//==============================================================================
// HandleViewer class, holds breakpoint handles
//==============================================================================
class HandleViewer : public Component
{

public:
    HandleViewer();
    ~HandleViewer();
    std::unique_ptr<TextButton> button1;
    std::unique_ptr<TextButton> button2;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void positionHandle (const MouseEvent& e);
    void repaint (Graphics& g);
    void resized() override;
    void addHandle (double x, double y, double width, double height, Colour colour, bool status = false);
    void insertHandle (double x, double y, Colour colour);
    double getSnapYPosition (const double y);
    double getSnapXPosition (const double x);
    HandleComponent* getPreviousHandle (HandleComponent* thisHandle);
    HandleComponent* getNextHandle (HandleComponent* thisHandle);
    int getHandleIndex (HandleComponent* thisHandle);
    void removeHandle (HandleComponent* thisHandle);
    OwnedArray<HandleComponent, CriticalSection> handles;
    void fixEdgePoints (int gen);
    void showHandles (bool show);
    int handleIndex = 0;
    bool showingGrid = false;
    double tableSize = 0.0;
    Range<float> minMax = {};
    Colour colour = {};
    int handleViewerGen = 0;

    bool isShowingGrid()
    {
        return showingGrid;
    }

    void setShowingGrid (bool val)
    {
        showingGrid = val;
    }
    bool shouldShowHandles;

    GenTable* getParentTable()
    {
        return findParentComponentOfClass <GenTable>();
    };

};

//==============================================================================
// Handle class
//==============================================================================
class HandleComponent : public Component,
    public ChangeBroadcaster
{
public:
    HandleComponent (double xPos, double yPos, int index, bool fixed, int gen, Colour colour);
    ~HandleComponent();


    void setPosition (double x, double y, bool circularEnv);

    void paint (Graphics& g)  override;
    void removeThisHandle();
    void mouseEnter (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    int index;
    int height, width;
    int x, y;
    void setColour (Colour icolour);
    void setRelativePosition (juce::Point<double> point);

    HandleViewer* getParentHandleViewer()
    {
        return findParentComponentOfClass <HandleViewer>();
    };

    GenTable* getParentGenTable()
    {
        return findParentComponentOfClass <GenTable>();
    };

    TableManager* getParentTableManager()
    {
        return findParentComponentOfClass <TableManager>();
    };

    int getUniqueID()
    {
        return uniqueID;
    }

    void setUniqueID (int _id)
    {
        uniqueID = _id;
    }

    double getDoubleY()
    {
        return highResY;
    }

    void setDoubleY (double _y)
    {
        highResY = _y;
    }

    HandleComponent* getPreviousHandle();
    HandleComponent* getNextHandle();
    String changeMessage;
    String mouseStatus;
    double xPosRelative, yPosRelative;
    bool status;

private:
    Colour colour;
    bool fixed;
    double highResY;
    int uniqueID;

    ComponentDragger dragger;
    int lastX, lastY;
    int offsetX, offsetY;
    int genRoutine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HandleComponent);
};

#endif // SOUNDFILEWAVEFORM_H
