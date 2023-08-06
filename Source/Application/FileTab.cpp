/*
  Copyright (C) 2017 Rory Walsh

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

#include "FileTab.h"

FileTab::FileTab (String name, String filename, bool isCsdFile, String iconsPathName):
    TextButton (name, filename),
    isCsdFile(isCsdFile),
    csdFile (filename),
    play ("Play", DrawableButton::ButtonStyle::ImageStretched),
    close ("", DrawableButton::ButtonStyle::ImageStretched),
    showEditor ("", DrawableButton::ButtonStyle::ImageStretched),
    editGUI ("", DrawableButton::ButtonStyle::ImageStretched),
    armForRecord ("", DrawableButton::ButtonStyle::ImageStretched),
    iconsPath (iconsPathName),
    overlay()
{

    addChildComponent (overlay);
    overlay.setVisible (false);
    play.setName ("playButton");



    close.setName ("closeButton");
    close.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
    close.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
    close.setTooltip ("Close file");
    close.getProperties().set ("filename", csdFile.getFullPathName());

    play.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
    play.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
    play.setClickingTogglesState (false);
    play.getProperties().set ("filename", csdFile.getFullPathName());
    play.getProperties().set ("state", "off");

    showEditor.setName ("showEditorButton");
    showEditor.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
    showEditor.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
    showEditor.setClickingTogglesState (true);
    showEditor.setTooltip ("Show plugin Editor");

    editGUI.setName ("editGUIButton");
    editGUI.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
    editGUI.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
    editGUI.setClickingTogglesState (true);
    editGUI.setTooltip ("Edit Plugin GUI");

    armForRecord.setName ("armForRecord");
    armForRecord.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
    armForRecord.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
    armForRecord.setClickingTogglesState (true);
    armForRecord.setTooltip ("Arm for record");
    
    
    setDrawableImages (play, 60, 25, "play");
    setDrawableImages (close, 25, 25, "close");
    setDrawableImages (showEditor, 25, 25, "showEditor");
    setDrawableImages (editGUI, 25, 25, "editGUI");
    setDrawableImages (armForRecord, 55, 25, "armForRecord");

    addAndMakeVisible (close);

    if(isCsdFile)
    {
        addAndMakeVisible(play);
        addAndMakeVisible(showEditor);
        addAndMakeVisible(editGUI);
        addAndMakeVisible(armForRecord);
    }
}

void FileTab::drawButtonShape (Graphics& g, const Path& outline, Colour baseColour, float height)
{
    const float mainBrightness = baseColour.getBrightness();
    const float mainAlpha = baseColour.getFloatAlpha();

    //g.setGradientFill (ColourGradient (baseColour.brighter (0.2f), 0.0f, 0.0f,
    //                                   baseColour.darker (0.25f), 0.0f, height, false));
    g.setColour (baseColour);
    g.fillPath (outline);

    g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
    g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
                  .scaled (1.0f, (height - 1.6f) / height));

    g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
    g.strokePath (outline, PathStrokeType (1.0f));
}


void FileTab::drawButtonText (Graphics& g)
{
    Font font(jmin(15.0f, getHeight() * 0.6f));
    g.setFont(font);
    g.setColour(fontColour.darker(getToggleState() ? 0.f : 0.5f));

    const int yIndent = jmin(4, proportionOfHeight(0.3f));
    const int leftIndent = 140;

    const int textWidth = getWidth() - 165;

    if (textWidth > 0)
    {
        if(isCsdFile)
        g.drawFittedText(getButtonText(),
                         leftIndent, yIndent, textWidth, getHeight() - yIndent * 2,
                         Justification::centred, 1);
        else
            g.drawFittedText(getButtonText(),
                             10, yIndent, getWidth(), getHeight() - yIndent * 2,
                             Justification::centred, 1);
    }


}

const String FileTab::getFilename()
{
    return csdFile.getFullPathName();
}

void FileTab::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    if (isEnabled() == false)
        jassert (false);

    Colour baseColour (getToggleState() ? buttonColour : buttonColour.darker (1.0f)); // Colour (30, 30, 30));

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

    overlay.overlayColour = baseColour;

    const bool flatOnLeft   = isConnectedOnLeft();
    const bool flatOnRight  = isConnectedOnRight();
    const bool flatOnTop    = isConnectedOnTop();
    const bool flatOnBottom = isConnectedOnBottom();

    const float width  = getWidth() - 1.0f;
    const float height = getHeight() - 1.0f;

    if (width > 0 && height > 0)
    {
        const float cornerSize = 4.0f;

        Path outline;
        outline.addRoundedRectangle (0.5f, 0.5f, width, height, cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));

        drawButtonShape (g, outline, baseColour, height);
    }

    drawButtonText (g);
}

void FileTab::addButtonListeners (Button::Listener* listener)
{
    play.addListener (listener);
    close.addListener (listener);
    showEditor.addListener (listener);
    editGUI.addListener (listener);
    armForRecord.addListener (listener);
}

void FileTab::disableButtons (bool disable)
{
    if(isCsdFile)
    {
        if (disable)
        {
            overlay.setVisible(true);
            overlay.toFront(true);
        } else
            overlay.setVisible(false);
    }

}

void FileTab::resized()
{
    overlay.setBounds (5, 3, 160, 25);
    play.setBounds (5, 3, 60, 25);
    showEditor.setBounds (67, 3, 30, 25);
    editGUI.setBounds (99, 5, 25, 23);
    armForRecord.setBounds (130, 9, 34, 12);
    close.setBounds (getWidth() - 22, 3, 20, 20);
}

extern const String getSVGTextFromFile (const String filename);

void FileTab::setDrawableImages (DrawableButton& button, int width, int height, String type)
{
    DrawableImage imageNormal, imageNormalPressed, imageDownPressed;

    if (type == "play")
    {
        DrawableImage imageDown;
        imageNormalPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, playButtonColour, false, true));
        imageDownPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, playButtonColour, true, true));
        imageNormal.setImage (CabbageImages::drawPlayStopIcon (width, height, playButtonColour, false));
        imageDown.setImage (CabbageImages::drawPlayStopIcon (width, height, playButtonColour, true));
        button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,  &imageDownPressed, &imageDownPressed);
    }
    else if (type == "close")
    {
        imageNormal.setImage (CabbageImages::drawCloseIcon (width, height));
        imageNormalPressed.setImage (CabbageImages::drawCloseIcon (width - 3, height - 3));
        button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal);
    }
    else if (type == "showEditor")
    {
        imageNormal.setImage (CabbageImages::drawEditorIcon (width, height));
        imageNormalPressed.setImage (CabbageImages::drawEditorIcon (width - 1, height - 1));
        button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr,  &imageNormalPressed, &imageNormalPressed);
    }
    else if (type == "editGUI")
    {
        String svgFile = getSVGTextFromFile (iconsPath + "/filetab-editGUI-off.svg");
		std::unique_ptr<XmlElement> svgOff(XmlDocument::parse (svgFile));

        svgFile = getSVGTextFromFile (iconsPath + "/filetab-editGUI-on.svg");
		std::unique_ptr<XmlElement> svgOn(XmlDocument::parse(svgFile));

        if (iconsPath == "" || svgOn == nullptr || svgOff == nullptr) 
        { // if there is no iconsPath defined (or the svg files are missing), then we fallback on the previous hard-coded icon:
            imageNormal.setImage (CabbageImages::drawEditGUIIcon (width, height));
            imageNormalPressed.setImage (CabbageImages::drawEditGUIIcon (width - 1, height - 1));
            button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr, &imageNormalPressed, &imageNormalPressed);
        }
        else
        {
            if (svgOff == nullptr || svgOn == nullptr)
                jassert (false);
            
            if (svgOff != nullptr)
                drawable_editGUIoff = std::unique_ptr<Drawable>(Drawable::createFromSVG (*svgOff));
            
            if (svgOn != nullptr)
                drawable_editGUIon = std::unique_ptr<Drawable>(Drawable::createFromSVG (*svgOn));

            button.setImages (drawable_editGUIoff.get(), drawable_editGUIoff.get(), drawable_editGUIoff.get(), drawable_editGUIoff.get(),
                drawable_editGUIon.get(), drawable_editGUIon.get(), drawable_editGUIon.get(), drawable_editGUIon.get());
        }
    }
    else if (type == "armForRecord")
    {
            String svgTextOff = R"xxx(
            <svg width="24" height="7" viewBox="0 0 24 7" fill="none" xmlns="http://www.w3.org/2000/svg">
            <circle cx="3" cy="3" r="3" fill="#B4B4B4"/>
            <path d="M8.70455 6V0.181818H10.6705C11.125 0.181818 11.4981 0.25947 11.7898 0.414773C12.0814 0.568182 12.2973 0.779356 12.4375 1.0483C12.5777 1.31723 12.6477 1.62311 12.6477 1.96591C12.6477 2.30871 12.5777 2.61269 12.4375 2.87784C12.2973 3.14299 12.0824 3.35133 11.7926 3.50284C11.5028 3.65246 11.1326 3.72727 10.6818 3.72727H9.09091V3.09091H10.6591C10.9697 3.09091 11.2197 3.04545 11.4091 2.95455C11.6004 2.86364 11.7386 2.73485 11.8239 2.56818C11.911 2.39962 11.9545 2.19886 11.9545 1.96591C11.9545 1.73295 11.911 1.52936 11.8239 1.35511C11.7367 1.18087 11.5975 1.0464 11.4062 0.951705C11.215 0.855114 10.9621 0.806818 10.6477 0.806818H9.40909V6H8.70455ZM11.4432 3.38636L12.875 6H12.0568L10.6477 3.38636H11.4432ZM13.8217 6V0.181818H17.3331V0.806818H14.5263V2.77273H17.1513V3.39773H14.5263V5.375H17.3786V6H13.8217ZM23.2848 2H22.5803C22.5386 1.79735 22.4657 1.61932 22.3615 1.46591C22.2592 1.3125 22.1342 1.18371 21.9865 1.07955C21.8407 0.973485 21.6787 0.893939 21.5007 0.840909C21.3227 0.787879 21.1371 0.761364 20.9439 0.761364C20.5916 0.761364 20.2725 0.850379 19.9865 1.02841C19.7024 1.20644 19.4761 1.46875 19.3075 1.81534C19.1409 2.16193 19.0575 2.58712 19.0575 3.09091C19.0575 3.5947 19.1409 4.01989 19.3075 4.36648C19.4761 4.71307 19.7024 4.97538 19.9865 5.15341C20.2725 5.33144 20.5916 5.42045 20.9439 5.42045C21.1371 5.42045 21.3227 5.39394 21.5007 5.34091C21.6787 5.28788 21.8407 5.20928 21.9865 5.10511C22.1342 4.99905 22.2592 4.86932 22.3615 4.71591C22.4657 4.56061 22.5386 4.38258 22.5803 4.18182H23.2848C23.2318 4.47917 23.1352 4.74527 22.995 4.98011C22.8549 5.21496 22.6806 5.41477 22.4723 5.57955C22.264 5.74242 22.0301 5.86648 21.7706 5.9517C21.513 6.03693 21.2375 6.07955 20.9439 6.07955C20.4477 6.07955 20.0064 5.95833 19.62 5.71591C19.2337 5.47348 18.9297 5.12879 18.7081 4.68182C18.4865 4.23485 18.3757 3.70455 18.3757 3.09091C18.3757 2.47727 18.4865 1.94697 18.7081 1.5C18.9297 1.05303 19.2337 0.708333 19.62 0.465909C20.0064 0.223485 20.4477 0.102273 20.9439 0.102273C21.2375 0.102273 21.513 0.144886 21.7706 0.230114C22.0301 0.315341 22.264 0.440341 22.4723 0.605114C22.6806 0.767992 22.8549 0.966856 22.995 1.2017C23.1352 1.43466 23.2318 1.70076 23.2848 2Z" fill="#B4B4B4"/>
            </svg>
            )xxx";

            String svgTextOn =R"xxx(
        <svg width="24" height="7" viewBox="0 0 24 7" fill="none" xmlns="http://www.w3.org/2000/svg">
        <circle cx="3" cy="3" r="3" fill="#FF0000"/>
        <path d="M8.70455 6V0.181818H10.6705C11.125 0.181818 11.4981 0.25947 11.7898 0.414773C12.0814 0.568182 12.2973 0.779356 12.4375 1.0483C12.5777 1.31723 12.6477 1.62311 12.6477 1.96591C12.6477 2.30871 12.5777 2.61269 12.4375 2.87784C12.2973 3.14299 12.0824 3.35133 11.7926 3.50284C11.5028 3.65246 11.1326 3.72727 10.6818 3.72727H9.09091V3.09091H10.6591C10.9697 3.09091 11.2197 3.04545 11.4091 2.95455C11.6004 2.86364 11.7386 2.73485 11.8239 2.56818C11.911 2.39962 11.9545 2.19886 11.9545 1.96591C11.9545 1.73295 11.911 1.52936 11.8239 1.35511C11.7367 1.18087 11.5975 1.0464 11.4062 0.951705C11.215 0.855114 10.9621 0.806818 10.6477 0.806818H9.40909V6H8.70455ZM11.4432 3.38636L12.875 6H12.0568L10.6477 3.38636H11.4432ZM13.8217 6V0.181818H17.3331V0.806818H14.5263V2.77273H17.1513V3.39773H14.5263V5.375H17.3786V6H13.8217ZM23.2848 2H22.5803C22.5386 1.79735 22.4657 1.61932 22.3615 1.46591C22.2592 1.3125 22.1342 1.18371 21.9865 1.07955C21.8407 0.973485 21.6787 0.893939 21.5007 0.840909C21.3227 0.787879 21.1371 0.761364 20.9439 0.761364C20.5916 0.761364 20.2725 0.850379 19.9865 1.02841C19.7024 1.20644 19.4761 1.46875 19.3075 1.81534C19.1409 2.16193 19.0575 2.58712 19.0575 3.09091C19.0575 3.5947 19.1409 4.01989 19.3075 4.36648C19.4761 4.71307 19.7024 4.97538 19.9865 5.15341C20.2725 5.33144 20.5916 5.42045 20.9439 5.42045C21.1371 5.42045 21.3227 5.39394 21.5007 5.34091C21.6787 5.28788 21.8407 5.20928 21.9865 5.10511C22.1342 4.99905 22.2592 4.86932 22.3615 4.71591C22.4657 4.56061 22.5386 4.38258 22.5803 4.18182H23.2848C23.2318 4.47917 23.1352 4.74527 22.995 4.98011C22.8549 5.21496 22.6806 5.41477 22.4723 5.57955C22.264 5.74242 22.0301 5.86648 21.7706 5.9517C21.513 6.03693 21.2375 6.07955 20.9439 6.07955C20.4477 6.07955 20.0064 5.95833 19.62 5.71591C19.2337 5.47348 18.9297 5.12879 18.7081 4.68182C18.4865 4.23485 18.3757 3.70455 18.3757 3.09091C18.3757 2.47727 18.4865 1.94697 18.7081 1.5C18.9297 1.05303 19.2337 0.708333 19.62 0.465909C20.0064 0.223485 20.4477 0.102273 20.9439 0.102273C21.2375 0.102273 21.513 0.144886 21.7706 0.230114C22.0301 0.315341 22.264 0.440341 22.4723 0.605114C22.6806 0.767992 22.8549 0.966856 22.995 1.2017C23.1352 1.43466 23.2318 1.70076 23.2848 2Z" fill="#B4B4B4"/>
        </svg>
            )xxx";

        std::unique_ptr<XmlElement> svgOn(XmlDocument::parse(svgTextOn));
        drawable_armForRecordOn = std::unique_ptr<Drawable>(Drawable::createFromSVG (*svgOn));
        std::unique_ptr<XmlElement> svgOff(XmlDocument::parse(svgTextOff));
        drawable_armForRecordOff = std::unique_ptr<Drawable>(Drawable::createFromSVG (*svgOff));
        
        button.setImages (drawable_armForRecordOff.get(), drawable_armForRecordOff.get(), drawable_armForRecordOn.get(), drawable_armForRecordOn.get(), drawable_armForRecordOn.get(), drawable_armForRecordOn.get(),  drawable_armForRecordOn.get(), drawable_armForRecordOn.get());
    }
}

FileTab::~FileTab()
{
}

void FileTab::setIconsPath(String path)
{
    iconsPath = path;
}
