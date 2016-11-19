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

#include "CabbageLookAndFeel.h"


namespace LookAndFeelHelpers
{
static Colour createBaseColour (Colour buttonColour,
                                bool hasKeyboardFocus,
                                bool isMouseOverButton,
                                bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

    if (isButtonDown)      return baseColour.contrasting (0.2f);
    if (isMouseOverButton) return baseColour.contrasting (0.1f);

    return baseColour;
}

static TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
{
    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = 400;

    AttributedString s;
    s.setJustification (Justification::centred);
    s.append (text, Font (tooltipFontSize, Font::bold), colour);

    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
}
}
//main Cabbage look and feel class
CabbageLookAndFeel::CabbageLookAndFeel()
{
    setColour(AlertWindow::backgroundColourId, CabbageUtilities::getDarkerBackgroundSkin());
    setColour(AlertWindow::textColourId, Colour(200, 200, 200));
    setColour(AlertWindow::outlineColourId, Colours::whitesmoke);
    setColour(DirectoryContentsDisplayComponent::textColourId, Colours::black);
    setColour(DirectoryContentsDisplayComponent::highlightColourId, Colours::whitesmoke);
    setColour(ListBox::backgroundColourId, CabbageUtilities::getDarkerBackgroundSkin());
    //setColour(TableListBox::textColourId, Colours::white);
    setColour(ListBox::textColourId, Colours::white);
    setColour(Label::textColourId, Colours::white);
    setColour(TextButton::textColourOnId, CabbageUtilities::getComponentFontColour());
    setColour(TextButton::textColourOffId, CabbageUtilities::getComponentFontColour());
    setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    setColour(ComboBox::textColourId, CabbageUtilities::getComponentFontColour());
    setColour(ComboBox::backgroundColourId, CabbageUtilities::getDarkerBackgroundSkin());
    setColour(ScrollBar::trackColourId, Colours::transparentBlack);
    setColour(ScrollBar::thumbColourId, CabbageUtilities::getComponentSkin());
    setColour(TooltipWindow::backgroundColourId, CabbageUtilities::getDarkerBackgroundSkin());
    setColour(TooltipWindow::textColourId, CabbageUtilities::getComponentFontColour());

    setColour(CaretComponent::caretColourId, Colours::white);
    setColour(ListBox::outlineColourId, CabbageUtilities::getDarkerBackgroundSkin());
    //browserComponent.setColour(Label::backgroundColourId, Colours::cornflowerblue);
    //setColour(ListBox::textColourId, Colours::cornflowerblue);
    setColour(TextEditor::backgroundColourId, CabbageUtilities::getDarkerBackgroundSkin());
    setColour(TextEditor::textColourId, Colours::white);
    //setColour(TextEditor::highlightedTextColourId, Colours::lime);
    setColour(TextEditor::shadowColourId, Colours::pink);
    setColour(FileChooserDialogBox::titleTextColourId, Colours::lime);
}

CabbageLookAndFeel::~CabbageLookAndFeel()
{
}

//==============================================================================

void CabbageLookAndFeel::drawLevelMeter(Graphics &g, int width, int	height,	float level)
{
    g.setColour (Colours::white.withAlpha (0.7f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) width, (float) height, 3.0f);
    g.setColour (Colours::black.withAlpha (0.2f));
    g.drawRoundedRectangle (1.0f, 1.0f, width - 2.0f, height - 2.0f, 3.0f, 1.0f);

    const int totalBlocks = 15;
    const int numBlocks = roundToInt (totalBlocks * level);
    const float w = (width - 6.0f) / (float) totalBlocks;

    for (int i = 0; i < totalBlocks; ++i)
    {
        if (i >= numBlocks)
            g.setColour (Colours::lightblue.withAlpha (0.6f));
        else
            g.setColour (i < totalBlocks - 1 ? Colours::cornflowerblue
                         : Colours::red);

        g.fillRoundedRectangle (3.0f + i * w + w * 0.1f, 3.0f, w * 0.8f, height - 6.0f, w * 0.4f);
    }
}

//==============================================================================
void CabbageLookAndFeel::drawTreeviewPlusMinusBox (Graphics& g, int x, int y, int w, int h, bool isPlus, bool /*isMouseOver*/)
{
    const int boxSize = ((jmin (16, w, h) << 1) / 3) | 1;

    x += (w - boxSize) >> 1;
    y += (h - boxSize) >> 1;
    w = boxSize;
    h = boxSize;

    //g.setColour (Colour (0xe5ffffff));
    g.setColour(Colours::black);//background
    g.fillRect (x, y, w, h);

    //g.setColour (Colour (0x80000000));
    g.setColour(Colours::cornflowerblue);//outline
    g.drawRect (x, y, w, h);

    //g.setColour(Colours::lime);

    const float size = boxSize / 2 + 1.0f;
    const float centre = (float) (boxSize / 2);
    //plus sign
    g.setColour(Colours::green);

    g.fillRect (x + (w - size) * 0.5f, y + centre, size, 1.0f);

    if (isPlus)
    {
        g.fillRect (x + centre, y + (h - size) * 0.5f, 1.0f, size);

    }
}

//==============================================================================
AttributedString CabbageLookAndFeel::createFileChooserHeaderText (const String& title,
        const String& instructions)
{
    AttributedString s;
    s.setJustification (Justification::centred);

    const Colour colour (CabbageUtilities::getComponentFontColour());
    s.append (title + "\n\n", Font (17.0f, Font::bold), colour);
    s.append (instructions, Font (14.0f), colour);

    return s;
}

void CabbageLookAndFeel::drawFileBrowserRow (Graphics& g, int width, int height,
        const String& filename, Image* icon,
        const String& fileSizeDescription,
        const String& fileTimeDescription,
        const bool isDirectory, const bool isItemSelected,
        const int /*itemIndex*/, DirectoryContentsDisplayComponent& dcc)
{
    Component* const fileListComp = dynamic_cast<Component*> (&dcc);

    if (isItemSelected)
        g.fillAll(Colours::black.brighter(.5f));
    else
        g.fillAll(Colours::black);

    const int x = 32;

//    if (icon != nullptr && icon->isValid())
//    {
//		g.setColour (Colours::black);
//		g.fillRoundedRectangle(0, 0, 30, height, 2);
////        g.drawImageWithin (*icon, 2, 2, x - 4, height - 4,
////                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
////                           false);
//    }
//    else
//    {
    if(isDirectory)
    {
        g.setColour (Colours::black);
        Path p;
        p.startNewSubPath(4, 2);
        p.lineTo(18, 2);
        p.lineTo(18, 5);
        p.lineTo(22, 5);
        p.lineTo(22, height-5);
        p.lineTo(4, height-5);
        p.lineTo(4, 2);
        g.strokePath(p, PathStrokeType(1));
        g.fillPath(p);
        g.setColour(Colours::black);
        p.scaleToFit(5, 3, 20, height-8, true);
        g.fillPath(p);
        p.startNewSubPath(8, 7);
        p.lineTo(24, 7);
        p.closeSubPath();
        g.setColour(Colours::white);
        g.fillPath(p);
        g.setColour (Colours::black);
        g.strokePath(p, PathStrokeType(1));
    }
    else
    {
        g.setColour (Colours::cornflowerblue);
        g.drawLine(8, 2, 15, 2);
        g.drawLine(15 ,2, 20, 5);
        g.drawLine(20, 5, 20, height-5);
        g.drawLine(20, height-5, 8, height-5);
        g.drawLine(8, height-5, 8, 2);
        g.setColour (Colours::cornflowerblue.darker(.6f));
    }



    g.setColour (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::textColourId)
                 : findColour (DirectoryContentsDisplayComponent::textColourId));
    g.setColour(Colours::whitesmoke);
    g.setFont (height * 0.7f);

    if (width > 450 && ! isDirectory)
    {
        const int sizeX = roundToInt (width * 0.7f);
        const int dateX = roundToInt (width * 0.8f);

        g.drawFittedText (filename,
                          x, 0, sizeX - x, height,
                          Justification::centredLeft, 1);

        g.setFont (height * 0.5f);
        g.setColour (Colours::cornflowerblue.brighter(6.6f));

        if (! isDirectory)
        {
            g.drawFittedText (fileSizeDescription,
                              sizeX, 0, dateX - sizeX - 8, height,
                              Justification::centredRight, 1);

            g.drawFittedText (fileTimeDescription,
                              dateX, 0, width - 8 - dateX, height,
                              Justification::centredRight, 1);
        }
    }
    else
    {
        g.drawFittedText (filename,
                          x, 0, width - x, height,
                          Justification::centredLeft, 1);

    }
}

Button* CabbageLookAndFeel::createFileBrowserGoUpButton()
{
    DrawableButton* goUpButton = new DrawableButton("up", DrawableButton::ImageOnButtonBackground);
    Path arrowPath;
    arrowPath.addArrow (Line<float> (50.0f, 100.0f, 50.0f, 0.0f), 40.0f, 100.0f, 50.0f);


    DrawablePath arrowImage;
    arrowImage.setFill (Colours::white);
    arrowImage.setStrokeFill (Colours::white);
    arrowImage.setPath (arrowPath);

    goUpButton->setImages (&arrowImage);

    return goUpButton;
}

void CabbageLookAndFeel::layoutFileBrowserComponent (FileBrowserComponent& browserComp,
        DirectoryContentsDisplayComponent* fileListComponent,
        FilePreviewComponent* previewComp,
        ComboBox* currentPathBox,
        TextEditor* filenameBox,
        Button* goUpButton)
{
    const int x = 8;
    int w = browserComp.getWidth() - x - x;

    if (previewComp != nullptr)
    {
        const int previewWidth = w / 3;
        previewComp->setBounds (x + w - previewWidth, 0, previewWidth, browserComp.getHeight());

        w -= previewWidth + 4;
    }

    int y = 4;

    const int controlsHeight = 22;
    const int bottomSectionHeight = controlsHeight + 8;
    const int upButtonWidth = 50;

    currentPathBox->setBounds (x, y, w - upButtonWidth - 6, controlsHeight);
    goUpButton->setBounds (x + w - upButtonWidth, y, upButtonWidth, controlsHeight);
    goUpButton->setColour(TextButton::buttonColourId, Colours::cornflowerblue);

    y += controlsHeight + 4;

    if (Component* const listAsComp = dynamic_cast <Component*> (fileListComponent))
    {
        listAsComp->setBounds (x, y, w, browserComp.getHeight() - y - bottomSectionHeight);
        y = listAsComp->getBottom() + 4;
    }

    filenameBox->setColour(TextEditor::backgroundColourId, Colour(210,210,210));
    filenameBox->setColour(TextEditor::textColourId, Colour(10, 10, 10));
    filenameBox->setBounds (x + 53, y, w - 56, controlsHeight);
    filenameBox->grabKeyboardFocus();
}

// Pulls a drawable out of compressed valuetree data..
static Drawable* loadDrawableFromData (const void* data, size_t numBytes)
{
    MemoryInputStream m (data, numBytes, false);
    GZIPDecompressorInputStream gz (m);
    ValueTree drawable (ValueTree::readFromStream (gz));
    return Drawable::createFromValueTree (drawable.getChild (0), nullptr);
}

