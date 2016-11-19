/*
  ==============================================================================

    DockableWindows.h
    Created: 6 Jul 2016 10:27:19pm
    Author:  jim

  ==============================================================================
*/

#ifndef DOCKABLEWINDOWS_H_INCLUDED
#define DOCKABLEWINDOWS_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

class DockableWindowManager;
class DockableComponentWrapper;

/**
Base for things that can act as docks for DockableComponents
*/
class DockBase
{
public:
    DockBase(DockableWindowManager & manager_, Component * dockComponent);

    virtual ~DockBase();
    /**
    Called when the user is dragging over the dock.  This should do something to highlight where the
    component may be placed, or do nothing if the component cannot be placed here.
    */
    virtual void showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition) = 0;
    /**
    Called when the dragging has stopped or moved away from the component and we don't need to show
    the highlighted position any more.
    */
    virtual void hideDockableComponentPlacement() = 0;
    /**
    Mark or otherwise show that the component is being moved from the dock.    E.g. by making the component
    grey or invisible.   If the user decides not to drag the component after all attachDockableComponent
    will be called.
    */
    virtual void startDockableComponentDrag(DockableComponentWrapper* component) = 0;
    /**
    Called when the user drags a window over the dock and releases the mouse, use this to place the
    window into the dock.  Return false if the DockableComponentWrapper cannot be added, and a new window will be
    created instead.
    */
    virtual bool attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition) = 0;
    /**
    Should remove the component from the dock if it's present, and resize or rearrange any other windows
    accordingly.
    */
    virtual void detachDockableComponent(DockableComponentWrapper* component) = 0;

    /**
    Tests to see if this Dock contains the provided screen position.  Used internally
    while drags are in progress.
    */
    bool containsScreenPosition(const Point<int>& screenPosition) const;

    bool isUsingComponent(Component * c) const
    {
        return dockComponent == c;
    }

    /**
    Your dock should implement this to enable components to be revealed and/or moved to the front.
    */
    virtual void revealComponent(DockableComponentWrapper* dockableComponent) {}
private:
    DockableWindowManager & manager;
    Component* dockComponent;
};

/**
The DockableWindowManager controls the attaching and removing of DockableWindow objects from DockBase
implementations and top level windows.
*/
class DockableWindowManager
{
public:
    DockableWindowManager();

    class TransparentDragImageWindow
        :
        public TopLevelWindow
    {
    public:
        TransparentDragImageWindow(Image imageForDraggingWindow);
        void paint(Graphics& g) override;
        Image image;
    };

    /**
    Shows an outline of the component while it's being dragged, and highlight any
    docks the component is dragged over.
    */
    void handleComponentDrag(DockableComponentWrapper *, Point<int> location, int w, int h);

    /**
    Removes the outline when the mouse is released.
    */
    void clearTargetPosition();

    /**
    Removes the component from it's parent, prior to rehoming it.
    */
    void divorceComponentFromParent(DockableComponentWrapper* component);

    /**
    Returns the dock under the provided screen position, if there is no
    dock returns nullptr.

    @note may return the wrong result if there are overlapping docks.
    */
    DockBase* getDockUnderScreenPosition(Point<int> position);

    /**
    Called to put a component in new screen position.  Assesses whether this is
    a dock, a tab or a top level window and attaches the window to the new
    component.
    */
    void handleComponentDragEnd(DockableComponentWrapper* component, const Point<int> & screenPosition);

    /**
    Creates a DockableComponentWrapper.  Use this when writing docks to encapsulate components you are adding
    to the docking window system.
    */
    DockableComponentWrapper* createDockableComponent(Component* component);

    /**
    Returns the DockBase object with a Dock component of 'component'.  Note that this is the
    dock's component.  This isn't a way of directly finding the Dock holding your DockableComponentWrapper.
    */
    DockBase* getDockWithComponent(Component* component) const;

    /**
    Brings a specific DockableComponentWrapper to the front of any tabbed docks.
    */
    void bringComponentToFront(DockableComponentWrapper* dockableComponent);

private:
    friend class DockBase;
    void addDock(DockBase* newDock);
    void removeDock(DockBase* dockToRemove);

    void createHeavyWeightWindow(DockableComponentWrapper * comp, const Point<int> & screenPosition);

    /** All the DockableComponentWrapper windows we have on the desktop */
    OwnedArray<ResizableWindow> windows;

    /** All the dockable component wrappers we have.. */
    OwnedArray<DockableComponentWrapper> dockableComponents;

    /** The floating window currently being dragged. */
    ScopedPointer<TransparentDragImageWindow> transparentDragImageWindow;

    /** And all the docks ... */
    Array<DockBase *> docks;

    DockBase * highlightedDock{ nullptr };
    Component * currentlyDraggedComponent{ nullptr };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DockableWindowManager)
};


class DockableComponentTitleBar;
class DockableComponentTab;

