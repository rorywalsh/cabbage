/*
  Copyright (C) 2010 Rory Walsh, Damien Rennick

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

#ifndef __CABBAGELOOKANDFEEL_H_
#define __CABBAGELOOKANDFEEL_H_

/*
  ====================================================================================

	Cabbage Look And Feel Class

  ====================================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/CabbageUtilities.h"
//#include "BinaryData.h"


class CabbageLookAndFeel	:	public LookAndFeel_V2
{
public:
    CabbageLookAndFeel();
    ~CabbageLookAndFeel();


    void drawTableHeaderColumn (Graphics& g, const String& columnName, int /*columnId*/,
                                int width, int height,
                                bool isMouseOver, bool isMouseDown,
                                int columnFlags);
    void drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header);
    Image drawLinearThumbImage (float width, float height, const Colour thumbFill, bool isVertical, String svgPath);
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           Slider &slider);
    void drawLinearSliderBackground (Graphics &g, int x, int y, int width, int height,
                                     float sliderPos,
                                     float minSliderPos,
                                     float maxSliderPos,
                                     const Slider::SliderStyle style,
                                     Slider &slider);
    void drawLinearSliderThumb (Graphics &g, int x, int y, int width, int height, float sliderPos,
                                float minSliderPos,
                                float maxSliderPos,
                                const Slider::SliderStyle style,
                                Slider &slider);

    void drawButtonBackground (Graphics&, Button&, const Colour&, bool, bool);
    void drawButtonText (Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown);

    void drawLabel (Graphics &g, Label &label);
    void drawComboBox (Graphics&, int, int, bool, int, int, int, int, ComboBox&);
    Font getComboBoxFont (ComboBox& box);
    Label* createComboBoxTextBox (ComboBox&);
    void drawBubble (Graphics&, BubbleComponent&, const Point<float>& tip, const Rectangle<float>& body) override;
    Font getSliderPopupFont (Slider&) override;
    int getSliderPopupPlacement (Slider&) override;

    void positionComboBoxText (ComboBox& box, Label& label);
    void drawToggleButton (Graphics &g, ToggleButton &button, bool isMouseOverButton, bool isButtonDown);
    void drawTextEditorOutline (Graphics &g, int width, int height, TextEditor &textEditor);
    void fillTextEditorBackground (Graphics &g, int width, int height, TextEditor &textEditor);
    void drawGroupComponentOutline (Graphics &g, int w, int h, const String &text, const Justification &position,
                                    GroupComponent &group);
    int getDefaultScrollbarWidth();
    int getMinimumScrollbarThumbSize()
    {
        return 15;
    }
    void drawScrollbar (Graphics &g, ScrollBar &scrollbar, int x, int y, int width, int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool isMouseOver,
                        bool isMouseDown);
    void drawScrollbarButton (Graphics &g, ScrollBar &scrollbar, int width, int height, int buttonDirection,
                              bool isScrollbarVertical,
                              bool isMouseOverButton,
                              bool isButtonDown);

    void setDefaultSansSerifTypefaceName (const String &newName);
    void drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar);
    void drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
                         const String &itemText, bool isMouseOverItem,
                         bool isMenuOpen, bool isMouseOverBar,
                         MenuBarComponent &menuBar);

    void drawPopupMenuBackground(Graphics &g, int width, int height);
    Image drawCheckMark();
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const String& text,
                            const String& shortcutKeyText,
                            const Drawable* icon, const Colour* const textColourToUse);

    void drawDocumentWindowTitleBar (DocumentWindow &window, Graphics &g, int w, int h, int titleSpaceX, int titleSpaceW,
                                     const Image *icon,
                                     bool drawTitleTextOnLeft);
    Image drawWindowButtonNormal(int buttonType);
    Image drawWindowButtonIsOver(int buttonType);
    Button* createDocumentWindowButton (int buttonType);

    int getAlertBoxWindowFlags();
    int getAlertWindowButtonHeight();
    Font getAlertWindowMessageFont();
    Font getAlertWindowFont();
    void drawAlertBox (Graphics& g, AlertWindow& alert, const Rectangle<int>& textArea, TextLayout& textLayout);

    AlertWindow* createAlertWindow (const String& title, const String& message, const String& button1, const String& button2,
                                    const String& button3, AlertWindow::AlertIconType iconType, int numButtons,
                                    Component* associatedComponent);

    int getTabButtonSpaceAroundImage();
    int getTabButtonOverlap (int tabDepth);
    int getTabButtonBestWidth (TabBarButton&, int tabDepth);
    Rectangle<int> getTabButtonExtraComponentBounds (const TabBarButton&, Rectangle<int>& textArea, Component& extraComp);

    void drawTabButton (TabBarButton&, Graphics& g, bool isMouseOver, bool isMouseDown);
    void drawTabButtonText (TabBarButton&, Graphics& g, bool isMouseOver, bool isMouseDown);
    void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics& g, int w, int h);

    void createTabButtonShape (TabBarButton&, Path& path,  bool isMouseOver, bool isMouseDown);
    void fillTabButtonShape (TabBarButton&, Graphics& g, const Path& path, bool isMouseOver, bool isMouseDown);

    const Drawable* getDefaultFolderImage() override;
    const Drawable* getDefaultDocumentFileImage() override;
    ScopedPointer<Drawable> folderImage, documentImage;
    AttributedString createFileChooserHeaderText (const String& title, const String& instructions) override;

    void drawFileBrowserRow (Graphics&, int width, int height,
                             const String& filename, Image* icon,
                             const String& fileSizeDescription, const String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int itemIndex,
                             DirectoryContentsDisplayComponent&) override;

    Button* createFileBrowserGoUpButton() override;

    void layoutFileBrowserComponent (FileBrowserComponent&,
                                     DirectoryContentsDisplayComponent*,
                                     FilePreviewComponent*,
                                     ComboBox* currentPathBox,
                                     TextEditor* filenameBox,
                                     Button* goUpButton) override;


    void drawTreeviewPlusMinusBox (Graphics& g, int x, int y, int w, int h, bool isPlus, bool /*isMouseOver*/);
    void drawLevelMeter(Graphics &g, int width, int	height,	float level);
    Button* createTabBarExtrasButton();

    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&);

    void drawStretchableLayoutResizerBar (Graphics& g, int w, int h,
                                          bool /*isVerticalBar*/,
                                          bool isMouseOver,
                                          bool isMouseDragging);

    bool drawBackgroundForSVGSlider(Graphics& g, Component* comp, String type, int x, int y, int width, int height);
	static Image drawToggleImage(float width, float height, bool isToggleOn, Colour colour, bool isRect, String imgPath, float corners);
	static Image drawTextButtonImage(float width, float height, bool isButtonDown, Colour colour, String imgFile, int svgWidth, int svgHeight, bool on);
	static Image drawFromSVG(String svgString, int width, int height, AffineTransform affine);
	static Image drawFromPNG(String pngString, int width, int height, AffineTransform affine);
	
    int getDefaultMenuBarHeight()
    {
        return 18;
    }

    juce_UseDebuggingNewOperator


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageLookAndFeel);


};


