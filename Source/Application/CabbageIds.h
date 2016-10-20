/*
  ==============================================================================

    CabbageCommandIDs.h
    Created: 12 Oct 2016 11:49:31am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECOMMANDIDS_H_INCLUDED
#define CABBAGECOMMANDIDS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace CommandIDs
{
enum
{
    newProject              = 0x200010,
    open                    = 0x200020,
    closeDocument           = 0x200030,
    saveDocument            = 0x200040,
    saveDocumentAs          = 0x200041,
	settings				= 0x200042,

    closeProject            = 0x200051,
    saveProject             = 0x200060,
    saveAll                 = 0x200080,
    openInIDE               = 0x200072,
    saveAndOpenInIDE        = 0x200073,
    createNewExporter       = 0x20007d,

    showUTF8Tool            = 0x200076,
    showGlobalPreferences   = 0x200077,
    showConfigPanel         = 0x200074,
    showFilePanel           = 0x200078,
    showTranslationTool     = 0x200079,
    showProjectSettings     = 0x20007a,
    showProjectModules      = 0x20007b,
    showSVGPathTool         = 0x20007c,

    closeWindow             = 0x201001,
    closeAllDocuments       = 0x201000,
    goToPreviousDoc         = 0x201002,
    goToNextDoc             = 0x201003,
    goToCounterpart         = 0x201004,
    deleteSelectedItem      = 0x201005,

    showFindPanel           = 0x2010a0,
    findSelection           = 0x2010a1,
    findNext                = 0x2010a2,
    findPrevious            = 0x2010a3,


    showBuildTab            = 0x410001,
    cleanAll                = 0x410010,
    enableBuild             = 0x410011,
    showWarnings            = 0x410012,
    reinstantiateComp       = 0x410013,
    launchApp               = 0x410014,
    killApp                 = 0x410015,
    buildNow                = 0x41001a,
    toggleContinuousBuild   = 0x41001b,

    enableSnapToGrid        = 0x410016,
    zoomIn                  = 0x410017,
    zoomOut                 = 0x410018,
    zoomNormal              = 0x410019,
    spaceBarDrag            = 0x4100a1,

    nextError               = 0x4100b1,
    prevError               = 0x4100b2,

    loginLogout             = 0x4100c1,

    lastCommandIDEntry
};
}

namespace CommandCategories
{
static const char* const general       = "General";
static const char* const editing       = "Editing";
static const char* const view          = "View";
static const char* const windows       = "Windows";
}

// Handy list of static Identifiers..
namespace Ids
{
#define DECLARE_ID(name)      const Identifier name (#name)

DECLARE_ID (name);
DECLARE_ID (font);
DECLARE_ID (colour);
DECLARE_ID (csoundManualDirectory);
DECLARE_ID (cabbagePlantDirectory);
DECLARE_ID (cabbageExamplesDirectory);
const Identifier ID ("id");
const Identifier ID_uppercase ("ID");
const Identifier class_ ("class");

#undef DECLARE_ID
};

// Handy list of static Identifiers..
namespace CabbageColourIds
{
static const Identifier menuBarBackground = "Interface - MenuBar Background";
static const Identifier menuBarText = "Interface - MenuBarText";
static const Identifier menuBarMouseOverBackground = "Interface - MenuBar MouseOver Background";
static const Identifier popupMenuMouseOverBackground = "Interface - PopupMenu MouseOver Background";
static const Identifier popupMenuBackground = "Interface - PopupMenu Background";
static const Identifier popupMenuText = "Interface - PopupMenu Text";
static const Identifier popupMenuHighlightedText = "Interface - PopupMenu Highlighted Text";
static const Identifier mainBackground = "Interface - Main Background";
static const Identifier alertWindowBackground = "Editor - Alart Window Background";
static const Identifier codeBackground = "Editor - Code Background";
static const Identifier lineNumberBackground = "Editor - Line Numbers Background";
static const Identifier lineNumbers = "Editor - Line Numbers";
static const Identifier plainText = "Editor - PlainText";
static const Identifier selectTextBackground = "Editor - Selected Text Background";
static const Identifier caret = "Editor - Caret";
static const Identifier identifierLiteral = "Editor - Identifier";
static const Identifier operatorLiteral = "Editor - Operator";
static const Identifier keyword = "Editor - Keyword";
static const Identifier comment = "Editor - Comment";
static const Identifier error = "Editor - Error";
static const Identifier csdtags = "Editor - Csd Tags";
}

#endif  // CABBAGECOMMANDIDS_H_INCLUDED