const Drawable* CabbageLookAndFeel::getDefaultFolderImage()
{

    if (folderImage == nullptr)
    {
        static const unsigned char drawableData[] =
        {
            120,218,197,86,77,111,27,55,16,229,182,161,237,6,61,39,233,77,63,192,38,56,195,225,215,209,105,210,2,141,13,20,201,193,109,111,178,181,178,183,145,181,130,180,110,145,127,159,199,93,73,137,87,53,218,91,109,192,160,151,179,156,55,111,222,188,229,155,247,
            231,87,231,175,47,222,170,234,155,229,244,190,86,213,115,253,102,61,253,123,122,189,168,85,51,83,213,119,250,238,221,47,231,151,175,223,169,170,250,121,221,62,172,84,245,172,60,63,209,243,118,49,171,215,170,107,87,23,245,188,83,213,145,182,167,19,91,
            254,127,223,220,222,117,37,68,82,40,143,174,219,174,107,239,135,168,147,18,37,108,85,245,237,46,207,70,33,249,175,211,238,78,85,186,28,253,76,175,73,109,186,117,251,177,190,106,102,229,241,247,58,24,103,203,15,101,245,103,219,44,187,15,221,39,0,172,142,
            245,125,211,1,196,205,116,181,125,114,164,175,31,186,78,45,219,229,31,245,186,189,106,150,179,102,121,139,100,154,240,231,167,102,177,64,72,247,105,213,23,122,187,158,206,154,122,217,169,85,57,18,1,47,53,101,107,18,135,204,167,147,192,201,216,20,114,
            244,195,62,171,234,7,125,198,100,136,216,145,149,211,9,57,103,40,249,72,219,8,167,170,87,250,140,162,199,123,226,3,34,82,202,134,131,13,172,74,170,233,162,0,177,234,166,93,180,15,235,141,170,206,180,157,204,231,150,156,159,207,39,195,50,214,88,18,150,
            245,205,124,250,104,169,212,135,158,19,144,53,20,112,172,55,237,2,132,13,199,149,130,230,115,145,112,147,147,82,61,157,32,238,178,253,11,145,213,138,10,52,138,38,103,111,99,164,211,137,139,198,35,177,35,167,212,143,15,215,205,13,160,109,163,172,225,152,
            16,232,17,149,140,103,144,158,146,90,113,217,12,6,197,167,236,3,54,5,181,101,73,54,138,90,245,165,227,120,18,252,150,77,15,242,188,228,204,81,169,139,102,249,5,68,192,145,14,244,112,1,145,29,94,137,96,235,49,136,151,58,246,32,88,192,161,88,176,76,226,
            36,247,24,176,7,232,62,16,83,42,155,201,160,30,222,65,72,98,82,76,33,198,254,197,96,124,10,150,243,8,130,48,228,36,94,124,6,4,43,38,0,142,205,99,30,4,221,13,33,230,220,71,177,65,49,142,243,150,7,1,51,20,2,5,96,96,84,225,56,217,188,3,33,46,24,228,112,
            69,69,12,68,228,108,242,99,16,165,118,208,28,51,200,98,87,42,74,62,209,24,4,206,48,22,153,125,132,220,196,56,15,234,99,216,130,0,141,38,74,162,130,48,35,163,141,94,196,245,32,94,104,7,154,132,209,40,108,162,165,232,153,165,17,4,138,201,176,135,58,49,
            165,130,122,108,114,54,28,240,64,17,89,188,79,177,116,149,10,4,246,91,30,94,104,112,96,226,144,131,144,142,98,78,177,7,128,81,242,224,140,36,249,80,208,145,196,12,202,15,16,60,161,200,69,187,169,213,86,198,123,87,224,255,199,21,94,105,134,72,40,177,245,
            14,182,32,232,54,196,231,100,111,11,189,168,201,39,177,84,102,38,139,177,168,74,210,87,174,64,20,138,160,67,111,10,4,98,196,97,60,158,118,133,25,111,173,224,171,37,97,185,119,133,221,242,63,184,194,140,71,174,240,252,145,43,72,32,147,146,147,4,104,104,
            117,134,10,18,12,107,212,40,72,148,57,6,71,69,135,222,248,16,160,168,3,169,144,55,201,69,41,147,137,134,99,50,97,8,178,85,43,217,140,201,151,192,152,10,242,190,24,11,59,183,29,25,42,115,236,98,14,229,252,32,80,66,0,162,17,136,72,6,67,5,45,242,224,10,
            193,102,71,50,6,17,129,212,18,115,105,150,80,169,45,123,222,141,76,178,70,32,55,24,90,217,132,71,73,200,57,238,204,3,136,49,144,185,55,183,190,20,137,52,246,47,113,232,158,69,35,49,145,208,129,193,56,178,77,135,230,145,113,22,140,69,74,20,146,2,120,218,
            155,135,48,32,10,89,30,156,165,204,254,222,193,160,12,19,49,6,210,59,11,70,62,4,31,15,64,196,2,157,98,33,58,1,104,32,152,50,31,128,64,148,183,197,108,209,89,107,240,41,75,36,123,16,208,108,180,44,236,250,182,227,27,20,137,118,76,60,165,137,221,92,94,
            78,215,31,235,245,230,183,242,229,30,214,251,251,195,145,94,148,15,253,170,221,52,93,211,46,7,109,171,81,208,177,94,247,119,132,47,81,186,92,22,246,7,255,254,15,7,107,141,171,197,191,156,123,162,135,187,198,227,131,113,219,80,159,1,4,239,223,231,0,0
        };

        folderImage = loadDrawableFromData (drawableData, sizeof (drawableData));
    }

    return folderImage;
}

const Drawable* CabbageLookAndFeel::getDefaultDocumentFileImage()
{
    if (documentImage == nullptr)
    {
        static const unsigned char drawableData[] =
        {
            120,218,213,88,77,115,219,54,16,37,147,208,246,228,214,75,155,246,164,123,29,12,176,216,197,199,49,105,218,94,156,153,78,114,72,219,155,108,75,137,26,89,212,200,116,59,233,175,239,3,105,201,164,68,50,158,166,233,76,196,11,69,60,173,128,197,123,139,183,
            124,241,234,217,155,103,207,207,126,204,242,7,171,233,213,44,203,31,23,47,54,211,191,166,231,203,89,182,184,204,242,147,226,195,165,219,252,125,150,229,249,207,155,242,102,157,229,143,210,227,199,197,101,121,113,115,53,91,85,89,85,174,207,102,243,42,
            203,143,10,125,58,209,233,251,171,197,219,119,85,250,173,97,151,30,157,151,85,85,94,53,168,147,132,50,226,179,252,225,246,143,174,179,44,63,254,101,90,189,203,242,34,5,127,84,172,77,118,93,109,202,247,179,55,139,203,244,248,97,161,179,63,202,197,170,
            122,93,125,192,196,242,227,226,106,81,205,54,217,197,116,125,251,228,168,56,191,169,170,108,85,174,126,159,109,202,55,139,213,229,98,245,182,249,97,254,240,167,197,114,137,5,86,31,214,245,111,175,203,37,254,230,162,92,150,55,155,180,148,249,237,39,203,
            94,215,127,58,10,213,245,39,203,234,249,102,249,87,47,203,63,129,204,49,227,252,73,225,149,145,104,131,245,254,116,34,202,82,164,16,153,179,236,108,177,234,7,49,41,237,130,144,167,17,144,15,42,104,239,93,12,35,32,99,68,9,187,24,125,7,244,77,23,36,164,
            40,56,226,61,12,107,229,130,215,100,105,24,227,89,17,246,211,105,55,140,49,218,43,207,100,245,72,28,195,70,17,230,201,118,8,243,164,139,233,95,88,23,52,152,162,54,104,48,217,237,105,15,111,91,107,253,131,160,118,34,239,69,128,54,232,135,101,121,61,203,
            110,169,181,147,2,253,159,82,48,180,229,247,167,74,193,41,141,188,35,93,241,116,18,148,113,214,120,207,113,47,19,109,16,51,182,153,193,5,59,2,10,90,69,114,218,135,48,2,50,198,43,171,189,152,81,144,88,108,85,136,78,246,64,54,42,163,35,69,30,3,121,82,38,
            98,81,98,70,64,70,139,34,111,163,167,49,144,13,202,138,179,58,220,23,52,180,186,54,104,48,79,109,208,96,198,219,19,31,220,187,118,10,6,65,237,100,222,139,5,109,80,191,30,236,151,162,135,147,142,30,68,105,182,58,6,22,84,43,229,124,148,116,97,145,55,231,
            139,11,76,228,16,37,14,48,205,145,77,134,34,176,55,152,182,200,57,99,93,204,144,145,253,65,97,229,132,72,104,63,62,71,21,140,54,186,41,226,59,84,19,63,130,15,222,235,224,185,59,104,27,226,68,101,153,241,227,177,248,29,20,136,26,8,252,178,183,241,219,
            131,137,160,209,107,109,92,79,124,16,211,184,104,93,77,130,110,124,2,65,172,67,201,60,157,88,163,2,91,99,92,216,198,55,78,69,75,190,150,119,84,98,200,71,150,109,124,36,204,227,52,8,33,229,223,68,167,173,167,131,248,137,212,226,141,19,233,160,154,248,
            144,142,195,140,137,185,59,104,15,247,119,40,126,23,69,81,200,242,110,254,123,20,49,94,112,110,245,199,111,241,167,87,36,252,101,138,132,149,22,22,38,65,134,29,182,139,24,230,192,31,144,184,133,130,72,44,131,210,142,111,147,216,30,76,123,30,113,206,242,
            150,196,157,65,129,130,76,180,194,61,34,225,160,5,228,233,160,118,34,137,26,202,115,212,29,108,72,134,243,223,90,114,226,199,226,119,80,6,245,152,197,122,217,146,184,53,24,140,210,30,21,59,80,79,124,182,202,71,207,218,112,159,72,80,53,140,109,68,2,191,
            227,217,210,78,36,94,137,88,231,82,157,8,176,61,0,122,191,19,137,3,255,13,39,183,228,20,193,151,144,119,166,79,36,40,253,156,138,72,11,181,19,137,14,46,176,217,27,180,135,251,219,31,255,235,61,148,165,96,72,122,118,23,229,81,52,135,24,250,163,183,216,
            211,43,17,217,151,136,253,116,137,28,53,188,127,92,188,221,76,47,23,169,59,90,167,144,141,239,197,86,104,141,189,60,157,80,84,142,140,4,31,154,241,122,105,132,41,107,13,201,39,86,120,24,82,114,206,198,6,96,27,227,172,36,232,168,201,36,219,24,113,62,163,
            154,101,233,143,166,203,102,26,141,206,174,179,252,89,161,39,243,249,197,121,186,38,233,246,146,211,53,1,123,56,194,231,122,143,103,179,217,60,204,167,19,147,110,41,93,173,219,123,72,89,248,35,173,16,220,50,179,111,60,181,24,88,103,156,235,7,78,248,14,
            4,119,78,162,93,60,112,35,109,16,124,126,12,17,71,67,24,1,165,142,1,181,215,248,56,6,66,235,193,137,167,61,22,30,5,3,27,101,71,64,169,25,112,216,2,63,22,169,110,43,18,200,140,129,208,160,88,44,220,208,125,65,67,171,107,131,6,243,212,6,13,102,188,61,241,
            225,189,107,165,96,16,212,78,230,189,88,208,6,245,235,214,237,235,150,62,167,110,155,106,170,53,133,192,117,193,20,84,78,74,174,98,39,92,156,8,112,21,46,80,106,12,209,207,225,228,16,113,59,225,126,87,60,133,25,209,34,36,2,99,242,52,197,48,30,75,244,247,
            212,238,246,182,173,221,185,78,215,127,167,221,162,163,221,250,152,217,146,196,222,145,100,223,235,105,108,28,250,149,212,74,224,86,2,213,118,110,119,204,224,144,208,38,214,131,200,14,214,223,120,189,230,53,1,193,70,133,154,131,56,223,16,229,48,188,14,
            201,205,213,121,71,233,68,89,15,124,103,37,53,26,11,118,176,127,169,88,166,158,219,178,117,173,83,108,75,95,55,68,186,193,53,246,146,206,127,6,63,53,78,58,228,204,155,224,113,74,91,232,221,195,240,105,215,34,29,138,64,128,183,8,130,233,71,173,56,54,101,
            99,75,186,111,65,58,28,229,145,82,19,152,12,99,180,81,130,131,75,234,229,220,247,53,231,154,79,205,185,185,155,199,249,172,38,85,253,204,76,68,95,92,204,207,255,221,75,178,227,14,187,224,224,97,202,172,173,219,12,167,130,133,9,54,135,245,92,176,29,134,
            165,110,139,141,18,16,223,29,188,183,65,207,144,106,144,151,143,128,224,176,168,110,140,32,62,56,110,219,195,54,235,20,68,209,216,34,232,21,6,41,234,157,39,211,201,107,160,230,66,225,56,153,9,101,21,37,237,150,204,14,115,208,22,221,54,216,230,33,116,
            14,65,14,44,19,8,236,73,71,246,182,110,125,224,75,132,195,214,247,163,36,51,252,84,76,124,37,212,100,88,62,183,179,76,67,217,218,242,244,229,116,243,126,182,185,254,21,105,126,208,220,239,94,229,30,21,203,244,202,117,93,94,47,170,69,185,106,246,60,219,
            3,29,23,155,250,109,237,29,170,72,175,109,119,129,127,235,9,92,20,85,185,254,72,220,147,162,121,235,219,13,44,144,225,63,241,244,165,51,0,0
        };

        documentImage = loadDrawableFromData (drawableData, sizeof (drawableData));
    }

    return documentImage;
}


