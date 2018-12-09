#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class PropertyPanelLookAndFeel : public LookAndFeel_V3
{
public:
    PropertyPanelLookAndFeel() {};
    ~PropertyPanelLookAndFeel() {};

    void setColours (Colour panelBG, Colour labelBG, Colour labelText);

    void drawPropertyPanelSectionHeader(Graphics& g, const String& name, bool isOpen, int width, int height) override;
    void drawPropertyComponentBackground (Graphics&, int width, int height, PropertyComponent&) override;
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;

private:
    Colour panelBGcolour;
    Colour labelBGcolour;
    Colour labelTextColour;
};