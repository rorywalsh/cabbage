#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class FlatButtonLookAndFeel : public LookAndFeel_V3
{
public:
    FlatButtonLookAndFeel() {};
    ~FlatButtonLookAndFeel() {};

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    void drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

};