//========= linear slider ================================================================================
void CabbageLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{
    // g.fillAll (slider.findColour (Slider::backgroundColourId));

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        g.setColour(slider.findColour (Slider::thumbColourId));
        g.fillRoundedRectangle(x, y, width, height, 3);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

//========= bubble componenent that shows when we move a slider ====================
void CabbageLookAndFeel::drawBubble (Graphics& g, BubbleComponent& comp,
                                     const Point<float>& tip, const Rectangle<float>& body)
{
    Path p;
    p.addBubble (body.reduced (0.5f), body.getUnion (Rectangle<float> (tip.x, tip.y, 1.0f, 1.0f)),
                 tip, 5.0f, jmin (15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));

    g.setColour (comp.findColour (BubbleComponent::backgroundColourId));
    g.fillPath (p);

    g.setColour (comp.findColour (BubbleComponent::outlineColourId));
    g.strokePath (p, PathStrokeType (1.0f));
}

Font CabbageLookAndFeel::getSliderPopupFont (Slider&)
{
    return Font (15.0f, Font::bold);
}

int CabbageLookAndFeel::getSliderPopupPlacement (Slider&)
{
    return BubbleComponent::above
           | BubbleComponent::below
           | BubbleComponent::left
           | BubbleComponent::right;
}

//=========== Rotary Sliders ==============================================================================
void CabbageLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float diameter = radius*2.f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    bool useSliderBackgroundSVG = false;
    bool useSliderSVG = false;
    //if slider background svg exists...
    String svgPath = slider.getProperties().getWithDefault("svgpath", "");
    String svgSlider = slider.getProperties().getWithDefault("svgslider", "");
    String svgSliderBg = slider.getProperties().getWithDefault("svgsliderbg", "");
    int svgSliderWidthBg = slider.getProperties().getWithDefault("svgsliderbgwidth", 100);
    int svgSliderHeightBg = slider.getProperties().getWithDefault("svgsliderbgheight", 100);
    int svgSliderWidth = slider.getProperties().getWithDefault("svgsliderwidth", 100);
    int svgSliderHeight = slider.getProperties().getWithDefault("svgsliderheight", 100);

    //if valid background SVG file....
    if(svgSliderBg.length()>0)
    {
        g.drawImage(drawFromSVG(svgSliderBg, svgSliderWidthBg, svgSliderHeightBg, AffineTransform::identity), rx, ry,
                    diameter, diameter, 0, 0, svgSliderWidthBg, svgSliderHeightBg, false);
        useSliderBackgroundSVG = true;
    }

    slider.setSliderStyle(Slider::RotaryVerticalDrag);

    if (radius > 12.0f)
    {

        //tracker
        if(slider.findColour (Slider::trackColourId).getAlpha()==0)
            g.setColour(Colours::transparentBlack);
        else
            g.setColour (slider.findColour (Slider::trackColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));

        const float thickness = 0.7f;
        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
            g.fillPath (filledArc);
        }

        if(svgSlider.length()>0)
        {
            if(slider.findColour (Slider::trackColourId).getAlpha()==0)
                g.setColour(Colours::transparentBlack);
            else
                g.setColour (slider.findColour (Slider::trackColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
            const float thickness = slider.getProperties().getWithDefault("trackerthickness", .7);
            {
                Path filledArc;
                filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 1.f-thickness);
                g.fillPath (filledArc);
            }

            g.setOpacity(1.0);
            g.drawImage(drawFromSVG(svgSlider, svgSliderWidth, svgSliderHeight, AffineTransform::rotation(angle,
                                    svgRSliderDiameter/2, svgRSliderDiameter/2)), rx, ry, diameter, diameter,
                        0, 0, svgSliderWidth, svgSliderHeight, false);
            useSliderSVG = true;

        }
        else
            useSliderSVG = false;


        //outinecolour
        if(!useSliderBackgroundSVG)
        {
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));

            Path outlineArc;
            outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
            outlineArc.closeSubPath();

            g.strokePath (outlineArc, PathStrokeType (slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
        }
        if(!useSliderSVG)
        {
            Path newPolygon;
            Point<float> centre (centreX, centreY);

            if (diameter >= 25)   //If diameter is >= 40 then polygon has 12 steps
            {
                newPolygon.addPolygon(centre, 12.f, radius*.65, 0.f);
                newPolygon.applyTransform (AffineTransform::rotation (angle,
                                           centreX, centreY));
            }
            else //Else just use a circle. This is clearer than a polygon when very small.
                newPolygon.addEllipse (-radius*.2, -radius*.2, radius * .3f, radius * .3f);


            g.setColour (slider.findColour (Slider::thumbColourId));

            Colour thumbColour = slider.findColour (Slider::thumbColourId).withAlpha (isMouseOver ? 1.0f : 0.9f);
            ColourGradient cg = ColourGradient (Colours::white, 0, 0, thumbColour, diameter*0.6, diameter*0.4, false);
            if(slider.findColour (Slider::thumbColourId)!=Colour(0.f,0.f,0.f,0.f))
                g.setGradientFill (cg);
            g.fillPath (newPolygon);
        }
    }
    else
    {
        Path p;
        g.setColour (slider.findColour (Slider::thumbColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        g.fillPath(p, AffineTransform::rotation (angle).translated (centreX, centreY));

        //if (slider.isEnabled())
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId).withAlpha (isMouseOver ? 0.7f : 0.5f));
        //else
        //    g.setColour (Colour (0x80808080));

        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);

        p.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.1f);

        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));

    }

}

//=========== Linear Slider Background ===========================================================================
void CabbageLookAndFeel::drawLinearSliderBackground (Graphics &g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style,
        Slider &slider)
{

    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    float xOffset = (sliderRadius/width);
    const Colour trackColour (slider.findColour (Slider::trackColourId));
    float zeroPosProportional = 0;
    if (slider.getMinimum() < 0)
        zeroPosProportional = slider.valueToProportionOfLength(0); //takes into account skew factor

    const int useGradient = slider.getProperties().getWithDefault("gradient", 1);
    const float trackerThickness = slider.getProperties().getWithDefault("trackerthickness", .75);
    bool useSVG=false;

    String svgSliderBg = slider.getProperties().getWithDefault("svgsliderbg", "");
    int svgSliderWidthBg = slider.getProperties().getWithDefault("svgsliderbgwidth", 100);
    int svgSliderHeightBg = slider.getProperties().getWithDefault("svgsliderbgheight", 100);


    //if valid background SVG file....
    if(svgSliderBg.length()>0)
    {
        if(slider.isHorizontal())
            g.drawImage(drawFromSVG(svgSliderBg, svgSliderWidthBg, svgSliderHeightBg, AffineTransform::identity),
                        width*(xOffset/3), 0,  width*(1+xOffset*2), height, 0, 0, svgSliderWidthBg, svgSliderHeightBg, false);
        else
            g.drawImage(drawFromSVG(svgSliderBg, svgSliderWidthBg, svgSliderHeightBg, AffineTransform::identity),
                        0, 0, width, height+sliderRadius, 0, 0, svgSliderWidthBg, svgSliderHeightBg, false);

        useSVG  = true;
    }

    //bool useSVG = drawBackgroundForSVGSlider(g, &slider, "slider", x, y, width, height);

    Path indent;
    if (slider.isHorizontal())
    {
        if(!useSVG)
        {
            width = width-8;
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (width/2.f+sliderRadius)+3;
            const float markerGap = width/9.f;
            g.drawLine (midPoint, height*0.25, midPoint, height*0.75, 1.5);
            g.setOpacity (0.3);
            for (int i=1; i<5; i++)
            {
                g.drawLine (midPoint+markerGap*i, height*0.3, midPoint+markerGap*i, height*0.7, .7);
                g.drawLine (midPoint-markerGap*i, height*0.3, midPoint-markerGap*i, height*0.7, .7);
            }
            //backgrounds
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.1);
            g.fillRoundedRectangle (sliderRadius, height*0.44, width*1.021, height*0.15, height*0.05); //for light effect
            g.setColour (Colour::fromRGBA(5, 5, 5, 255));
            g.fillRoundedRectangle (sliderRadius, height*0.425, width*1.016, height*0.15, height*0.05); //main rectangle
        }

        const float scale = trackerThickness;
        const float ih = (height * scale);
        const float iy = ((height-ih)/2.f);

        //gradient fill for tracker...
        if(slider.getSliderStyle()==Slider::TwoValueHorizontal)
        {
            g.setColour(trackColour);
            const float minPos = slider.valueToProportionOfLength(slider.getMinValue())*width;
            const float maxPos = slider.valueToProportionOfLength(slider.getMaxValue())*width;
            indent.addRoundedRectangle(minPos + sliderRadius, iy, maxPos-minPos, ih, 5.0f);
        }
        else if(slider.getSliderStyle()==Slider::ThreeValueHorizontal)
        {
            g.setColour(trackColour);
            const float minPos = slider.valueToProportionOfLength(slider.getMinValue())*width;
            const float currentPos = slider.valueToProportionOfLength(slider.getValue())*width;
            indent.addRoundedRectangle(minPos + sliderRadius, iy, currentPos-minPos, ih, 5.0f);
        }
        else
        {
            if(useGradient)
            {
                if(slider.getMinimum()>=0)
                    g.setGradientFill(ColourGradient (Colours::transparentBlack, 0, 0, trackColour, width*0.25, 0, false));
                else
                    g.setGradientFill(ColourGradient(Colours::transparentBlack,
                                                     (slider.getValue()<= 0 ? zeroPosProportional*width*1.25 : zeroPosProportional*width),
                                                     0,
                                                     trackColour,
                                                     (slider.getValue()<= 0 ? 0 : width),
                                                     0,
                                                     false));
            }
            else
                g.setColour(trackColour);

            if(slider.getValue()>0)
                g.fillRoundedRectangle (zeroPosProportional*width + sliderRadius, iy,
                                        sliderPos - sliderRadius*0.5 - zeroPosProportional*width, ih,
                                        5.0f);
            else
                g.fillRoundedRectangle(sliderPos, iy,
                                       jmax(0.f, zeroPosProportional*width + sliderRadius - sliderPos), ih,
                                       5.0f);
        }

        if(!useSVG)
        {
            g.fillPath (indent);
            g.setColour (Colour (0x4c000000));
            g.strokePath (indent, PathStrokeType (0.3f));
        }

    }
    else //vertical
    {
        if(!useSVG)
        {
            height = height-8;
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (height/2.f+sliderRadius)+3;
            const float markerGap = height/9.f;
            g.drawLine (width*0.25, midPoint, width*0.75, midPoint, 1.59);
            g.setOpacity (0.3);

            for (int i=1; i<5; i++)
            {
                g.drawLine (width*0.3, midPoint+markerGap*i, width*0.7, midPoint+markerGap*i, .7);
                g.drawLine (width*0.3, midPoint-markerGap*i, width*0.7, midPoint-markerGap*i, .7);
            }

            g.setColour(Colours::whitesmoke);
            g.setOpacity (0.1);
            g.fillRoundedRectangle(width*0.44, sliderRadius, width*0.15, height*1.003, width*0.05);
            g.setColour (Colour::fromRGBA(5, 5, 5, 255));
            g.fillRoundedRectangle (width*0.425, sliderRadius, width*0.15, height*1.029, width*0.05);
        }

//        const float ix = x + width * 0.5f - sliderRadius * 0.25f;
//        const float iw = sliderRadius*.5f;

        const float scale = trackerThickness;
        const float iw = (width* scale);
        const float ix = ((width-iw)/2.f);


        if(slider.getSliderStyle()==Slider::TwoValueVertical)
        {
            g.setColour(trackColour);
            const float minPos = slider.valueToProportionOfLength(slider.getMinValue())*height;
            const float maxPos = slider.valueToProportionOfLength(slider.getMaxValue())*height;
            indent.addRoundedRectangle(ix, height-maxPos+sliderRadius*1.5f, iw, maxPos - minPos - sliderRadius*.5f, 3.0f);
        }
        else if(slider.getSliderStyle()==Slider::ThreeValueVertical)
        {
            g.setColour(trackColour);
            const float minPos = slider.valueToProportionOfLength(slider.getMinValue())*height;
            const float currentPos = slider.valueToProportionOfLength(slider.getValue())*height;
            indent.addRoundedRectangle(ix, height-currentPos+sliderRadius*.5f, iw, currentPos-minPos+sliderRadius, 3.0f);
        }
        else
        {
            if(useGradient)
            {
                if(slider.getMinimum()>=0)
                    g.setGradientFill(ColourGradient(Colours::transparentBlack, 0, height, trackColour, 0, height*0.8, false));
                else
                    g.setGradientFill(ColourGradient(Colours::transparentBlack,
                                                     0,
                                                     (slider.getValue()<= 0 ? zeroPosProportional*height : zeroPosProportional*height*1.25),
                                                     trackColour,
                                                     0,
                                                     (slider.getValue()<= 0 ? height : 0),
                                                     false));
            }
            else
                g.setColour(trackColour);


            if(slider.getValue()>=0)
                g.fillRoundedRectangle (ix, y + sliderPos - sliderRadius*2,
                                        iw, height - sliderPos + sliderRadius+1.5f - zeroPosProportional*height,
                                        3.0f);
            else
                g.fillRoundedRectangle (ix, zeroPosProportional*height+sliderRadius,
                                        iw, sliderPos - sliderRadius - zeroPosProportional*height,
                                        3.0f);
        }

        if(!useSVG)
        {
            g.fillPath (indent);
            g.setColour (Colour (0x4c000000));
            g.strokePath (indent, PathStrokeType (0.3f));
        }
    }

}


//========== Linear Slider Thumb =========================================================================
void CabbageLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    float sliderWidth, sliderHeight;
    bool useSVG=false;
    String svgSlider = slider.getProperties().getWithDefault("svgslider", "");
    int svgSliderWidth = slider.getProperties().getWithDefault("svgsliderwidth", 100);
    int svgSliderHeight = slider.getProperties().getWithDefault("svgsliderheight", 100);

    if(svgSlider.length()>0)
    {
        if(slider.isHorizontal())
        {
            sliderWidth = height;
            sliderHeight = height;
            g.drawImage(drawFromSVG(svgSlider, svgSliderWidth, svgSliderHeight, AffineTransform::identity),
                        sliderPos-width*.05, 0, sliderWidth, sliderHeight, 0, 0, svgHSliderThumb, svgHSliderThumb, false);
        }
        else
        {
            sliderWidth = width;
            sliderHeight = width;
            g.drawImage(drawFromSVG(svgSlider, svgSliderWidth, svgSliderHeight, AffineTransform::identity),
                        0, sliderPos-(height*.07), sliderWidth, sliderHeight, 0, 0, svgVSliderThumb, svgVSliderThumb, false);
        }
        useSVG  = true;
    }

    if(!useSVG)
    {
        Colour knobColour (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
                           slider.hasKeyboardFocus (false) && slider.isEnabled(),
                           slider.isMouseOverOrDragging() && slider.isEnabled(),
                           slider.isMouseButtonDown() && slider.isEnabled()));

        const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

        if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
        {
            float kx, ky;
            if (style == Slider::LinearVertical)
            {
                kx = x + width * 0.5f;
                ky = sliderPos;
                sliderWidth = sliderRadius * 2.0f;
                sliderHeight = sliderRadius * 1.5f;
            }
            else
            {
                kx = sliderPos;
                ky = y + height * 0.5f;
                sliderWidth = sliderRadius * 1.5f;
                sliderHeight = sliderRadius * 2.0f;
            }

            CabbageUtilities::drawSphericalThumb(g,
                                                 kx - sliderRadius,
                                                 ky - sliderRadius,
                                                 sliderWidth,
                                                 sliderHeight,
                                                 knobColour, outlineThickness);
        }
        else
        {
            if (style == Slider::ThreeValueVertical)
            {
                CabbageUtilities::drawSphericalThumb (g, x + width * 0.5f - sliderRadius,
                                                      sliderPos - sliderRadius,
                                                      sliderRadius * 2.0f,
                                                      sliderRadius * 2.0f,
                                                      knobColour, outlineThickness);
            }
            else if (style == Slider::ThreeValueHorizontal)
            {
                CabbageUtilities::drawSphericalThumb (g,sliderPos - sliderRadius * 0.75f,
                                                      y + height * 0.5f - sliderRadius,
                                                      sliderRadius * 1.5f,
                                                      sliderRadius * 2.0f,
                                                      knobColour, outlineThickness);
            }

            if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
            {
                const float sr = jmin (sliderRadius, width * 0.4f);
                CabbageUtilities::drawGlassPointer (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
                                                    minSliderPos - sliderRadius,
                                                    sliderRadius * 2.0f, knobColour, outlineThickness, 1);

                CabbageUtilities::drawGlassPointer (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                                                    sliderRadius * 2.0f, knobColour, outlineThickness, 3);
            }
            else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
            {
                const float sr = jmin (sliderRadius, height * 0.4f);
                CabbageUtilities::drawGlassPointer (g, minSliderPos - sr,
                                                    jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f),
                                                    sliderRadius * 2.0f, knobColour, outlineThickness, 2);

                CabbageUtilities::drawGlassPointer (g, maxSliderPos - sliderRadius,
                                                    jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f),
                                                    sliderRadius * 2.0f, knobColour, outlineThickness, 4);
            }
        }
    }
}


