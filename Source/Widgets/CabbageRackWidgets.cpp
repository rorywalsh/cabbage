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

CabbageScrew::CabbageScrew (ValueTree wData) : CabbageWidgetBase(),
widgetData (wData)
{
    widgetData.addListener (this);

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


CabbagePort::CabbagePort (ValueTree wData) : CabbageWidgetBase(),
widgetData (wData)
{
    widgetData.addListener (this);

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
    "   width=\"8.3556204mm\"\n"
    "   height=\"8.3556299mm\"\n"
    "   viewBox=\"0 0 8.3556203 8.3556298\"\n"
    "   version=\"1.1\"\n"
    "   id=\"svg15246\"\n"
    "   sodipodi:docname=\"Port.svg\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\">\n"
    "  <defs\n"
    "     id=\"defs15240\">\n"
    "    <clipPath\n"
    "       id=\"clip89\">\n"
    "      <rect\n"
    "         y=\"0\"\n"
    "         x=\"0\"\n"
    "         width=\"18\"\n"
    "         height=\"19\"\n"
    "         id=\"rect4864\" />\n"
    "    </clipPath>\n"
    "    <clipPath\n"
    "       id=\"clip90\">\n"
    "      <path\n"
    "         inkscape:connector-curvature=\"0\"\n"
    "         d=\"m 0.898438,0.128906 h 16.25 v 17.882813 h -16.25 z m 0,0\"\n"
    "         id=\"path4861\" />\n"
    "    </clipPath>\n"
    "    <mask\n"
    "       id=\"mask44\">\n"
    "      <g\n"
    "         style=\"filter:url(#alpha)\"\n"
    "         id=\"g4858\"\n"
    "         transform=\"matrix(0.26458333,0,0,0.26458333,89.358789,128.57765)\">\n"
    "        <rect\n"
    "           x=\"0\"\n"
    "           y=\"0\"\n"
    "           width=\"3052.8701\"\n"
    "           height=\"3351.5\"\n"
    "           style=\"fill:#000000;fill-opacity:0.14999402;stroke:none\"\n"
    "           id=\"rect4856\" />\n"
    "      </g>\n"
    "    </mask>\n"
    "    <filter\n"
    "       id=\"alpha\"\n"
    "       filterUnits=\"objectBoundingBox\"\n"
    "       x=\"0\"\n"
    "       y=\"0\"\n"
    "       width=\"1\"\n"
    "       height=\"1\">\n"
    "      <feColorMatrix\n"
    "         type=\"matrix\"\n"
    "         in=\"SourceGraphic\"\n"
    "         values=\"0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 1 0\"\n"
    "         id=\"feColorMatrix4149\" />\n"
    "    </filter>\n"
    "    <clipPath\n"
    "       id=\"clipPath17821\">\n"
    "      <rect\n"
    "         y=\"0\"\n"
    "         x=\"0\"\n"
    "         width=\"18\"\n"
    "         height=\"19\"\n"
    "         id=\"rect17819\" />\n"
    "    </clipPath>\n"
    "    <clipPath\n"
    "       id=\"clipPath17825\">\n"
    "      <path\n"
    "         inkscape:connector-curvature=\"0\"\n"
    "         d=\"m 0.898438,0.128906 h 16.25 v 17.882813 h -16.25 z m 0,0\"\n"
    "         id=\"path17823\" />\n"
    "    </clipPath>\n"
    "    <clipPath\n"
    "       id=\"clip87\">\n"
    "      <rect\n"
    "         y=\"0\"\n"
    "         x=\"0\"\n"
    "         width=\"24\"\n"
    "         height=\"26\"\n"
    "         id=\"rect4848\" />\n"
    "    </clipPath>\n"
    "    <clipPath\n"
    "       id=\"clip88\">\n"
    "      <path\n"
    "         inkscape:connector-curvature=\"0\"\n"
    "         d=\"m 0.683594,0.921875 h 22.679687 v 24.9375 H 0.683594 Z m 0,0\"\n"
    "         id=\"path4845\" />\n"
    "    </clipPath>\n"
    "    <mask\n"
    "       id=\"mask43\">\n"
    "      <g\n"
    "         style=\"filter:url(#alpha)\"\n"
    "         id=\"g4842\"\n"
    "         transform=\"matrix(0.26458333,0,0,0.26458333,89.358789,128.57765)\">\n"
    "        <rect\n"
    "           x=\"0\"\n"
    "           y=\"0\"\n"
    "           width=\"3052.8701\"\n"
    "           height=\"3351.5\"\n"
    "           style=\"fill:#000000;fill-opacity:0.14999402;stroke:none\"\n"
    "           id=\"rect4840\" />\n"
    "      </g>\n"
    "    </mask>\n"
    "    <filter\n"
    "       id=\"filter17836\"\n"
    "       filterUnits=\"objectBoundingBox\"\n"
    "       x=\"0\"\n"
    "       y=\"0\"\n"
    "       width=\"1\"\n"
    "       height=\"1\">\n"
    "      <feColorMatrix\n"
    "         type=\"matrix\"\n"
    "         in=\"SourceGraphic\"\n"
    "         values=\"0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 1 0\"\n"
    "         id=\"feColorMatrix17834\" />\n"
    "    </filter>\n"
    "    <clipPath\n"
    "       id=\"clipPath17840\">\n"
    "      <rect\n"
    "         y=\"0\"\n"
    "         x=\"0\"\n"
    "         width=\"24\"\n"
    "         height=\"26\"\n"
    "         id=\"rect17838\" />\n"
    "    </clipPath>\n"
    "    <clipPath\n"
    "       id=\"clipPath17844\">\n"
    "      <path\n"
    "         inkscape:connector-curvature=\"0\"\n"
    "         d=\"m 0.683594,0.921875 h 22.679687 v 24.9375 H 0.683594 Z m 0,0\"\n"
    "         id=\"path17842\" />\n"
    "    </clipPath>\n"
    "  </defs>\n"
    "  <sodipodi:namedview\n"
    "     id=\"base\"\n"
    "     pagecolor=\"#ffffff\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1.0\"\n"
    "     inkscape:pageopacity=\"0.0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:zoom=\"11.2\"\n"
    "     inkscape:cx=\"15.941594\"\n"
    "     inkscape:cy=\"5.9688197\"\n"
    "     inkscape:document-units=\"mm\"\n"
    "     inkscape:current-layer=\"layer1\"\n"
    "     showgrid=\"false\"\n"
    "     inkscape:window-width=\"1274\"\n"
    "     inkscape:window-height=\"1080\"\n"
    "     inkscape:window-x=\"644\"\n"
    "     inkscape:window-y=\"0\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     units=\"px\"\n"
    "     fit-margin-top=\"0\"\n"
    "     fit-margin-left=\"0\"\n"
    "     fit-margin-right=\"0\"\n"
    "     fit-margin-bottom=\"0\" />\n"
    "  <metadata\n"
    "     id=\"metadata15243\">\n"
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
    "     transform=\"translate(-88.611154,-119.19859)\">\n"
    "    <path\n"
    "       inkscape:connector-curvature=\"0\"\n"
    "       id=\"path7255\"\n"
    "       d=\"m 92.788964,127.42922 c -2.235179,0 -4.05281,-1.81762 -4.05281,-4.05282 0,-2.23516 1.817631,-4.05281 4.05281,-4.05281 2.235176,0 4.05281,1.81765 4.05281,4.05281 0,2.2352 -1.817634,4.05282 -4.05281,4.05282\"\n"
    "       style=\"fill:#333333;fill-opacity:1;fill-rule:nonzero;stroke:#636663;stroke-width:0.25;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\" />\n"
    "    <path\n"
    "       inkscape:connector-curvature=\"0\"\n"
    "       id=\"path7261\"\n"
    "       d=\"m 92.788964,126.29511 c -1.609548,0 -2.918685,-1.30916 -2.918685,-2.91871 0,-1.60954 1.309137,-2.91867 2.918685,-2.91867 1.609549,0 2.918682,1.30913 2.918682,2.91867 0,1.60955 -1.309133,2.91871 -2.918682,2.91871\"\n"
    "       style=\"fill:#333333;fill-opacity:1;fill-rule:nonzero;stroke:#b3b3b3;stroke-width:0.25;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\" />\n"
    "    <path\n"
    "       inkscape:connector-curvature=\"0\"\n"
    "       id=\"path7265\"\n"
    "       d=\"m 94.588681,123.3764 c 0,0.99357 -0.806153,1.79974 -1.799717,1.79974 -0.993567,0 -1.79972,-0.80617 -1.79972,-1.79974 0,-0.99356 0.806153,-1.79969 1.79972,-1.79969 0.993564,0 1.799717,0.80613 1.799717,1.79969\"\n"
    "       style=\"fill:#000000;fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:0.35277775\" />\n"
    "  </g>\n"
    "</svg>\n"
    "";
}

//==============================================================================
void CabbagePort::paint (Graphics& g)
{
    drawFromSVG(g, svgText, 0, 0, getWidth(), getHeight(), AffineTransform());
}

//==============================================================================
void CabbagePort::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    handleCommonUpdates (this, valueTree);
    repaint();
}


CabbageLight::CabbageLight(ValueTree wData, CabbagePluginEditor* owner) : CabbageWidgetBase(),
widgetData (wData),
owner (owner),
mainColour (Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)))
{
    widgetData.addListener (this);
    initialiseCommonAttributes (this, wData);
}

//==============================================================================
void CabbageLight::paint (Graphics& g)
{
    g.fillAll (Colours::transparentBlack);
    g.setColour (mainColour);
    g.fillEllipse (0, 0, getWidth(), getHeight());
}

//==============================================================================
void CabbageLight::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    mainColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour));
    handleCommonUpdates (this, valueTree);
    repaint();
}



