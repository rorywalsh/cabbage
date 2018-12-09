#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class PropertyPanelLookAndFeel : public LookAndFeel_V3
{
public:
    PropertyPanelLookAndFeel() {};
    ~PropertyPanelLookAndFeel() {};

    void setBackgroundColour (Colour bg);

    void drawPropertyPanelSectionHeader(Graphics& g, const String& name, bool isOpen, int width, int height) override;

private:
    Colour backgroundColour;
};