//======= Toggle Buttons ========================================================================
void CabbageLookAndFeel::drawToggleButton (Graphics &g, ToggleButton &button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
{
    int destWidth, destHeight;
    destHeight = button.getHeight();
    if (button.getButtonText().length() > 0)
        destWidth = destHeight;
    else
        destWidth = button.getWidth();

    int destX = 0;
    int destY = 0;
    bool isToggleOn;

    if (button.getToggleState() == true)
        isToggleOn = true;
    else
        isToggleOn = false;

    bool isRECT = button.getProperties().getWithDefault("isRect", 0);

    float corner = button.getProperties().getWithDefault("cornersize", 5);

    String imgButtonOn = button.getProperties().getWithDefault("imgbuttonon", "");
    String imgButtonOff = button.getProperties().getWithDefault("imgbuttonoff", "");

    int imgWidth = button.getProperties().getWithDefault("imgbuttonwidth", 100);
    int imgHeight = button.getProperties().getWithDefault("imgbuttonheight", 100);

    //----- Creating the image

    //CabbageUtilities::debug(button.findColour(TextButton::buttonColourId).toString());

    Image newButton;
    if(!button.getToggleState())
    {
        if(imgButtonOn.isNotEmpty())
            newButton = drawTextButtonImage(destWidth, destHeight, button.getToggleState(), button.findColour(TextButton::buttonColourId), imgButtonOn, imgWidth, imgHeight, false);
        else
            newButton = drawToggleImage (destWidth, destHeight, true, button.findColour(TextButton::buttonColourId), isRECT, "", corner);
    }
    else
    {
        if(imgButtonOff.isNotEmpty())
            newButton = drawTextButtonImage (destWidth, destHeight, button.getToggleState(), button.findColour(TextButton::buttonOnColourId), imgButtonOff, imgWidth, imgHeight, true);
        else
            newButton = drawToggleImage (destWidth, destHeight, true, button.findColour(TextButton::buttonOnColourId), isRECT, "", corner);
    }


    //----- Drawing image
    g.drawImage (newButton, destX, destY, destWidth, destHeight, 0, 0, destWidth, destHeight, false);

    //----- Text
    if (button.getButtonText().length() > 0)
    {
        Justification just (1); //left
        //g.setFont (CabbageUtilities::getComponentFont());
        if(isToggleOn)
            g.setColour (button.findColour(TextButton::textColourOnId));
        else
            g.setColour (button.findColour(TextButton::textColourOffId));
        //g.setColour(Colours::white);
        String name;
        name << button.getButtonText();
        name = CabbageUtilities::cabbageString (name, CabbageUtilities::getComponentFont(), (button.getWidth()-(destWidth+5))); //shortening string if too long

        g.drawText (name, destWidth+5, destY, button.getWidth(), button.getHeight(), just, false);
    }
}

Image CabbageLookAndFeel::drawFromSVG(String svgString, int width, int height, AffineTransform affine)
{
    Image svgImg;
    svgImg = Image(Image::ARGB, width, height, true);
    ScopedPointer<XmlElement> svg (XmlDocument::parse(svgString));
    if(svg == nullptr)
        return Image::null;

    ScopedPointer<Drawable> drawable;

    Graphics graph(svgImg);
    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG (*svg);
        drawable->draw(graph, 1.f, affine);
        return svgImg;
    }
    return Image::null;
}

Image CabbageLookAndFeel::drawFromPNG(String pngString, int width, int height, AffineTransform affine)
{
    Image img = ImageCache::getFromFile(File(pngString));
    img.rescaled(width, height);
    return img;

}

Image CabbageLookAndFeel::drawTextButtonImage(float width, float height, bool isButtonDown, Colour colour,
        String imgFile, int imgWidth, int imgHeight, bool on)
{
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);
    float opacity;

    //if alpha is full draw invible button
    if(colour.getAlpha()==0x00)
    {
        g.fillAll(Colours::transparentBlack);
        return img;
    }


    if(imgFile.length()>0)
    {
        //----- If "off"
        if (on == false)
        {
            if(imgFile.containsIgnoreCase(".svg"))
                g.drawImage(drawFromSVG(imgFile, imgWidth, imgHeight, AffineTransform::identity), 0, 0, width, height, 0, 0, imgWidth, imgHeight, false);
            else
                g.drawImage(drawFromPNG(imgFile, imgWidth, imgHeight, AffineTransform::identity), 0, 0, width, height, 0, 0, imgWidth, imgHeight, false);
        }
        else
        {
            if(imgFile.containsIgnoreCase(".svg"))
                g.drawImage(drawFromSVG(imgFile, imgWidth, imgHeight, AffineTransform::identity), 1, 1, width-2, height-2, 0, 0, imgWidth, imgHeight, false);
            else
                g.drawImage(drawFromPNG(imgFile, imgWidth, imgHeight, AffineTransform::identity), 0, 0, width, height, 0, 0, imgWidth, imgHeight, false);

        }
    }
    else
    {

        //----- Outline
        g.setColour (Colour::fromRGBA (10, 10, 10, 255));
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);

        //----- If "off"
        if (isButtonDown == false)
        {
            //----- Shadow
            for (float i=0.01; i<0.05; i+=0.01)
            {
                g.setColour (Colour::fromRGBA (0, 0, 0, 255/(i*100)));
                g.fillRoundedRectangle (width*i, height*i,
                                        width*0.95, height*0.95, height*0.1);
                opacity = 0.3;
            }
        }
        else
            opacity = 0.1;

        //----- Filling in the button
        //Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
        //Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
        Colour bg1 = colour;
        Colour bg2 = colour.darker();

        ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
        g.setGradientFill (cg);
        g.fillRoundedRectangle (width*0.01, height*0.01, width*0.93, height*0.93, height*0.1);

        //----- For emphasising the top and left edges to give the illusion that light is shining on them
        ColourGradient edgeHighlight = ColourGradient (Colours::whitesmoke, 0, 0,
                                       Colours::transparentWhite, 0, height*0.1, false);
        g.setGradientFill (edgeHighlight);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);

        ColourGradient edgeHighlight2 = ColourGradient (Colours::whitesmoke, 0, 0,
                                        Colours::transparentWhite, height*0.1, 0, false);
        g.setGradientFill (edgeHighlight2);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);
    }
    return img;
}

Image CabbageLookAndFeel::drawToggleImage (float width, float height, bool isToggleOn, Colour colour, bool isRect, String imgPath, float corners)
{
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);
    float opacity = 0;

    //if alpha is full draw invible button
    if(colour.getAlpha()==0x00)
    {
        g.fillAll(Colours::transparentBlack);
        return img;
    }

    if (isRect)   //if rectangular toggle
    {
        g.setColour (Colour::fromRGBA (10, 10, 10, 255));
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);

        if (isToggleOn == true)
        {
            g.setColour (colour);
            g.fillRoundedRectangle(width*0.01, height*0.01, width*0.93, height*0.93, corners);
            opacity = 0.4;
        }
        else   //off
        {
            // Shadow
            for (float i=0.01; i<0.05; i+=0.01)
            {
                g.setColour (Colour::fromRGBA (0, 0, 0, 255/(i*100)));
                g.fillRoundedRectangle (width*i+1, height*i+1,
                                        width*0.95, height*0.95, corners);
            }
            // Filling in the button
            Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
            Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
            ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
            g.setGradientFill (cg);
            g.fillRoundedRectangle (width*0.01, height*0.01, width*0.93, height*0.93, corners);
            opacity = 0.2;
        }

        // For emphasising the top and left edges to give the illusion that light is shining on them
        ColourGradient edgeHighlight = ColourGradient (Colours::whitesmoke, 0, 0,
                                       Colours::transparentWhite, 0, height*0.1, false);
        g.setGradientFill (edgeHighlight);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);

        ColourGradient edgeHighlight2 = ColourGradient (Colours::whitesmoke, 0, 0,
                                        Colours::transparentWhite, height*0.1, 0, false);
        g.setGradientFill (edgeHighlight2);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);
    }
    else   //else if round toggle
    {
        //base
        ColourGradient base = ColourGradient (Colours::white, width*-0.3, height*-0.3, Colours::black,
                                              width*0.8, height*0.8, false);
        g.setGradientFill(base);
        g.fillEllipse (0, 0, width, height);

        g.setColour(Colours::black);
        g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);

        Colour outline = Colour::fromRGB(70, 70, 70);

        g.setColour(outline.withAlpha(colour.getAlpha()));
        g.fillEllipse(width*0.04, height*0.04, width*0.92, height*0.92);

        if (isToggleOn)   //on
        {
//                ColourGradient cg = ColourGradient(colour.withSaturation(0.2), width*0.4, height*0.4, colour,
//                                                   width*0.8, height*0.8, true);

            //g.setGradientFill (cg);
            g.setColour(colour);
            g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);
        }
        else   //off
        {
            g.setColour(Colours::black);
            g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);

            Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
            Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
            ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);


            //ColourGradient cg = ColourGradient (Colours::white, width*0.4, height*0.4, colour.darker(0.9), width*0.3, height*0.3, true);
            g.setGradientFill (cg);
            g.setOpacity(0.4);
            g.fillEllipse(width*0.1, height*0.1, width*0.8, height*0.8);
        }
    }
    return img;
}
//========= Text Button Background ======================================================================
void CabbageLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& /*backgroundColour*/,
        bool /*isButtonOver*/,
        bool isButtonDown)
{
    Image newButton;
    float width = button.getWidth();
    float height = button.getHeight();

    String svgButtonOn = button.getProperties().getWithDefault("svgbuttonon", "");
    String svgButtonOff = button.getProperties().getWithDefault("svgbuttonoff", "");

    int svgWidth = button.getProperties().getWithDefault("svgbuttonwidth", 100);
    int svgHeight = button.getProperties().getWithDefault("svgbuttonheight", 100);


    if(!button.getToggleState())
    {
        newButton = drawTextButtonImage(width, height, isButtonDown, button.findColour(TextButton::buttonColourId), svgButtonOff, svgWidth, svgHeight, false);
    }
    else
    {
        newButton = drawTextButtonImage (width, height, isButtonDown, button.findColour(TextButton::buttonOnColourId), svgButtonOn, svgWidth, svgHeight, true);
    }
    g.drawImage (newButton, 0, 0, width, height, 0, 0, width, height, false);
}

//======= Text Button text =========================================================================
void CabbageLookAndFeel::drawButtonText (Graphics &g, TextButton &button, bool isMouseOverButton,
        bool isButtonDown)
{
    float width = button.getWidth();
    float height = button.getHeight();
    float destX;
    float destWidth = width*0.8;
    Font font = CabbageUtilities::getComponentFont();
    String text;
    text << button.getButtonText();
    text = CabbageUtilities::cabbageString (text, font, destWidth);
    Justification just (36); //centered
    float destY = (height*0.5) - (font.getHeight()*0.5);

    Colour textColour;
    if(!button.getToggleState())
        textColour = button.findColour(TextButton::textColourOffId);
    else
        textColour = button.findColour(TextButton::textColourOnId);

    //----- The text colour and start position change if the button is down or up....
    if (isButtonDown == true)
    {
        g.setColour (textColour.brighter(0.3));
        destX = width*0.095;
    }
    else
    {
        g.setColour (textColour);
        destX = width*0.1;
    }

    g.setFont (font);
    g.drawText (text, destX, destY, destWidth, font.getHeight(),
                just, false);
}

//=========== ComboBoxes ============================================================================
void CabbageLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool /*isButtonDown*/,
                                      int /*buttonX*/,
                                      int /*buttonY*/,
                                      int /*buttonW*/,
                                      int /*buttonH*/,
                                      ComboBox& box)
{
    // The box that contains the arrow
    g.setColour(CabbageUtilities::getComponentFontColour());
    float arrowBoxWidth;
    if (width >= 40)
        arrowBoxWidth = 20;
    else
        arrowBoxWidth = width/2;

    // Main bg
    g.setColour(box.findColour(ComboBox::backgroundColourId));
    g.fillRoundedRectangle (0, 0, width, height, height*0.1);

    // Border outline
    g.setColour(CabbageUtilities::getBorderColour());
    float borderWidth = CabbageUtilities::getBorderWidth();
    g.drawRoundedRectangle (borderWidth/2, borderWidth/2, width-borderWidth,
                            height-borderWidth, height*0.1, borderWidth);

    // Arrow
    g.setColour(box.findColour(ComboBox::textColourId));
    const Line<float> line(width-(arrowBoxWidth/2), height*0.3, width-(arrowBoxWidth/2), height*0.7);
    g.drawArrow(line, 0, arrowBoxWidth*0.4, height*0.4);
}

Font CabbageLookAndFeel::getComboBoxFont (ComboBox& box)
{
    return Font(jmin (15.0f, box.getHeight() * 0.85f), 1);
}

Label* CabbageLookAndFeel::createComboBoxTextBox (ComboBox&)
{
    return new Label (String::empty, String::empty);
}

