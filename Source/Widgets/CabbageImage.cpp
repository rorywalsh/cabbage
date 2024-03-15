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

#include "CabbageImage.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageImage::CabbageImage (ValueTree wData, CabbagePluginEditor* owner, bool isLineWidget)
    : CabbageWidgetBase(owner),
    shape (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::shape)),
    owner (owner),
    corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
    cropx (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropx)),
    cropy (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropy)),
    cropwidth (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropwidth)),
    cropheight (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropheight)),
    lineThickness (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    widgetData (wData),
    outlineColour (Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::outlinecolour))),
    mainColour (Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour))),
    isLineWidget (isLineWidget),
    glShaderCode(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::glshader)),
    isParent(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::isparent))
{
    
    prevWidth = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::width);
    prevHeight = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::height);
    widgetData.addListener (this);
	
    svgElement = createSVG(wData);
    //int isParent = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::isparent);
    String fileBase64 = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file);

	if (fileBase64.isNotEmpty()) {
		MemoryOutputStream out;
		bool result = Base64::convertFromBase64(out, fileBase64);
		if (result) 
		{
			img = ImageCache::getFromMemory(out.getData(), int(out.getDataSize()));
		}
		else
		{
            String path = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile);
            if (path.isEmpty())
            {
                imgFile = File::getCurrentWorkingDirectory().getChildFile(fileBase64).getFullPathName();
            }
            else
            {
                imgFile = File(path).getParentDirectory().getChildFile(fileBase64).getFullPathName();
            }
			if (File(imgFile).existsAsFile())
				img = ImageCache::getFromFile(imgFile);
		}
	}

	this->setWantsKeyboardFocus (false);
    initialiseCommonAttributes (this, wData);
    
    const int allowInteraction = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::mouseinteraction);
    if(allowInteraction)
        setInterceptsMouseClicks(true, true);
    else
        setInterceptsMouseClicks(false, true);
    
}

//==============================================================================
void CabbageImage::paint (Graphics& g)
{
    if (glShaderCode.isNotEmpty() && owner->openGLEnabled)
    {
        g.setColour(mainColour);
        if (shader.get() == nullptr || shader->getFragmentShaderCode() != glShaderCode)
        {
            shader.reset();

            shader.reset(new OpenGLGraphicsContextCustomShader(glShaderCode));

            auto result = shader->checkCompilation(g.getInternalContext());

            if (result.failed())
            {
                shader.reset();
            }

        }

        if (shader.get() != nullptr)
        {
            shader->fillRect(g.getInternalContext(), getLocalBounds());
        }
    }
    else 
    {
        if (isLineWidget)
        {
            g.setColour(mainColour);
            g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 1);

            g.setColour(CabbageUtilities::getBackgroundSkin());
            g.fillRoundedRectangle(0, 0, getWidth() - 1, getHeight() - 1, 1);
        }
        else
        {

            if (imgFile.hasFileExtension(".svg"))
            {
                CabbageLookAndFeel2::drawFromSVG(g, imgFile, 0, 0, getWidth(), getHeight(), AffineTransform());
            }
            else if (img.isValid())

            {
                g.drawImage(img, 0, 0, getWidth(), getHeight(), cropx, cropy,
                    cropwidth == 0 ? img.getWidth() : cropwidth,
                    cropheight == 0 ? img.getHeight() : cropheight);
            }

            else
            {
                g.fillAll(Colours::transparentBlack);
                g.setColour(mainColour);

                if (shape == "square")
                    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), corners);
                else
                    g.fillEllipse(lineThickness * .9f, lineThickness * .9f, getWidth() - lineThickness * 1.9f, getHeight() - lineThickness * 1.9f);


                g.setColour(outlineColour);

                if (shape == "square")
                    g.drawRoundedRectangle(0, 0, jmax(1, getWidth()), jmax(1, getHeight()), corners, lineThickness);
                else
                    g.drawEllipse(lineThickness / 2.f, lineThickness / 2.f, jmax(1, getWidth() - lineThickness), jmax(1, getHeight() - lineThickness), lineThickness);
            }

            if (usesSVGElement)
            {
                g.fillAll(Colours::transparentBlack);
                svg = (XmlDocument::parse(svgElement));

                if (svg == nullptr)
                    return;

                if (svg != nullptr)
                {
                    drawable = Drawable::createFromSVG(*svg);
                    drawable->draw(g, 1.f, AffineTransform());
                }
            }
        }
    }
}
//==============================================================================
void CabbageImage::mouseDown (const MouseEvent& e)
{
    owner->sendChannelDataToCsound (getChannel(), currentToggleValue);
    currentToggleValue =! currentToggleValue;
}