/**
Base class for windows that can be dragged between docks and desktop windows.

@todo add a way of using custom tabs and titlebars.
*/
class DockableComponentWrapper
    :
    public Component
{
public:
    DockableComponentWrapper(DockableWindowManager &);
    DockableComponentWrapper(DockableWindowManager &, Component * contentComponentUnowned);

    void setContentComponentUnowned(Component* content);

    /**
    This should be somewhere else so the user can do a nice override, maybe it should
    be a feature of the DockableWindowManager
    */
    String getWindowTitle() const
    {
        if (contentComponent)
            return contentComponent->getName();

        return String::empty;
    }

    void resized() override;

    /**
    Docks should review the tab sizes and make a decision about whether the tabs can have their
    ideal size or whether they need to compress the sizes to allow all the tabs to fit on the screen.

    @see setTabWidth
    */
    int getIdealTabWidth() const;

    /** @see getIdealTabWidth */
    void setTabWidth(int tabWidth);

    /** @see getIdealTabWidth */
    int getTabWidth();

    /**
    If the component is in a dock this returns the dock it's in.  If it's not in a dock,
    or it's a stand alone window this returns nullptr.
    */
    DockBase* getCurrentDock() const;

    /**
    Whether the wrapper should show the tab handle at the bottom.
    */
    void setShowTabButton(bool shouldShowTab, int xPosition, bool isFront);

    /** @internal - callback from the Tab button */
    void tabButtonClicked()
    {
        manager.bringComponentToFront(this);
    }

    Rectangle<int> getTabButtonBounds() const;

private:
    ScopedPointer<DockableComponentTitleBar> titleBar;
    ScopedPointer<DockableComponentTab> tabButton;

    int tabXPosition{ 0 };
    int tabWidthToUse{ -1 };

    Component * contentComponent{ nullptr };
    DockableWindowManager & manager;
};

/**
Provides drag and drop features for DockableWindows widgets that can be used for
moving the windows around.
*/
class DockableComponentDraggable
    :
    public Component
{
public:
    DockableComponentDraggable(DockableComponentWrapper& owner_, DockableWindowManager& manager_);
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    DockableComponentWrapper & getDockableComponent() const
    {
        return owner;
    }
    bool isDragging() const
    {
        return dragging;
    }
private:
    bool dragging{ false };
    DockableComponentWrapper & owner;
    DockableWindowManager & manager;
};

/**
Title bar that implements the drag and drop functions.
*/
class DockableComponentTitleBar
    :
    public DockableComponentDraggable
{
public:
    DockableComponentTitleBar(DockableComponentWrapper& owner_, DockableWindowManager& manager_);
    void paint(Graphics& g) override;
};

/**
Tab that implements the drag and drop functions.
*/
class DockableComponentTab
    :
    public DockableComponentDraggable
{
public:
    DockableComponentTab(DockableComponentWrapper& owner_, DockableWindowManager& manager_);

    /**
    Return the width required for rendering the table.  There's no guarantee that this
    sized space will actully be available though.  So the paint will need to cope with
    smaller sizes.
    */
    virtual int getIdealWidth();

    /**
    Returns true if this tab is showing at the front of the stack of tabs.  The paint
    routine should probably call this to find out if it needs to render the tab differently.
    */
    bool isFrontTab() const
    {
        return frontTab;
    }

    /**
    @internal: Called internally to set the frontTab flag.
    */
    void setIsFrontTab(bool nowFrontTab);

    void mouseUp(const MouseEvent& e) override;
private:
    void paint(Graphics& g) override;
    bool frontTab{ false };
};

/**
A simple example dock.  This lets you put an arbitray number of Components in a vertical stack.  The
windows are sized to fit the available space.
*/
class WindowDockVertical
    :
    public Component,
    DockBase
{
public:
    WindowDockVertical(DockableWindowManager& manager_);

    ~WindowDockVertical();

    /**
    We assume you are managing the components lifetime.  However an optional change could be to have the
    DockManager manage them.
    */
    void addComponentToDock(Component* comp);

    void resized() override;
    void paint(Graphics& g) override;
    void paintOverChildren(Graphics& g) override;

private:
    struct PlacementPosition
    {
        int yPosition;
        int insertAfterComponentIndex;
    };

    /** Finds the nearest top or bottom edge of an existing component to the mouse Y position */
    PlacementPosition getPlacementPositionForPoint(Point<int> pointRelativeToComponent) const;

    void startDockableComponentDrag(DockableComponentWrapper* component) override;
    void showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition) override;
    bool attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition) override;
    void detachDockableComponent(DockableComponentWrapper* component) override;
    void hideDockableComponentPlacement() override;

    bool highlight{ false };
    int highlightYPosition{ 0 };

    Array<DockableComponentWrapper *> dockableComponentWrappers;
    DockableWindowManager & manager;
};

/**
Displays a number of components on top of each other in a tab-stylee!
*/
class TabDock
    :
    public Component,
    DockBase
{
public:
    TabDock(DockableWindowManager& manager_);
    void addComponentToDock(Component* comp);
    void resized() override;
    void paintOverChildren(Graphics& g) override;

private:
    struct PlacementPosition
    {
        int xPosition;
        int insertAfterComponentIndex;
    };

    /** Finds the nearest top or bottom edge of an existing component to the mouse Y position */
    PlacementPosition getPlacementPositionForPoint(Point<int> pointRelativeToComponent) const;

    /* Implementation of DockBase */
    void startDockableComponentDrag(DockableComponentWrapper* component) override;
    void showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition) override;
    bool attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition) override;
    void detachDockableComponent(DockableComponentWrapper* component) override;
    void hideDockableComponentPlacement() override;
    void revealComponent(DockableComponentWrapper* dockableComponent) override;

    DockableWindowManager & manager;

    Array<DockableComponentWrapper *> dockedComponents;

    bool highlight{ false };
    int highlightXPosition{ 0 };
    const int tabHeight = 16;
};



#endif  // DOCKABLEWINDOWS_H_INCLUDED