void CabbageLookAndFeel::positionComboBoxText (ComboBox& box, Label& label)
{
    label.setBounds (1, 1,
                     box.getWidth() + 3 - box.getHeight(),
                     box.getHeight() - 2);

    label.setFont (getComboBoxFont (box));
}


//============= TextEditor Outline ======================================================================
void CabbageLookAndFeel::drawTextEditorOutline (Graphics &g, int width, int height, TextEditor &/*textEditor*/)
{

    g.setColour (Colours::transparentBlack);
    g.fillAll();
    //g.drawRoundedRectangle (0, 0, width, height, height/3, 1);
}



//============= TextEditor Background ==============================================================
void CabbageLookAndFeel::fillTextEditorBackground (Graphics &g, int width, int height, TextEditor &textEditor)
{
    textEditor.setColour (0x1000200, textEditor.findColour(TextEditor::backgroundColourId)); //this gets rid of the square rectangle background
    textEditor.setColour(TextEditor::textColourId, textEditor.findColour(TextEditor::textColourId));

    g.setColour (textEditor.findColour(TextEditor::backgroundColourId));
    if(textEditor.findColour(Label::outlineColourId)==Colours::white)
        g.fillRect(0, 0, width, height);
    else
        g.fillRoundedRectangle (0, 0, width, height, 5);
}


//=========== Labels, slider textboxes are also labels =============================================
void CabbageLookAndFeel::drawLabel (Graphics &g, Label &label)
{
    g.fillAll (Colours::transparentBlack);

    if (! label.isBeingEdited())
    {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const Font font (getLabelFont (label));

        g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);

        Rectangle<int> textArea (label.getBorderSize().subtractedFrom (label.getLocalBounds()));

        g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                          jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                          label.getMinimumHorizontalScale());

        g.setColour (label.findColour (Label::outlineColourId));
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (Label::outlineColourId));
    }

    g.drawRoundedRectangle(label.getLocalBounds().toFloat(), 4.f, 1.f);
}


//======== Group Components ======================================================================
void CabbageLookAndFeel::drawGroupComponentOutline (Graphics &g, int w, int h, const String &text,
        const Justification &position,
        GroupComponent &group)
{
    g.fillAll(Colours::transparentBlack);
    String svgFile = group.getProperties().getWithDefault("svggroupbox", "");
    int svgWidth = group.getProperties().getWithDefault("svggroupboxwidth", 100);
    int svgHeight = group.getProperties().getWithDefault("svggroupboxheight", 100);

    //if valid SVG file....
    if(svgFile.length()>0)
        g.drawImage(drawFromSVG(svgFile, svgWidth, svgHeight, AffineTransform::identity), 0, 0,
                    w, h, 0, 0, svgGroupboxWidth, svgGroupboxHeight, false);

    else
    {

        int corners = group.getProperties().getWithDefault("cornersize", 5);
        Colour col;
        g.setColour (group.findColour(TextButton::buttonColourId));
        g.fillRoundedRectangle (0, 0, w, h, corners);

        //----- Outline
        g.setColour (CabbageUtilities::getComponentFontColour());
        g.setOpacity (0.1);
        g.drawRoundedRectangle (0.5, 0.5, w-1, h-1, corners, 1);

        g.setColour (group.findColour(GroupComponent::textColourId));

        float lineThickness = group.getProperties().getWithDefault("groupLine", 0);

        if(lineThickness>0)
        {
            g.drawLine (10, 20, w-10, 20, lineThickness);
        }
    }

    //----- Text
    String name = group.getText();
    Font font = CabbageUtilities::getTitleFont();
#ifndef MACOSX
    font.setFallbackFontName("Verdana");
#endif
    g.setFont (font);

    g.setColour (group.findColour(GroupComponent::textColourId));
    name = CabbageUtilities::cabbageString (name, font, group.getWidth());
    g.drawText (name, 0, 5, w, font.getHeight(), 36, false);
}

//======== Scrollbars ==============================================================================
void CabbageLookAndFeel::drawScrollbar (Graphics &g, ScrollBar &scrollbar, int x, int y, int width,
                                        int height,
                                        bool isScrollbarVertical,
                                        int thumbStartPosition,
                                        int thumbSize,
                                        bool isMouseOver,
                                        bool isMouseDown)
{
    g.setColour (Colours::transparentBlack);
    g.fillAll();

    g.setColour (CabbageUtilities::getComponentSkin().withAlpha(.5f));
    g.drawRect (x, y, width, height);

    if (isScrollbarVertical == false) //horizontal
        g.fillRoundedRectangle (thumbStartPosition+3, 3, jmax(0, thumbSize-6), height-6, 5);
    else //vertical
        g.fillRoundedRectangle (3, thumbStartPosition+3, width-6, jmax(1, thumbSize-6), 5);

}

//======= get deafult Scrollbar buttons =======================================================================
int CabbageLookAndFeel::getDefaultScrollbarWidth()
{
    return 18;
}

//======= Scrollbar buttons =======================================================================
void CabbageLookAndFeel::drawScrollbarButton (Graphics &g, ScrollBar &scrollbar, int width, int height,
        int buttonDirection,
        bool isScrollbarVertical,
        bool isMouseOverButton,
        bool isButtonDown)
{
    g.setColour (Colours::transparentBlack.withAlpha(0.f));
    g.fillAll();

    if (isButtonDown == true)
        g.setColour (CabbageUtilities::getComponentSkin().brighter(0.5f));
    else
        g.setColour (CabbageUtilities::getComponentSkin());

    if (buttonDirection == 3)   //left arrow
    {
        Line<float> left (width*0.8, height/2, width*0.2, height/2);
        g.drawArrow (left, 0, 10, 10);
    }
    else if (buttonDirection == 1)   //right arrow
    {
        Line<float> right (width*0.2, height/2, width*0.8, height/2);
        g.drawArrow (right, 0, 10, 10);
    }
    else if (buttonDirection == 0)   //up arrow
    {
        Line<float> up (width/2, height*0.8, width/2, height*0.2);
        g.drawArrow (up, 0, 10, 10);
    }
    else if (buttonDirection == 2)   //down arrow
    {
        Line<float> down (width/2, height*0.2, width/2, height*0.8);
        g.drawArrow (down, 0, 10, 10);
    }


}

//======== Popup Menu background ======================================================================
void CabbageLookAndFeel::drawPopupMenuBackground(Graphics &g, int width, int height)
{
    g.setColour (findColour(PopupMenu::backgroundColourId));
    g.fillAll();
    g.setColour (findColour(PopupMenu::backgroundColourId));
    g.drawRect (0, 0, width, height, 1); //dont want to see top line
}

//====== Returns image of a check mark ==============================================
Image CabbageLookAndFeel::drawCheckMark()
{
    Image img = Image(Image::ARGB, 10, 10, true);
    Graphics g(img);

    Path path;
    path.startNewSubPath(3, 7);
    path.lineTo(5, 10);
    path.lineTo(10, 0);
    g.setColour (Colours::cornflowerblue);
    g.strokePath(path, PathStrokeType(2.0f));

    return img;
}

//======== Popup Menu Items ===========================================================================
void CabbageLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const String& text,
        const String& shortcutKeyText,
        const Drawable* icon, const Colour* const textColourToUse)
{
    Colour textColour;
    if ((isHighlighted == true) && (isSeparator == false))
    {
        g.setColour (CabbageUtilities::getComponentSkin());
        g.fillAll();
        g.setColour (Colours::cornflowerblue);
    }
    else
    {
        if (textColourToUse != nullptr)
        {
            textColour = *textColourToUse;
            g.setColour(textColour);
        }
        else
            g.setColour (CabbageUtilities::getComponentFontColour());
    }


    g.setFont (CabbageUtilities::getComponentFont());
    g.drawText (CabbageUtilities::cabbageString(text, CabbageUtilities::getComponentFont(), area.getWidth()*0.8), 20, 0, area.getWidth()*0.8, area.getHeight(), 1, false);

    if (isSeparator == true)
    {
        g.setColour(CabbageUtilities::getComponentSkin());
        g.drawLine(0, area.getHeight()/2,  area.getWidth(), 3);
    }

    if (isTicked)
    {
        Image checkMark = drawCheckMark();
        g.setColour(Colours::cornflowerblue);
        g.drawImage(checkMark, 5, (area.getHeight()/2)-5, 10, 10, 0, 0, 10, 10, false);
    }
    if (hasSubMenu)
    {
        g.setColour(Colours::cornflowerblue);
        const Line<float> line( area.getWidth()-15, area.getHeight()*.5,  area.getWidth()-5, area.getHeight()*.5);
        g.drawArrow(line, 0, area.getHeight()*.3, area.getHeight()*.3);
    }
    if (shortcutKeyText.isNotEmpty())
    {
        const int leftBorder = (area.getHeight() * 5) / 4;
        const int rightBorder = 4;

        g.drawText (shortcutKeyText,
                    leftBorder, 0,  area.getWidth() - (leftBorder + rightBorder + 4), area.getHeight(),
                    Justification::centredRight,
                    true);
    }

}

//======== Menubar background ======================================================================
void CabbageLookAndFeel::drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar)
{
    g.setColour (CabbageUtilities::getBackgroundSkin());
    g.fillAll();
}

//======== Menubar item background ======================================================================
void CabbageLookAndFeel::drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
        const String &itemText,
        bool isMouseOverItem,
        bool isMenuOpen,
        bool isMouseOverBar,
        MenuBarComponent &menuBar)
{
    if ((isMouseOverItem == true) || (isMenuOpen == true))
    {
        g.setColour (CabbageUtilities::getBackgroundSkin().darker (0.2));
        g.fillRect (0, 0, width, height);
    }

    g.setColour (CabbageUtilities::getComponentFontColour());
    g.setFont (CabbageUtilities::getComponentFont());
    g.drawFittedText(CabbageUtilities::cabbageString(itemText, CabbageUtilities::getComponentFont(), width*0.9), 5, 0, width, height, 36, 1);
}

//======== Document Window title bar ===================================================================
void CabbageLookAndFeel::drawDocumentWindowTitleBar (DocumentWindow &window, Graphics &g, int w, int h,
        int /*titleSpaceX*/,
        int titleSpaceW,
        const Image */*icon*/,
        bool /*drawTitleTextOnLeft*/)
{
    window.setUsingNativeTitleBar(false);
    Colour background = Colour::fromString(window.getProperties().getWithDefault("colour", "").toString());
    String fontcolour = window.getProperties().getWithDefault("fontcolour", "").toString();
    String titlebarcolour = window.getProperties().getWithDefault("titlebarcolour", "").toString();
#ifdef CABBAGE_HOST
    window.setOpaque(true);
    g.setColour (background);
    g.fillRoundedRectangle(0, 0, w, h+20, 10);
#else
    g.setColour (background);
    g.fillAll();

    if(titlebarcolour.length()>1)
    {
        g.setColour (Colour::fromString(titlebarcolour));
        g.fillRect(0, 0, w, h+22);

    }
    g.fillRoundedRectangle(0, 0, w, h+20, 10);

#endif

    if(fontcolour.isNotEmpty())
        g.setColour (Colour::fromString(fontcolour));
    else
        g.setColour (CabbageUtilities::getComponentFontColour());
    Font font = CabbageUtilities::getTitleFont();
#ifndef MACOSX
    font.setFallbackFontName("Verdana");
#endif
    font.setHeight(16);
    g.setFont (font);
    g.drawText (CabbageUtilities::cabbageString(window.getName(), font, titleSpaceW), (w/2)-(titleSpaceW/2),
                (h/2)-(font.getHeight()/2), titleSpaceW, font.getHeight(), 36, false);
}

//====== Draw Window Button Normal Image =================================================================
Image CabbageLookAndFeel::drawWindowButtonNormal(int buttonType)
{
    int width, height;
    width = height = 20;
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);

    String str;

    if (buttonType == 1)
        str << "_";
    else if (buttonType == 2)
        str << "+";
    else if (buttonType == 4)
        str << "x";

    //----- Background
    g.setColour (Colours::transparentBlack);
    g.fillRoundedRectangle(1, 1, width-2, height-2, 2);

    //----- Text symbol
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    g.setColour (CabbageUtilities::getComponentFontColour());
    g.drawText(str, (width/2) - (font.getHeight()/2), (width/2) - (font.getHeight()/2),
               font.getHeight(), font.getHeight(), 36, false);

    return img;
}

//====== Draw Window Button Image Is Over ==============================================================
Image CabbageLookAndFeel::drawWindowButtonIsOver(int buttonType)
{
    int width, height;
    width = height = 20;
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);

    String str;

    if (buttonType == 1)
        str << "_";
    else if (buttonType == 2)
        str << "+";
    else if (buttonType == 4)
        str << "x";

    //---- Background
    g.setColour (Colours::transparentBlack);
    g.fillRoundedRectangle(1, 1, width-2, height-2, 2);

    //---- Text symbol
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    g.setColour (Colours::whitesmoke);
    g.drawText(str, (width/2) - (font.getHeight()/2), (width/2) - (font.getHeight()/2),
               font.getHeight(), font.getHeight(), 36, false);

    //---- V lines
    ColourGradient cg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                       0, height, false);
    cg.addColour (0.5, CabbageUtilities::getComponentFontColour());
    g.setGradientFill (cg);
    g.drawLine (0, 0, 0, height, 1);
    g.drawLine (width, 0, width, height, 1);

    return img;
}

//========= Document Window Buttons =======================================================================
Button* CabbageLookAndFeel::createDocumentWindowButton (int buttonType)
{
    ImageButton* button;
    button = new ImageButton("Button");
    Image normalImage, isOverImage;

    normalImage = drawWindowButtonNormal(buttonType);
    isOverImage = drawWindowButtonIsOver(buttonType);

    button->setImages(true, false, true, normalImage, 1, Colours::transparentBlack, isOverImage,
                      1, Colours::transparentBlack, isOverImage, 1, Colours::transparentBlack, 0);


    return button;
}

