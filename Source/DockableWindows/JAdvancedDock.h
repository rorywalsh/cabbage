/*

  ==============================================================================

	JAdvancedDock.h
	Created: 6 Jul 2016 10:27:02pm
	Author:  jim

  ==============================================================================
*/

#ifndef ADVANCEDDOCK_H_INCLUDED
#define ADVANCEDDOCK_H_INCLUDED

#include "JDockableWindows.h"

namespace AdvancedDockPlaces
{
enum Places
{
    top, left, right, bottom, centre,
    none
};
};

class AdvancedDockPlacementDialog;

/**
The advanced dock allows vertical and horizontal splits, as well as tabs.
*/
class JAdvancedDock
    :
    public Component,
    DockBase
{
public:
    JAdvancedDock(DockableWindowManager& manager_);

    ~JAdvancedDock();;

    /** Adds a component to the dock so it's visible to the user.

     We assume you are managing the components lifetime.  However an optional
     change could be to have the DockManager manage them.
     */
    void addComponentToDock(Component * component);

    /** Adds a component to the dock so it's visible to the user.

     @param rowPosition - where to insert the new row.  If rowPosition is -1 it
     will be inserted at the bottom. Rows are numbered from the top down.  A row
     position of 0 will insert the component at the top. */
    void addComponentToNewRow(Component * component, int rowPosition);


    void resized() override;
    void paint(Graphics& g) override;

private:
    struct WindowLocation
    {
        WindowLocation(int y, int x, int t);
        int y{ 0 };
        int x{ 0 };
        int tab{ 0 };
    };

    WindowLocation getWindowLocationAtPoint(const Point<int>& screenPosition);
    Rectangle<int> getWindowBoundsAtPoint(const Point<int>& p);
    /**
    Insert a new window in to the right place in our dock...
    */
    void insertWindow(const Point<int>& screenPos, AdvancedDockPlaces::Places places, DockableComponentWrapper* comp);

    void showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition) override;
    void hideDockableComponentPlacement() override;
    void startDockableComponentDrag(DockableComponentWrapper* component) override;
    void insertNewDock(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc);
    void insertNewRow(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc);
    void insertToNewTab(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc);
    bool attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition) override;
    void detachDockableComponent(DockableComponentWrapper* component) override;
    void revealComponent(DockableComponentWrapper* dockableComponent) override;


    class RowType;

    std::vector<RowType> rows;
    std::vector<std::unique_ptr<StretchableLayoutResizerBar>> resizers;
    StretchableLayoutManager layout;

    void rebuildRowResizers();
    void layoutRows(const Rectangle<int>& area);

    DockableWindowManager & manager;
    ScopedPointer<AdvancedDockPlacementDialog> placementDialog;
};


#endif  // ADVANCEDDOCK_H_INCLUDED
