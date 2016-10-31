/*
  ==============================================================================

    CabbageApplication.h
    Created: 12 Oct 2016 11:22:13am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEAPPLICATION_H_INCLUDED
#define CABBAGEAPPLICATION_H_INCLUDED

#include "CabbageCommonHeaders.h"
#include "./Settings/CabbageSettingsWindow.h"
#include "./CodeEditor/CabbageOutputConsole.h"
#include "Audio/AudioGraph.h"



class CabbageProjectWindow;
class CabbageMainDocumentWindow;

//==============================================================================
class CabbageApplication  : public JUCEApplication, public ChangeListener, public Timer, public ValueTree::Listener
{
public:


	class TempPropsWindow : public DocumentWindow
	{
		
	public: 
		TempPropsWindow():DocumentWindow("Tester",
            Colours::lightgrey,
            DocumentWindow::allButtons)
			{
				setSize(300, 500);
			}
			
			void closeButtonPressed()
			{
				this->setVisible(false);
			}
			~TempPropsWindow()
			{
				clearContentComponent();
			}
	};

    //==============================================================================
    CabbageApplication();

    static CabbageApplication& getApp();
    static ApplicationCommandManager& getCommandManager();
    void changeListenerCallback(ChangeBroadcaster* source);
    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }
    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

	void timerCallback();
    void initialiseBasics();
    bool initialiseLogger (const char* filePrefix);
    void initCommandManager();
    void deleteLogger();
    void shutdown();
    //==============================================================================
    MenuBarModel* getMenuModel();
    StringArray getMenuNames();
    void createMenu (PopupMenu&, const String& menuName);
    void createFileMenu (PopupMenu&);
    void createEditMenu (PopupMenu&);
    void createViewMenu (PopupMenu&);
    void createBuildMenu (PopupMenu&);
    void createColourSchemeItems (PopupMenu&);
    void createWindowMenu (PopupMenu&);
    void createToolsMenu (PopupMenu&);
    void handleMainMenuCommand (int menuItemID);
    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;
	void showAudioSettings();
    void createNewProject();
    void askUserToOpenFile();
    bool openFile (const File&, String type="");
	void newFile (String type);
    bool closeAllDocuments (bool askUserToSave);
    bool closeAllMainWindows();
	void showSettingsDialog();
	void saveDocument();
	void runCode();
	void stopCode();
	void showGenericWidgetWindow();
	void hideGenericWidgetWindow(bool freeContent=false);
	Identifier getCurrentInterfaceMode();
	void setCurrentInterfaceMode(Identifier mode);
    PropertySet* getDefaultSettings();
	void createGenericCsoundPluginWrapper();
    ScopedPointer<ApplicationCommandManager> commandManager;
    ScopedPointer<CabbageSettings> cabbageSettings;
    ScopedPointer<FileLogger> logger;
    PropertiesFile::Options getPropertyFileOptionsFor (const String& filename);
    CabbageIDELookAndFeel lookAndFeel;
    bool isRunningCommandLine;
	CodeEditorComponent* getCodeEditor();
	CabbageOutputConsole* getOutputConsole();
	void createEditorForAudioGraphNode();
	void createAudioGraph();
	void enableEditMode(bool enable);
	CabbagePluginEditor* getPluginEditor();
	
//	AudioGraph* getPluginWrapper()
//	{
//		return pluginWindow->getPluginWrapper();
//	}
    //==============================================================================
    void initialise (const String& commandLine) override;

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override  {}

    struct MainMenuModel  : public MenuBarModel
    {
        MainMenuModel()
        {
            setApplicationCommandManagerToWatch (&getCommandManager());
        }

        StringArray getMenuBarNames() override
        {
            return getApp().getMenuNames();
        }

        PopupMenu getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) override
        {
            PopupMenu menu;
            getApp().createMenu (menu, menuName);
            return menu;
        }

        void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override
        {
            getApp().handleMainMenuCommand (menuItemID);
        }
    };
    ScopedPointer<MainMenuModel> menuModel;
	File currentCsdFile;
	
	//==============================================================================
    void changed()
    {

    }

    //==============================================================================
    void valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier) override
	{
		CabbageUtilities::debug(identifier.toString(), tree.getProperty(identifier).toString());
	}
	
    void valueTreeChildAdded (ValueTree&, ValueTree&) override
    {
        changed();
    }
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override
    {
        changed();
    }
    void valueTreeChildOrderChanged (ValueTree&, int, int) override
    {
        changed();
    }
    void valueTreeParentChanged (ValueTree&) override
    {
        changed();
    }
private:
	bool isGUIEnabled = false;
	String consoleMessages;
	Identifier pluginType;
	ValueTree cabbageWidgets;
	Identifier currentInterfaceMode;
	ScopedPointer<AudioGraph> audioGraph;
    ScopedPointer<CabbageMainDocumentWindow> mainDocumentWindow;
	ScopedPointer<TempPropsWindow> propsWindow;
};




#endif  // CABBAGEAPPLICATION_H_INCLUDED