//============= alert windows methods ===================================
AlertWindow* CabbageLookAndFeel::createAlertWindow (const String& title,
        const String& message,
        const String& button1,
        const String& button2,
        const String& button3,
        AlertWindow::AlertIconType iconType,
        int numButtons,
        Component* associatedComponent)
{
    AlertWindow* aw = new AlertWindow (title, message, iconType, associatedComponent);

    if (numButtons == 1)
    {
        aw->addButton (button1, 0,
                       KeyPress (KeyPress::escapeKey),
                       KeyPress (KeyPress::returnKey));
    }
    else
    {
        const KeyPress button1ShortCut ((int) CharacterFunctions::toLowerCase (button1[0]), 0, 0);
        KeyPress button2ShortCut ((int) CharacterFunctions::toLowerCase (button2[0]), 0, 0);
        if (button1ShortCut == button2ShortCut)
            button2ShortCut = KeyPress();

        if (numButtons == 2)
        {
            aw->addButton (button1, 1, KeyPress (KeyPress::returnKey), button1ShortCut);
            aw->addButton (button2, 0, KeyPress (KeyPress::escapeKey), button2ShortCut);
        }
        else if (numButtons == 3)
        {
            aw->addButton (button1, 1, button1ShortCut);
            aw->addButton (button2, 2, button2ShortCut);
            aw->addButton (button3, 0, KeyPress (KeyPress::escapeKey));
        }
    }

    return aw;
}

void CabbageLookAndFeel::drawAlertBox (Graphics& g,
                                       AlertWindow& alert,
                                       const Rectangle<int>& textArea,
                                       TextLayout& textLayout)
{
    g.fillAll (alert.findColour (AlertWindow::backgroundColourId));
    g.fillAll (Colours::black);
    int iconSpaceUsed = 0;

    const int iconWidth = 80;
    int iconSize = jmin (iconWidth + 50, alert.getHeight() + 20);

    if (alert.containsAnyExtraComponents() || alert.getNumButtons() > 2)
        iconSize = jmin (iconSize, textArea.getHeight() + 50);

    const Rectangle<int> iconRect (iconSize / -10, iconSize / -10,
                                   iconSize, iconSize);

    if (alert.getAlertType() != AlertWindow::NoIcon)
    {
        Path icon;
        uint32 colour;
        char character;

//        if (alert.getAlertType() == AlertWindow::WarningIcon)
//        {
//        Image logo = ImageCache::getFromMemory (BinaryData::logo_cabbage_Black_png, BinaryData::logo_cabbage_Black_pngSize);
//        g.setOpacity(.2f);
//        g.drawImage(logo, -300, -100, 600, 500, 0, 0, logo.getWidth(), logo.getHeight());
//
//        }
//        else
//        {
//            colour    = alert.getAlertType() == AlertWindow::InfoIcon ? (uint32) 0x605555ff : (uint32) 0x40b69900;
//            character = alert.getAlertType() == AlertWindow::InfoIcon ? 'i' : '?';
//
//            icon.addEllipse ((float) iconRect.getX(), (float) iconRect.getY(),
//                             (float) iconRect.getWidth(), (float) iconRect.getHeight());
//
//        }

        iconSpaceUsed = iconWidth;
    }

    g.setColour (Colours::pink);//alert.findColour (AlertWindow::textColourId));

    textLayout.draw (g, Rectangle<int> (textArea.getX() + iconSpaceUsed,
                                        textArea.getY(),
                                        textArea.getWidth() - iconSpaceUsed,
                                        textArea.getHeight()).toFloat());

    g.setColour (Colours::white);
    g.drawRect(0.0, 0.0, float(alert.getWidth()), float(alert.getHeight()), .5f);
}

int CabbageLookAndFeel::getAlertBoxWindowFlags()
{
    return ComponentPeer::windowAppearsOnTaskbar
           | ComponentPeer::windowHasDropShadow;
}

int CabbageLookAndFeel::getAlertWindowButtonHeight()
{
    return 28;
}

Font CabbageLookAndFeel::getAlertWindowMessageFont()
{
    return Font (15.0f);
}

Font CabbageLookAndFeel::getAlertWindowFont()
{
    return Font (12.0f);
}

//tabbed component methods
//==============================================================================
int CabbageLookAndFeel::getTabButtonOverlap (int tabDepth)
{
    return 1 + tabDepth / 3;
}

int CabbageLookAndFeel::getTabButtonSpaceAroundImage()
{
    return 4;
}

int CabbageLookAndFeel::getTabButtonBestWidth (TabBarButton& button, int tabDepth)
{
    int width = Font (tabDepth * 0.6f).getStringWidth (button.getButtonText().trim())
                + getTabButtonOverlap (tabDepth) * 2;

    if (Component* const extraComponent = button.getExtraComponent())
        width += button.getTabbedButtonBar().isVertical() ? extraComponent->getHeight()
                 : extraComponent->getWidth();

    return jlimit (tabDepth * 2, tabDepth * 8, width);
}

Rectangle<int> CabbageLookAndFeel::getTabButtonExtraComponentBounds (const TabBarButton& button, Rectangle<int>& textArea, Component& comp)
{
    Rectangle<int> extraComp;

    const TabbedButtonBar::Orientation orientation = button.getTabbedButtonBar().getOrientation();

    if (button.getExtraComponentPlacement() == TabBarButton::beforeText)
    {
        switch (orientation)
        {
        case TabbedButtonBar::TabsAtBottom:
        case TabbedButtonBar::TabsAtTop:
            extraComp = textArea.removeFromLeft   (comp.getWidth());
            break;
        case TabbedButtonBar::TabsAtLeft:
            extraComp = textArea.removeFromBottom (comp.getHeight());
            break;
        case TabbedButtonBar::TabsAtRight:
            extraComp = textArea.removeFromTop    (comp.getHeight());
            break;
        default:
            jassertfalse;
            break;
        }
    }
    else
    {
        switch (orientation)
        {
        case TabbedButtonBar::TabsAtBottom:
        case TabbedButtonBar::TabsAtTop:
            extraComp = textArea.removeFromRight  (comp.getWidth());
            break;
        case TabbedButtonBar::TabsAtLeft:
            extraComp = textArea.removeFromTop    (comp.getHeight());
            break;
        case TabbedButtonBar::TabsAtRight:
            extraComp = textArea.removeFromBottom (comp.getHeight());
            break;
        default:
            jassertfalse;
            break;
        }
    }

    return extraComp;
}

void CabbageLookAndFeel::createTabButtonShape (TabBarButton& button, Path& p, bool /*isMouseOver*/, bool /*isMouseDown*/)
{
    const Rectangle<int> activeArea (button.getActiveArea());
    const float w = (float) activeArea.getWidth();
    const float h = (float) activeArea.getHeight();

    float length = w;
    float depth = h;

    if (button.getTabbedButtonBar().isVertical())
        std::swap (length, depth);

    const float indent = (float) getTabButtonOverlap ((int) depth);
    const float overhang = 4.0f;

    switch (button.getTabbedButtonBar().getOrientation())
    {
    case TabbedButtonBar::TabsAtLeft:
        p.startNewSubPath (w, 0.0f);
        p.lineTo (0.0f, indent);
        p.lineTo (0.0f, h - indent);
        p.lineTo (w, h);
        p.lineTo (w + overhang, h + overhang);
        p.lineTo (w + overhang, -overhang);
        break;

    case TabbedButtonBar::TabsAtRight:
        p.startNewSubPath (0.0f, 0.0f);
        p.lineTo (w, indent);
        p.lineTo (w, h - indent);
        p.lineTo (0.0f, h);
        p.lineTo (-overhang, h + overhang);
        p.lineTo (-overhang, -overhang);
        break;

    case TabbedButtonBar::TabsAtBottom:
        p.startNewSubPath (0.0f, 0.0f);
        p.lineTo (indent, h);
        p.lineTo (w - indent, h);
        p.lineTo (w, 0.0f);
        p.lineTo (w + overhang, -overhang);
        p.lineTo (-overhang, -overhang);
        break;

    default:
        p.startNewSubPath (0.0f, h);
        p.lineTo (indent, 0.0f);
        p.lineTo (w - indent, 0.0f);
        p.lineTo (w, h);
        p.lineTo (w + overhang, h + overhang);
        p.lineTo (-overhang, h + overhang);
        break;
    }

    p.closeSubPath();

    p = p.createPathWithRoundedCorners (3.0f);
}

void CabbageLookAndFeel::fillTabButtonShape (TabBarButton& button, Graphics& g, const Path& path,
        bool /*isMouseOver*/, bool /*isMouseDown*/)
{
    const Colour tabBackground (button.getTabBackgroundColour());
    const bool isFrontTab = button.isFrontTab();

    g.setColour (isFrontTab ? tabBackground
                 : tabBackground.withMultipliedAlpha (0.9f));

    g.fillPath (path);

    g.setColour (button.findColour (isFrontTab ? TabbedButtonBar::frontOutlineColourId
                                    : TabbedButtonBar::tabOutlineColourId, false)
                 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    g.strokePath (path, PathStrokeType (isFrontTab ? 1.0f : 0.5f));
}

void CabbageLookAndFeel::drawTabButtonText (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    const Rectangle<float> area (button.getTextArea().toFloat());

    float length = area.getWidth();
    float depth  = area.getHeight();

    if (button.getTabbedButtonBar().isVertical())
        std::swap (length, depth);

    Font font (depth * 0.6f);
    font.setUnderline (button.hasKeyboardFocus (false));

    AffineTransform t;

    switch (button.getTabbedButtonBar().getOrientation())
    {
    case TabbedButtonBar::TabsAtLeft:
        t = t.rotated (float_Pi * -0.5f).translated (area.getX(), area.getBottom());
        break;
    case TabbedButtonBar::TabsAtRight:
        t = t.rotated (float_Pi *  0.5f).translated (area.getRight(), area.getY());
        break;
    case TabbedButtonBar::TabsAtTop:
    case TabbedButtonBar::TabsAtBottom:
        t = t.translated (area.getX(), area.getY());
        break;
    default:
        jassertfalse;
        break;
    }

    Colour col;

    if (button.isFrontTab() && (button.isColourSpecified (TabbedButtonBar::frontTextColourId)
                                || isColourSpecified (TabbedButtonBar::frontTextColourId)))
        col = findColour (TabbedButtonBar::frontTextColourId);
    else if (button.isColourSpecified (TabbedButtonBar::tabTextColourId)
             || isColourSpecified (TabbedButtonBar::tabTextColourId))
        col = findColour (TabbedButtonBar::tabTextColourId);
    else
        col = button.getTabBackgroundColour().contrasting();

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

    g.setColour (col.withMultipliedAlpha (alpha));
    g.setFont (font);
    g.addTransform (t);

    g.drawFittedText (button.getButtonText().trim(),
                      0, 0, (int) length, (int) depth,
                      Justification::centred,
                      jmax (1, ((int) depth) / 12));
}

void CabbageLookAndFeel::drawTabButton (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    Path tabShape;
    createTabButtonShape (button, tabShape, isMouseOver, isMouseDown);

    const Rectangle<int> activeArea (button.getActiveArea());
    tabShape.applyTransform (AffineTransform::translation ((float) activeArea.getX(),
                             (float) activeArea.getY()));

    DropShadow (Colours::black.withAlpha (0.5f), 2, Point<int> (0, 1)).drawForPath (g, tabShape);

    fillTabButtonShape (button, g, tabShape, isMouseOver, isMouseDown);
    drawTabButtonText (button, g, isMouseOver, isMouseDown);
}


void CabbageLookAndFeel::drawTabAreaBehindFrontButton (TabbedButtonBar& bar, Graphics& g, const int w, const int h)
{
    const float shadowSize = 0.2f;

    Rectangle<int> shadowRect, line;
    ColourGradient gradient (Colours::black.withAlpha (bar.isEnabled() ? 0.25f : 0.15f), 0, 0,
                             Colours::transparentBlack, 0, 0, false);

    switch (bar.getOrientation())
    {
    case TabbedButtonBar::TabsAtLeft:
        gradient.point1.x = (float) w;
        gradient.point2.x = w * (1.0f - shadowSize);
        shadowRect.setBounds ((int) gradient.point2.x, 0, w - (int) gradient.point2.x, h);
        line.setBounds (w - 1, 0, 1, h);
        break;

    case TabbedButtonBar::TabsAtRight:
        gradient.point2.x = w * shadowSize;
        shadowRect.setBounds (0, 0, (int) gradient.point2.x, h);
        line.setBounds (0, 0, 1, h);
        break;

    case TabbedButtonBar::TabsAtTop:
        gradient.point1.y = (float) h;
        gradient.point2.y = h * (1.0f - shadowSize);
        shadowRect.setBounds (0, (int) gradient.point2.y, w, h - (int) gradient.point2.y);
        line.setBounds (0, h - 1, w, 1);
        break;

    case TabbedButtonBar::TabsAtBottom:
        gradient.point2.y = h * shadowSize;
        shadowRect.setBounds (0, 0, w, (int) gradient.point2.y);
        line.setBounds (0, 0, w, 1);
        break;

    default:
        break;
    }

    g.setGradientFill (gradient);
    g.fillRect (shadowRect.expanded (2, 2));

    g.setColour (Colour (0x80000000));
    g.fillRect (line);
}

Button* CabbageLookAndFeel::createTabBarExtrasButton()
{
    const float thickness = 7.0f;
    const float indent = 22.0f;

    Path p;
    p.addEllipse (-10.0f, -10.0f, 120.0f, 120.0f);

    DrawablePath ellipse;
    ellipse.setPath (p);
    ellipse.setFill (Colours::red);

    p.clear();
    p.addEllipse (0.0f, 0.0f, 100.0f, 100.0f);
    p.addRectangle (indent, 50.0f - thickness, 100.0f - indent * 2.0f, thickness * 2.0f);
    p.addRectangle (50.0f - thickness, indent, thickness * 2.0f, 50.0f - indent - thickness);
    p.addRectangle (50.0f - thickness, 50.0f + thickness, thickness * 2.0f, 50.0f - indent - thickness);
    p.setUsingNonZeroWinding (false);

    DrawablePath dp;
    dp.setPath (p);
    dp.setFill (Colour (0x59000000));

    DrawableComposite normalImage;
    normalImage.addAndMakeVisible (ellipse.createCopy());
    normalImage.addAndMakeVisible (dp.createCopy());

    dp.setFill (Colour (0xcc000000));

    DrawableComposite overImage;
    overImage.addAndMakeVisible (ellipse.createCopy());
    overImage.addAndMakeVisible (dp.createCopy());

    DrawableButton* db = new DrawableButton ("tabs", DrawableButton::ImageFitted);
    db->setImages (&normalImage, &overImage, nullptr);
    return db;
}

//table header
void CabbageLookAndFeel::drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header)
{
    g.fillAll (Colours::black);

    const int w = header.getWidth();
    const int h = header.getHeight();

    g.setGradientFill (ColourGradient (CabbageUtilities::getBackgroundSkin(), 0.0f, h * 0.5f,
                                       Colours::black, 0.0f, h - 1.0f,
                                       false));
    g.fillRect (0, h / 2, w, h);

    g.setColour (Colour (0x33000000));
    g.fillRect (0, h - 1, w, 1);

    for (int i = header.getNumColumns (true); --i >= 0;)
        g.fillRect (header.getColumnPosition (i).getRight() - 1, 0, 1, h - 1);
}