void CabbageImage::changeListenerCallback (ChangeBroadcaster* source)
{
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::visible, 0);
}
//==============================================================================
void CabbageImage::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (CabbagePluginEditor::PopupDocumentWindow* parentComp = dynamic_cast<CabbagePluginEditor::PopupDocumentWindow*> (getParentComponent()))
    {
        const int isParentvisible = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::visible);
        parentComp->addChangeListener (this);

        if (isParentvisible == 1)
        {
            parentComp->setVisible (true);
            parentComp->toFront (true);
        }
        else
            parentComp->setVisible (false);
    }
    
    if((prop.toString() == "width" || prop.toString() == "height") && isParent)
        resizeAllChildren(valueTree);
    
    lineThickness = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::outlinethickness);
    outlineColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour));
    mainColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour));
    shape = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::shape);
    updateImage(valueTree);
    cropy = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropy);
    cropx = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropx);
    cropwidth = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropwidth);
    cropheight = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropheight);
    handleCommonUpdates (this, valueTree, false, prop);
    
    if (prop == CabbageIdentifierIds::svgElement)
        svgElement = createSVG(valueTree);
    else if (prop == CabbageIdentifierIds::glshader)
    {
        glShaderCode = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::glshader);
        owner->setOpenGLForShader();
    }
    repaint();
}

String CabbageImage::createSVG(ValueTree valueTree)
{
    if(CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::svgElement).toString().isNotEmpty())
    {
        svgViewBox = "<svg viewBox=\"0 0 " + CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::width).toString() + " " + CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::height).toString() + "\">";
        usesSVGElement = true;
        return svgViewBox + CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::svgElement) + "\n</svg>";

    }
    else
    {
        usesSVGElement = false;
        return "";
    }
}

void CabbageImage::resizeAllChildren(ValueTree& valueTree)
{
    
    const double newWidth = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::width);
    const double newHeight = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::height);
    if(newWidth < 30 || newHeight < 30)
        return;
    
    const double xScale = newWidth/prevWidth;
    const double yScale = newHeight/prevHeight;
    for( auto comp : this->getChildren())
    {
        const Point<int> compSize(comp->getWidth(), comp->getHeight());
        const Point<int> compPos(comp->getX(), comp->getY());
        comp->setBounds(compPos.getX()*xScale, compPos.getY()*yScale, compSize.getX()*xScale, compSize.getY()*yScale);
    }
 
    prevWidth = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::width);
    prevHeight = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::height);
}


void CabbageImage::updateImage(ValueTree& valueTree)
{
    //int isParent = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::isparent);
    String fileBase64 = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::file);

    if (fileBase64.isNotEmpty()) {
        MemoryOutputStream out;
        bool result = Base64::convertFromBase64(out, fileBase64);
        if (result)
        {
            img = ImageCache::getFromMemory(out.getData(), out.getDataSize());
        }
        else
        {
            String path = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::csdfile);
            if (path.isEmpty())
            {
                imgFile = File::getCurrentWorkingDirectory().getChildFile(fileBase64).getFullPathName();
            }
            else
            {
                imgFile = File(path).getParentDirectory().getChildFile(fileBase64).getFullPathName();
            }
            if (File(imgFile).existsAsFile())
                img = ImageCache::getFromFile(imgFile);
        }
    }

}