/*
  ====================================================================================

	Alternative Look And Feel Class

  ====================================================================================
*/
class CabbageLookAndFeelBasic	:	public LookAndFeel_V2
{
public:
    CabbageLookAndFeelBasic();
    ~CabbageLookAndFeelBasic();

    void drawLabel(Graphics&, Label&);

    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&);

    void drawLinearSliderBackground (Graphics &g, int x, int y, int width, int height,
                                     float sliderPos,
                                     float minSliderPos,
                                     float maxSliderPos,
                                     const Slider::SliderStyle style,
                                     Slider &slider);
    void drawLinearSliderThumb (Graphics &g, int x, int y, int width, int height, float sliderPos,
                                float minSliderPos,
                                float maxSliderPos,
                                const Slider::SliderStyle style,
                                Slider &slider);
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown);

    void drawScrollbar (Graphics& g,
                        ScrollBar& scrollbar,
                        int x, int y,
                        int width, int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool /*isMouseOver*/,
                        bool /*isMouseDown*/);

    bool areScrollbarButtonsVisible();
    void drawScrollbarButton (Graphics& g, ScrollBar& scrollbar,
                              int width, int height, int buttonDirection,
                              bool /*isScrollbarVertical*/,
                              bool /*isMouseOverButton*/,
                              bool isButtonDown);
    ImageEffectFilter* getScrollbarEffect();
    int getMinimumScrollbarThumbSize (ScrollBar& scrollbar);
    int getDefaultScrollbarWidth();
    int getScrollbarButtonSize (ScrollBar& scrollbar);


    void drawDocumentWindowTitleBar (DocumentWindow &window, Graphics &g, int w, int h,
                                     int /*titleSpaceX*/,
                                     int titleSpaceW,
                                     const Image */*icon*/,

                                     bool /*drawTitleTextOnLeft*/);

    void drawResizableWindowBorder (Graphics &g, int w, int h, const BorderSize< int > &border, ResizableWindow &window);
    void drawPopupMenuBackground(Graphics &g, int width, int height);
    void drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar);
    Image drawCheckMark();
    void drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
                         const String &itemText,
                         bool isMouseOverItem,
                         bool isMenuOpen,
                         bool isMouseOverBar,
                         MenuBarComponent &menuBar);

    void drawPopupMenuItem (Graphics &g, int width, int height, bool isSeparator, bool /*isActive*/,
                            bool isHighlighted,
                            bool isTicked,
                            bool hasSubMenu,
                            const String &text,
                            const String &shortcutKeyText,
                            Image */*image*/,
                            const Colour */*const textColour*/);

    Button* createDocumentWindowButton (int buttonType);
    Image drawWindowButtonNormal(int buttonType);
    Image drawWindowButtonIsOver(int buttonType);
    void drawStretchableLayoutResizerBar (Graphics& g, int w, int h,
                                          bool /*isVerticalBar*/,
                                          bool isMouseOver,
                                          bool isMouseDragging);
    void drawToggleButton (Graphics &g, ToggleButton &button, bool /*isMouseOverButton*/, bool /*isButtonDown*/);
    juce_UseDebuggingNewOperator


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageLookAndFeelBasic);

};


#endif //__CABBAGELOOKANDFEEL_H_