void CabbageLookAndFeel::drawTableHeaderColumn (Graphics& g, const String& columnName, int /*columnId*/,
        int width, int height,
        bool isMouseOver, bool isMouseDown,
        int columnFlags)
{
    if (isMouseDown)
        g.fillAll (Colour (0x8899aadd));
    else if (isMouseOver)
        g.fillAll (Colour (0x5599aadd));

    int rightOfText = width - 4;

    if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
    {
        const float top = height * ((columnFlags & TableHeaderComponent::sortedForwards) != 0 ? 0.35f : (1.0f - 0.35f));
        const float bottom = height - top;

        const float w = height * 0.5f;
        const float x = rightOfText - (w * 1.25f);
        rightOfText = (int) x;

        Path sortArrow;
        sortArrow.addTriangle (x, bottom, x + w * 0.5f, top, x + w, bottom);

        g.setColour (Colour (0x99000000));
        g.fillPath (sortArrow);
    }

    //overriding colour
    g.setColour (Colours::cornflowerblue);
    g.setFont (Font (height * 0.5f, Font::bold));
    const int textX = 4;
    g.drawFittedText (columnName, textX, 0, rightOfText - textX, height, Justification::centredLeft, 1);
}

//==============================================================================
void CabbageLookAndFeel::drawStretchableLayoutResizerBar (Graphics& g, int w, int h,
        bool /*isVerticalBar*/,
        bool isMouseOver,
        bool isMouseDragging)
{
    float alpha = 0.5f;

    if (isMouseOver || isMouseDragging)
    {
        g.fillAll (Colour (Colours::cornflowerblue.withAlpha(.5f)));
        alpha = 1.0f;
    }

    const float cx = w * 0.5f;
    const float cy = h * 0.5f;
    const float cr = jmin (w, h) * 0.4f;

    g.setGradientFill (ColourGradient (Colours::white.withAlpha (alpha), cx + cr * 0.1f, cy + cr,
                                       Colours::black.withAlpha (alpha), cx, cy - cr * 4.0f,
                                       true));

    g.fillEllipse (cx - cr, cy - cr, cr * 2.0f, cr * 2.0f);
}
/*
  =========================================================================================================

	Alternative Look and Feel Class

  =========================================================================================================
*/
CabbageLookAndFeelBasic::CabbageLookAndFeelBasic()
{

}

CabbageLookAndFeelBasic::~CabbageLookAndFeelBasic()
{
}

//========= Document Window Buttons =======================================================================
Button* CabbageLookAndFeelBasic::createDocumentWindowButton (int buttonType)
{
    ImageButton* button;
    button = new ImageButton("Button");
    Image normalImage, isOverImage;

    normalImage = drawWindowButtonNormal(buttonType);
    isOverImage = drawWindowButtonIsOver(buttonType);

    button->setImages(true, false, true, normalImage, 1, Colours::transparentBlack, isOverImage,
                      1, Colours::transparentBlack, isOverImage, 1, Colours::transparentBlack, 0);


    return button;
}

//=========== Linear Slider Background ====================================================================
void CabbageLookAndFeelBasic::drawLinearSliderBackground (Graphics &g, int /*x*/, int /*y*/, int /*width*/, int /*height*/,
        float sliderPos,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const Slider::SliderStyle /*style*/,
        Slider &slider)
{
    //h slider
    slider.setTextBoxStyle (Slider::TextBoxRight, true, slider.getWidth()*0.25, 15);
    float availableWidth = slider.getWidth() * 0.65f;
    g.setColour(Colour::fromRGB(20, 20, 20));
    g.fillRoundedRectangle (slider.getWidth()*.03, slider.getHeight()*0.3, availableWidth-slider.getWidth()*.01, slider.getHeight()*0.4, slider.getHeight() / 20.0f);
//    g.setColour(Colour::fromRGB(10, 10, 10));
//    g.drawRoundedRectangle (slider.getWidth()*.1, slider.getHeight()*0.3 + 0.25f, availableWidth - slider.getWidth()*.1f, slider.getHeight()*0.4 - 0.5f, slider.getHeight() / 20.0f, 0.5f);

    // Slider is enabled and value changed only if mouse click is within the actual slider area...
    float sliderPosProportional;
    if (slider.isMouseButtonDown())
    {
        Point<float> mousePos = slider.getMouseXYRelative().toFloat();
        slider.setEnabled(true);
        if ((mousePos.getX() >= 0) && (mousePos.getX() < availableWidth))
        {
            sliderPosProportional = mousePos.getX() / (availableWidth-1.f);

            slider.setValue(slider.proportionOfLengthToValue(sliderPosProportional)); //takes into account the skew factor
        }
    }
    slider.setEnabled (false); // disabling slider
    sliderPosProportional = slider.valueToProportionOfLength(slider.getValue()); // Final slider position in proportion to length...

    //For the fill
    //float div = (slider.getValue()-slider.getMinimum()) / (slider.getMaximum()-slider.getMinimum());
    sliderPos = sliderPosProportional * availableWidth;  //div * availableWidth;
    Colour fillColour = Colours::green.withMultipliedAlpha(0.8f);
    if (slider.isMouseOver())
        fillColour = Colours::green.withMultipliedBrightness(2.0f);
    g.setColour(fillColour);
    g.fillRoundedRectangle (slider.getWidth()*.03, slider.getHeight()*0.4, sliderPos, slider.getHeight()*0.2, slider.getHeight() / 20.0f);

    //Fill border
//    g.setColour(Colours::black);
//    g.drawRoundedRectangle(12.5f, slider.getHeight()*0.3 + 0.5f, sliderPos - 1.0f, slider.getHeight()*0.4 - 1.0f, slider.getHeight() / 20.0f, 1.0f);
}

//========= linear slider ================================================================================
void CabbageLookAndFeelBasic::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{
    // g.fillAll (slider.findColour (Slider::backgroundColourId));

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        g.setColour(slider.findColour (Slider::thumbColourId));
        g.fillRoundedRectangle(x, y, width, height, 2);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

//=========== Linear Thumb =================================================================================
void CabbageLookAndFeelBasic::drawLinearSliderThumb (Graphics &g, int x, int y, int width, int height,
        float sliderPos,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const Slider::SliderStyle style,
        Slider &slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    float sliderWidth, sliderHeight;


    Colour knobColour (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
                       slider.hasKeyboardFocus (false) && slider.isEnabled(),
                       slider.isMouseOverOrDragging() && slider.isEnabled(),
                       slider.isMouseButtonDown() && slider.isEnabled()));

    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;

        if (style == Slider::LinearVertical)
        {
            kx = x + width * 0.5f;
            ky = sliderPos;
            sliderWidth = sliderRadius * 2.0f;
            sliderHeight = sliderRadius * 1.5f;

        }
        else
        {
            kx = sliderPos;
            ky = y + height * 0.5f;
            sliderWidth = sliderRadius * 1.5f;
            sliderHeight = sliderRadius * 2.0f;
        }


        ColourGradient thumbColour = ColourGradient(slider.isMouseOver() ? Colours::cornflowerblue.brighter() : Colours::cornflowerblue.withAlpha(.5f), kx - sliderRadius, ky - sliderRadius,
                                     Colours::cornflowerblue, kx+sliderRadius, sliderRadius, false);
        g.setGradientFill(thumbColour);

        g.setGradientFill(thumbColour);
        g.fillRoundedRectangle(kx - sliderRadius*1.4f, ky - sliderRadius, sliderRadius*1.5f, sliderRadius*1.5f, sliderRadius / 20.0f);

        //thumb border
        g.setColour(Colours::black);
        g.drawRoundedRectangle((kx - sliderRadius*1.4f)+1.f, (ky - sliderRadius)+1, sliderRadius*1.5f, sliderRadius*1.5f, sliderRadius / 20.0f, 1.0f);

//        CabbageUtilities::drawSphericalThumb(g,
//                                    kx - sliderRadius,
//                                    ky - sliderRadius,
//                                    sliderWidth,
//                                    sliderHeight,
//                                    knobColour, outlineThickness);
    }

}

//=================================================================================
void CabbageLookAndFeelBasic::drawStretchableLayoutResizerBar (Graphics& g, int w, int h,
        bool /*isVerticalBar*/,
        bool isMouseOver,
        bool isMouseDragging)
{
    float alpha = 0.5f;

    if (isMouseOver || isMouseDragging)
    {
        g.fillAll (Colour (Colours::cornflowerblue.withAlpha(.5f)));
        alpha = 1.0f;
    }

    const float cx = w * 0.5f;
    const float cy = h * 0.5f;
    const float cr = jmin (w, h) * 0.4f;

    g.setGradientFill (ColourGradient (Colours::white.withAlpha (alpha), cx + cr * 0.1f, cy + cr,
                                       Colours::black.withAlpha (alpha), cx, cy - cr * 4.0f,
                                       true));

    g.fillEllipse (cx - cr, cy - cr, cr * 2.0f, cr * 2.0f);
}

//=========================================================================================================
void CabbageLookAndFeelBasic::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown)
{
    Colour baseColour (backgroundColour.withMultipliedSaturation (1.3f)
                       .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.2f));

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

    const float width  = button.getWidth() - 1.0f;
    const float height = button.getHeight() - 1.0f;

    if (width > 0 && height > 0)
    {
        const bool drawRounded = button.getProperties().getWithDefault("isRounded", false);
        const float cornerSize = jmin ((drawRounded ? 15.0f : 5.f), jmin (width, height) * 0.45f);
        const float lineThickness = cornerSize * 0.1f;
        const float halfThickness = lineThickness * 0.5f;

        Path outline;
        outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));

        const Colour outlineColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                    : TextButton::textColourOffId));

        g.setColour (baseColour);
        g.fillPath (outline);

        if (! button.getToggleState())
        {
            g.setColour (outlineColour);
            g.strokePath (outline, PathStrokeType (lineThickness));
        }
    }
}

//======== Scrollbars ==============================================================================
bool CabbageLookAndFeelBasic::areScrollbarButtonsVisible()
{
    return true;
}

void CabbageLookAndFeelBasic::drawScrollbarButton (Graphics& g, ScrollBar& scrollbar,
        int width, int height, int buttonDirection,
        bool /*isScrollbarVertical*/,
        bool /*isMouseOverButton*/,
        bool isButtonDown)
{
    Path p;

    if (buttonDirection == 0)
        p.addTriangle (width * 0.5f, height * 0.2f,
                       width * 0.1f, height * 0.7f,
                       width * 0.9f, height * 0.7f);
    else if (buttonDirection == 1)
        p.addTriangle (width * 0.8f, height * 0.5f,
                       width * 0.3f, height * 0.1f,
                       width * 0.3f, height * 0.9f);
    else if (buttonDirection == 2)
        p.addTriangle (width * 0.5f, height * 0.8f,
                       width * 0.1f, height * 0.3f,
                       width * 0.9f, height * 0.3f);
    else if (buttonDirection == 3)
        p.addTriangle (width * 0.2f, height * 0.5f,
                       width * 0.7f, height * 0.1f,
                       width * 0.7f, height * 0.9f);

    if (isButtonDown)
        g.setColour (scrollbar.findColour (ScrollBar::thumbColourId).contrasting (0.2f));
    else
        g.setColour (scrollbar.findColour (ScrollBar::thumbColourId));

    g.fillPath (p);

    g.setColour (Colour (0x80000000));
    g.strokePath (p, PathStrokeType (0.5f));
}

