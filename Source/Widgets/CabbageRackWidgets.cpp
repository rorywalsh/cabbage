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

#include "CabbageRackWidgets.h"

CabbageScrew::CabbageScrew (ValueTree wData, bool isLineWidget) : CabbageWidgetBase(),
widgetData (wData),
owner (owner)
{
    widgetData.addListener (this);
    String file = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getFullPathName();
    imgFile = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file));
    if(File(imgFile).existsAsFile())
        img = ImageFileFormat::loadFrom(imgFile);
    this->setWantsKeyboardFocus (false);
    initialiseCommonAttributes (this, wData);
    svgText =
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
            "<!-- Created with Inkscape (http://www.inkscape.org/) -->\n"
            "\n"
            "<svg\n"
            "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
            "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
            "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
            "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
            "   xmlns=\"http://www.w3.org/2000/svg\"\n"
            "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
            "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
            "   width=\"14.999999\"\n"
            "   height=\"14.9989\"\n"
            "   viewBox=\"0 0 3.9687497 3.9684593\"\n"
            "   version=\"1.1\"\n"
            "   id=\"svg18800\"\n"
            "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\"\n"
            "   sodipodi:docname=\"Screw.svg\">\n"
            "  <defs\n"
            "     id=\"defs18794\" />\n"
            "  <sodipodi:namedview\n"
            "     id=\"base\"\n"
            "     pagecolor=\"#ffffff\"\n"
            "     bordercolor=\"#666666\"\n"
            "     borderopacity=\"1.0\"\n"
            "     inkscape:pageopacity=\"0.0\"\n"
            "     inkscape:pageshadow=\"2\"\n"
            "     inkscape:zoom=\"15.839192\"\n"
            "     inkscape:cx=\"-8.2033163\"\n"
            "     inkscape:cy=\"6.1745754\"\n"
            "     inkscape:document-units=\"mm\"\n"
            "     inkscape:current-layer=\"layer1\"\n"
            "     showgrid=\"false\"\n"
            "     fit-margin-top=\"0\"\n"
            "     fit-margin-left=\"0\"\n"
            "     fit-margin-right=\"0\"\n"
            "     fit-margin-bottom=\"0\"\n"
            "     inkscape:window-width=\"1920\"\n"
            "     inkscape:window-height=\"1080\"\n"
            "     inkscape:window-x=\"0\"\n"
            "     inkscape:window-y=\"0\"\n"
            "     inkscape:window-maximized=\"0\"\n"
            "     units=\"px\" />\n"
            "  <metadata\n"
            "     id=\"metadata18797\">\n"
            "    <rdf:RDF>\n"
            "      <cc:Work\n"
            "         rdf:about=\"\">\n"
            "        <dc:format>image/svg+xml</dc:format>\n"
            "        <dc:type\n"
            "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
            "        <dc:title></dc:title>\n"
            "      </cc:Work>\n"
            "    </rdf:RDF>\n"
            "  </metadata>\n"
            "  <g\n"
            "     inkscape:label=\"Layer 1\"\n"
            "     inkscape:groupmode=\"layer\"\n"
            "     id=\"layer1\"\n"
            "     transform=\"translate(-30.138143,-67.088277)\">\n"
            "    <path\n"
            "       inkscape:connector-curvature=\"0\"\n"
            "       id=\"path6833\"\n"
            "       style=\"fill:#b3b3b3;fill-opacity:1;fill-rule:nonzero;stroke:#333333;stroke-width:0.26458332;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\"\n"
            "       d=\"m 33.709893,69.072512 c 0,0.875407 -0.711844,1.5875 -1.58725,1.5875 -0.875406,0 -1.587499,-0.712093 -1.587499,-1.5875 0,-0.875406 0.712093,-1.5875 1.587499,-1.5875 0.875406,0 1.58725,0.712094 1.58725,1.5875\" />\n"
            "    <path\n"
            "       d=\"m 33.151139,68.877659 v 0.389457 h -2.056992 v -0.389457 z\"\n"
            "       style=\"fill:#4d4d4d;fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:0.28636625\"\n"
            "       id=\"path6843\"\n"
            "       inkscape:connector-curvature=\"0\" />\n"
            "    <path\n"
            "       d=\"m 32.317372,68.043891 v 2.056993 h -0.389457 v -2.056993 z\"\n"
            "       style=\"fill:#4d4d4d;fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:0.28636625\"\n"
            "       id=\"path6845\"\n"
            "       inkscape:connector-curvature=\"0\" />\n"
            "  </g>\n"
            "</svg>";

}

//==============================================================================
void CabbageScrew::paint (Graphics& g)
{

    drawFromSVG(g, svgText, 0, 0, getWidth(), getHeight(), AffineTransform());

}

//==============================================================================
void CabbageScrew::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    handleCommonUpdates (this, valueTree);
    repaint();
}


CabbagePort::CabbagePort (ValueTree wData, bool isLineWidget) : CabbageWidgetBase(),
                                                                  widgetData (wData),
                                                                  owner (owner)
{
    widgetData.addListener (this);
    String file = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getFullPathName();
    imgFile = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file));
    if(File(imgFile).existsAsFile())
        img = ImageFileFormat::loadFrom(imgFile);
    this->setWantsKeyboardFocus (false);
    initialiseCommonAttributes (this, wData);
}

//==============================================================================
void CabbagePort::paint (Graphics& g)
{

}

//==============================================================================
void CabbagePort::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    handleCommonUpdates (this, valueTree);
    repaint();
}