void CabbageLookAndFeelBasic::drawScrollbar (Graphics& g,
        ScrollBar& scrollbar,
        int x, int y,
        int width, int height,
        bool isScrollbarVertical,
        int thumbStartPosition,
        int thumbSize,
        bool /*isMouseOver*/,
        bool /*isMouseDown*/)
{
    g.fillAll (scrollbar.findColour (ScrollBar::backgroundColourId));

//#ifdef AndroidBuild
    g.fillAll (Colours::red);
//#endif


    Path slotPath, thumbPath;

    const float slotIndent = jmin (width, height) > 15 ? 1.0f : 0.0f;
    const float slotIndentx2 = slotIndent * 2.0f;
    const float thumbIndent = slotIndent + 1.0f;
    const float thumbIndentx2 = thumbIndent * 2.0f;

    float gx1 = 0.0f, gy1 = 0.0f, gx2 = 0.0f, gy2 = 0.0f;

    if (isScrollbarVertical)
    {
        slotPath.addRoundedRectangle (x + slotIndent,
                                      y + slotIndent,
                                      width - slotIndentx2,
                                      height - slotIndentx2,
                                      (width - slotIndentx2) * 0.5f);

        if (thumbSize > 0)
            thumbPath.addRoundedRectangle (x + thumbIndent,
                                           thumbStartPosition + thumbIndent,
                                           width - thumbIndentx2,
                                           thumbSize - thumbIndentx2,
                                           (width - thumbIndentx2) * 0.5f);
        gx1 = (float) x;
        gx2 = x + width * 0.7f;
    }
    else
    {
        slotPath.addRoundedRectangle (x + slotIndent,
                                      y + slotIndent,
                                      width - slotIndentx2,
                                      height - slotIndentx2,
                                      (height - slotIndentx2) * 0.5f);

        if (thumbSize > 0)
            thumbPath.addRoundedRectangle (thumbStartPosition + thumbIndent,
                                           y + thumbIndent,
                                           thumbSize - thumbIndentx2,
                                           height - thumbIndentx2,
                                           (height - thumbIndentx2) * 0.5f);
        gy1 = (float) y;
        gy2 = y + height * 0.7f;
    }

    const Colour thumbColour (scrollbar.findColour (ScrollBar::thumbColourId));
    Colour trackColour1, trackColour2;

    if (scrollbar.isColourSpecified (ScrollBar::trackColourId)
            || isColourSpecified (ScrollBar::trackColourId))
    {
        trackColour1 = trackColour2 = scrollbar.findColour (ScrollBar::trackColourId);
    }
    else
    {
        trackColour1 = thumbColour.overlaidWith (Colour (0x44000000));
        trackColour2 = thumbColour.overlaidWith (Colour (0x19000000));
    }

    g.setGradientFill (ColourGradient (trackColour1, gx1, gy1,
                                       trackColour2, gx2, gy2, false));
    g.fillPath (slotPath);

    if (isScrollbarVertical)
    {
        gx1 = x + width * 0.6f;
        gx2 = (float) x + width;
    }
    else
    {
        gy1 = y + height * 0.6f;
        gy2 = (float) y + height;
    }

    g.setGradientFill (ColourGradient (Colours::transparentBlack,gx1, gy1,
                                       Colour (0x19000000), gx2, gy2, false));
    g.fillPath (slotPath);

    g.setColour (thumbColour);
    g.fillPath (thumbPath);

    g.setGradientFill (ColourGradient (Colour (0x10000000), gx1, gy1,
                                       Colours::transparentBlack, gx2, gy2, false));

    g.saveState();

    if (isScrollbarVertical)
        g.reduceClipRegion (x + width / 2, y, width, height);
    else
        g.reduceClipRegion (x, y + height / 2, width, height);

    g.fillPath (thumbPath);
    g.restoreState();

    g.setColour (Colour (0x4c000000));
    g.strokePath (thumbPath, PathStrokeType (0.4f));
}

ImageEffectFilter* CabbageLookAndFeelBasic::getScrollbarEffect()
{
    return nullptr;
}

int CabbageLookAndFeelBasic::getMinimumScrollbarThumbSize (ScrollBar& scrollbar)
{
    return jmin (scrollbar.getWidth(), scrollbar.getHeight()) * 2;
}

int CabbageLookAndFeelBasic::getDefaultScrollbarWidth()
{
    return 18;
}

int CabbageLookAndFeelBasic::getScrollbarButtonSize (ScrollBar& scrollbar)
{
    return 2 + (scrollbar.isVertical() ? scrollbar.getWidth()
                : scrollbar.getHeight());
}

//======= Toggle Buttons ========================================================================
void CabbageLookAndFeelBasic::drawToggleButton (Graphics &g, ToggleButton &button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
{
    int destWidth, destHeight;
    destHeight = button.getHeight();
    if (button.getButtonText().length() > 0)
        destWidth = destHeight;
    else
        destWidth = button.getWidth();

    int destX = 0;
    int destY = 0;
    bool isToggleOn;

    if (button.getToggleState() == true)
        isToggleOn = true;
    else
        isToggleOn = false;

    bool isRECT = button.getProperties().getWithDefault("isRect", 0);
    String svgPath = button.getProperties().getWithDefault("svgpath", "");

    //----- Creating the image
    Image newButton;
    if(!button.getToggleState())
        newButton = CabbageLookAndFeel::drawToggleImage (destWidth, destHeight, true, button.findColour(TextButton::buttonColourId), isRECT, svgPath, 5.f);
    else
        newButton = CabbageLookAndFeel::drawToggleImage (destWidth, destHeight, true, button.findColour(TextButton::buttonOnColourId), isRECT, svgPath, 5.f);



    //----- Drawing image
    g.drawImage (newButton, destX, destY, destWidth, destHeight, 0, 0, destWidth, destHeight, false);

    //----- Text
    if (button.getButtonText().length() > 0)
    {
        Justification just (1); //left
        g.setColour (button.findColour(ToggleButton::textColourId));
        String name;
        name << button.getButtonText();
        name = CabbageUtilities::cabbageString (name, CabbageUtilities::getComponentFont(), (button.getWidth()-(destWidth+5)));

        g.drawText (name, destWidth+5, destY, button.getWidth(), button.getHeight(), just, false);
    }
}
//=========== Labels, slider textboxes are also labels =============================================
void CabbageLookAndFeelBasic::drawLabel (Graphics &g, Label &label)
{
    // Getting parent component of label
    Component* comp = label.getParentComponent();

    // If slider
    if (dynamic_cast<Slider*>(comp))
    {
        //g.setColour(CabbageUtilities::getDarkerBackgroundSkin());
        //g.fillRoundedRectangle (0, 0, label.getWidth(), label.getHeight(), label.getHeight() / 10.0f);
        g.setColour(CabbageUtilities::getComponentFontColour());
        g.setFont(CabbageUtilities::getValueFont());
        g.drawText (label.getText(), 0, 0, label.getWidth(), label.getHeight(), Justification::left, false);
        /* Border
        g.setColour(CabbageUtilities::getBorderColour());
        float borderWidth = CabbageUtilities::getBorderWidth();
        g.drawRoundedRectangle(borderWidth/2, borderWidth/2, label.getWidth()-borderWidth, label.getHeight()-borderWidth,
        	label.getHeight()/5, borderWidth);*/
    }
    // Else If not a slider
    else
    {
        g.setColour (Colours::transparentBlack);
        g.fillRoundedRectangle (0, 0, label.getWidth(), label.getHeight(), label.getHeight()/5);

        // For the text
        g.setFont (CabbageUtilities::getComponentFont());
        String col = label.getProperties().getWithDefault("textColour", "");
        if (col.length()>0)
            g.setColour(Colour::fromString(col));
        else
            g.setColour (CabbageUtilities::getComponentFontColour());

        g.drawText (label.getText(), 0, 0, label.getWidth(), label.getHeight(), Justification::centred, false);
    }
}

//======== Document Window title bar ===================================================================
void CabbageLookAndFeelBasic::drawDocumentWindowTitleBar (DocumentWindow &window, Graphics &g, int w, int h,
        int /*titleSpaceX*/,
        int titleSpaceW,
        const Image */*icon*/,
        bool /*drawTitleTextOnLeft*/)
{
    window.setUsingNativeTitleBar(false);
    g.setColour (CabbageUtilities::getComponentSkin().darker(.7f));
    g.fillAll();

    g.setColour (CabbageUtilities::getComponentFontColour());
    Font font = CabbageUtilities::getTitleFont();
#ifndef MACOSX
    font.setFallbackFontName("Verdana");
#endif
    g.setFont (font);
    g.drawFittedText(window.getName(), 0, 0, window.getWidth(), window.getTitleBarHeight(), Justification::centred, 2);
}

//======== Popup Menu background ======================================================================
void CabbageLookAndFeelBasic::drawPopupMenuBackground(Graphics &g, int width, int height)
{
    g.setColour (CabbageUtilities::getDarkerBackgroundSkin());
    g.fillAll();

    g.setColour (CabbageUtilities::getComponentSkin());
    g.drawRect (0, -5, width, height+5, 1); //dont want to see top line
}

//======== Menubar background ======================================================================
void CabbageLookAndFeelBasic::drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar)
{
    g.setColour (CabbageUtilities::getBackgroundSkin());
    g.fillAll();
}

//======== Menubar item background ======================================================================
void CabbageLookAndFeelBasic::drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
        const String &itemText,
        bool isMouseOverItem,
        bool isMenuOpen,
        bool isMouseOverBar,
        MenuBarComponent &menuBar)
{
    if ((isMouseOverItem == true) || (isMenuOpen == true))
    {
        g.setColour (CabbageUtilities::getBackgroundSkin().darker (0.2));
        g.fillRect (0, 0, width, height);
    }

    g.setColour (CabbageUtilities::getComponentFontColour());
    g.setFont (CabbageUtilities::getComponentFont());
    g.drawFittedText(CabbageUtilities::cabbageString(itemText, CabbageUtilities::getComponentFont(), width*0.9), 5, 0, width, height, 36, 1);
}

//====== Returns image of a check mark ==============================================
Image CabbageLookAndFeelBasic::drawCheckMark()
{
    Image img = Image(Image::ARGB, 10, 10, true);
    Graphics g(img);

    Path path;
    path.startNewSubPath(3, 7);
    path.lineTo(5, 10);
    path.lineTo(10, 0);
    g.setColour (Colours::cornflowerblue);
    g.strokePath(path, PathStrokeType(2.0f));

    return img;
}

void CabbageLookAndFeelBasic::drawResizableWindowBorder (Graphics &g, int w, int h, const
        BorderSize< int > &border,
        ResizableWindow &window)
{
    g.fillAll(Colours::transparentBlack);
}

//======== Popup Menu Items ===========================================================================
void CabbageLookAndFeelBasic::drawPopupMenuItem (Graphics &g, int width, int height, bool isSeparator, bool /*isActive*/,
        bool isHighlighted,
        bool isTicked,
        bool hasSubMenu,
        const String &text,
        const String &shortcutKeyText,
        Image */*image*/,
        const Colour */*const textColour*/)
{
    if ((isHighlighted == true) && (isSeparator == false))
    {
        g.setColour (CabbageUtilities::getComponentSkin());
        g.fillAll();
        g.setColour (Colours::cornflowerblue);
    }
    else
        g.setColour (CabbageUtilities::getComponentFontColour());

    g.setFont (CabbageUtilities::getComponentFont());
    g.drawText (CabbageUtilities::cabbageString(text, CabbageUtilities::getComponentFont(), width*0.8), 20, 0, width*0.8, height, 1, false);

    if (isSeparator == true)
    {
        g.setColour(CabbageUtilities::getComponentSkin());
        g.drawLine(0, height/2, width, 3);
    }

    if (isTicked)
    {
        Image checkMark = drawCheckMark();
        g.setColour(Colours::cornflowerblue);
        g.drawImage(checkMark, 5, (height/2)-5, 10, 10, 0, 0, 10, 10, false);
    }
    if (hasSubMenu)
    {
        g.setColour(Colours::cornflowerblue);
        const Line<float> line(width-15, height*.5, width-5, height*.5);
        g.drawArrow(line, 0, height*.3, height*.3);
    }
    if (shortcutKeyText.isNotEmpty())
    {
        const int leftBorder = (height * 5) / 4;
        const int rightBorder = 4;

        g.drawText (shortcutKeyText,
                    leftBorder, 0, width - (leftBorder + rightBorder + 4), height,
                    Justification::centredRight,
                    true);
    }
}

//====== Draw Window Button Normal Image =================================================================
Image CabbageLookAndFeelBasic::drawWindowButtonNormal(int buttonType)
{
    int width, height;
    width = height = 20;
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);

    String str;

    if (buttonType == 1)
        str << "_";
    else if (buttonType == 2)
        str << "+";
    else if (buttonType == 4)
        str << "x";

    //----- Background
    g.setColour (Colours::transparentBlack);
    g.fillRoundedRectangle(1, 1, width-2, height-2, 2);

    //----- Text symbol
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    g.setColour (CabbageUtilities::getComponentFontColour());
    g.drawText(str, (width/2) - (font.getHeight()/2), (width/2) - (font.getHeight()/2),
               font.getHeight(), font.getHeight(), 36, false);

    return img;
}

//====== Draw Window Button Image Is Over ==============================================================
Image CabbageLookAndFeelBasic::drawWindowButtonIsOver(int buttonType)
{
    int width, height;
    width = height = 20;
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);

    String str;

    if (buttonType == 1)
        str << "_";
    else if (buttonType == 2)
        str << "+";
    else if (buttonType == 4)
        str << "x";

    //---- Background
    g.setColour (Colours::transparentBlack);
    g.fillRoundedRectangle(1, 1, width-2, height-2, 2);

    //---- Text symbol
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    g.setColour (Colours::whitesmoke);
    g.drawText(str, (width/2) - (font.getHeight()/2), (width/2) - (font.getHeight()/2),
               font.getHeight(), font.getHeight(), 36, false);

    //---- V lines
    ColourGradient cg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                       0, height, false);
    cg.addColour (0.5, CabbageUtilities::getComponentFontColour());
    g.setGradientFill (cg);
    g.drawLine (0, 0, 0, height, 1);
    g.drawLine (width, 0, width, height, 1);

    return img;
}