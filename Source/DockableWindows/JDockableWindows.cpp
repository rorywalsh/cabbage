/*
  ==============================================================================

    DockableWindows.cpp
    Created: 6 Jul 2016 10:27:19pm
    Author:  jim

  ==============================================================================
*/

#include "JDockableWindows.h"


//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

DockBase::DockBase(DockableWindowManager& manager_, Component * dockComponent_)
    :
    manager(manager_),
    dockComponent(dockComponent_)
{
    manager.addDock(this);
}


DockBase::~DockBase()
{
    manager.removeDock(this);
}

bool DockBase::containsScreenPosition(const Point<int>& screenPosition) const
{
    auto screenBounds = dockComponent->getScreenBounds();
    return screenBounds.contains(screenPosition);
}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

DockableWindowManager::DockableWindowManager()
{
}

void DockableWindowManager::addDock(DockBase* newDock)
{
    docks.addIfNotAlreadyThere(newDock);
}

void DockableWindowManager::removeDock(DockBase* dockToRemove)
{
    docks.removeAllInstancesOf(dockToRemove);
}

DockableWindowManager::TransparentDragImageWindow::TransparentDragImageWindow(Image image_) :
    TopLevelWindow("Window Being Dragged", true), image(image_)
{
    setOpaque(false);
}

void DockableWindowManager::createHeavyWeightWindow(DockableComponentWrapper * comp, const Point<int> &screenPosition)
{
    auto window = new ResizableWindow(comp->getName(), true);
    window->setContentNonOwned(comp, true);
    window->setTopLeftPosition(screenPosition);
    windows.add(window);
    window->setVisible(true);
}


void DockableWindowManager::TransparentDragImageWindow::paint(Graphics& g)
{
    g.setColour(Colours::blue.withAlpha(0.5f));
    g.drawImageAt(image, 0, 0, false);
}

DockBase* DockableWindowManager::getDockUnderScreenPosition(Point<int> position)
{
    for (auto d : docks)
        if (d->containsScreenPosition(position))
            return d;

    return nullptr;
}

void DockableWindowManager::handleComponentDragEnd(DockableComponentWrapper* component, const Point<int> & screenPosition)
{
    divorceComponentFromParent(component);

    if (highlightedDock)
        highlightedDock->hideDockableComponentPlacement();

    highlightedDock = nullptr;

    component->setShowTabButton(false, 0, false);

    auto targetDock = getDockUnderScreenPosition(screenPosition);
    bool didAttachToDock = false;

    if (targetDock)
        didAttachToDock = targetDock->attachDockableComponent(component, screenPosition);

    if (!targetDock || !didAttachToDock)
        createHeavyWeightWindow(component, screenPosition);

    currentlyDraggedComponent = nullptr;
}

DockableComponentWrapper* DockableWindowManager::createDockableComponent(Component* component)
{
    auto d = new DockableComponentWrapper(*this, component);
    dockableComponents.add(d);
    return d;
}

DockBase* DockableWindowManager::getDockWithComponent(Component* component) const
{
    for (auto d: docks)
        if (d->isUsingComponent(component))
            return d;

    return nullptr;
}

void DockableWindowManager::bringComponentToFront(DockableComponentWrapper* dockableComponent)
{
    for (auto d : docks)
        d->revealComponent(dockableComponent);
}

void DockableWindowManager::handleComponentDrag(DockableComponentWrapper * componentBeingDragged, Point<int> location, int w, int h)
{
    if (!transparentDragImageWindow)
    {
        auto image = componentBeingDragged->createComponentSnapshot(componentBeingDragged->getLocalBounds());
        transparentDragImageWindow = new TransparentDragImageWindow(image);
    }

    auto dock = getDockUnderScreenPosition(location);

    if (dock)
    {
        dock->showDockableComponentPlacement(componentBeingDragged, location);

        if (dock != highlightedDock && highlightedDock)
            highlightedDock->hideDockableComponentPlacement();

        highlightedDock = dock;
    }

    transparentDragImageWindow->setBounds(location.getX(), location.getY(), w, h);
    transparentDragImageWindow->setVisible(true);

    if (componentBeingDragged != currentlyDraggedComponent)
    {
        auto parentDock = componentBeingDragged->getCurrentDock();

        if (parentDock)
            parentDock->startDockableComponentDrag(componentBeingDragged);

        currentlyDraggedComponent = componentBeingDragged;
    }
}

void DockableWindowManager::clearTargetPosition()
{
    transparentDragImageWindow = nullptr;
}

void DockableWindowManager::divorceComponentFromParent(DockableComponentWrapper* component)
{
    ResizableWindow* foundWindow{ nullptr };

    for (auto w : windows)
    {
        if (w->getContentComponent() == component)
            foundWindow = w;
    }

    if (foundWindow)
    {
        windows.removeObject(foundWindow);
    }
    else
    {
        for (auto d : docks)
            d->detachDockableComponent(component);
    }
}

//==============================================================================

DockableComponentWrapper::DockableComponentWrapper(DockableWindowManager & manager_)
    :
    manager(manager_)
{
    setInterceptsMouseClicks(false, true);
    titleBar = new DockableComponentTitleBar(*this, manager);
    addAndMakeVisible(titleBar);
}

DockableComponentWrapper::DockableComponentWrapper(DockableWindowManager&manager_, Component* contentComponentUnowned)
    :
    manager(manager_)
{
    setInterceptsMouseClicks(false, true);
    titleBar = new DockableComponentTitleBar(*this, manager);
    addAndMakeVisible(titleBar);

    setContentComponentUnowned(contentComponentUnowned);
}

void DockableComponentWrapper::resized()
{
    auto area = getLocalBounds();
    titleBar->setBounds(area.removeFromTop(20));

    if (tabButton)
    {
        if (tabWidthToUse == -1)
            tabWidthToUse = tabButton->getIdealWidth();

        auto tabArea = area.removeFromBottom(16);
        tabArea.setX(tabXPosition);
        tabArea.setWidth(tabWidthToUse);
        tabButton->setBounds(tabArea);
    }

    if (contentComponent)
        contentComponent->setBounds(area);
}

int DockableComponentWrapper::getIdealTabWidth() const
{
    if (tabButton)
        return tabButton->getIdealWidth();

    return 0;
}

void DockableComponentWrapper::setTabWidth(int tabWidth)
{
    tabWidthToUse = tabWidth;
}

int DockableComponentWrapper::getTabWidth()
{
    if (tabWidthToUse == -1)
        tabWidthToUse = getIdealTabWidth();

    return tabWidthToUse;
}

void DockableComponentWrapper::setContentComponentUnowned(Component* content)
{
    contentComponent = content;

    if (contentComponent)
        addAndMakeVisible(contentComponent);

    resized();
}

DockBase* DockableComponentWrapper::getCurrentDock() const
{
    auto parent = getParentComponent();

    while (parent != nullptr)
    {
        auto dockBase = manager.getDockWithComponent(parent);

        if (dockBase)
            return dockBase;

        parent = parent->getParentComponent();
    }

    return nullptr; // it's a window and not docked!
}

void DockableComponentWrapper::setShowTabButton(bool shouldShowTab, int xPos_, bool isFrontTab_)
{
    if (shouldShowTab)
    {
        DBG("showing tab for " + getWindowTitle());
        tabButton = new DockableComponentTab(*this, manager);
        addAndMakeVisible(tabButton);
        tabXPosition = xPos_;
        tabButton->setIsFrontTab(isFrontTab_);
    }
    else
    {
        DBG("hiding tab for " + getWindowTitle());
        tabButton = nullptr;
    }

    resized();
}

Rectangle<int> DockableComponentWrapper::getTabButtonBounds() const
{
    if (tabButton)
        return tabButton->getBounds();

    return {};
}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

DockableComponentDraggable::DockableComponentDraggable(DockableComponentWrapper& owner_, DockableWindowManager& manager_) :
    owner(owner_),
    manager(manager_)
{
    setMouseCursor(MouseCursor::DraggingHandCursor);
}

void DockableComponentDraggable::mouseDrag(const MouseEvent& e)
{
    if (dragging || e.getDistanceFromDragStart() > 10)
    {
        auto windowPosition = e.getScreenPosition();
        manager.handleComponentDrag(&owner, windowPosition, owner.getWidth(), owner.getHeight());
        dragging = true;
    }
}

void DockableComponentDraggable::mouseUp(const MouseEvent& e)
{
    manager.clearTargetPosition();
    manager.handleComponentDragEnd(&owner, e.getScreenPosition());
    dragging = false;
}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

DockableComponentTitleBar::DockableComponentTitleBar(DockableComponentWrapper& owner_, DockableWindowManager& manager_):
    DockableComponentDraggable(owner_, manager_)
{
}

void DockableComponentTitleBar::paint(Graphics& g)
{
    g.fillAll(Colours::grey);
    g.setFont(Font(12.0).boldened());
    g.drawText(getDockableComponent().getWindowTitle(), getLocalBounds(), Justification::centred, false);

    g.setColour(Colours::black.withAlpha(0.2f));
    g.drawHorizontalLine(getHeight() - 1, 0.0f, float(getWidth()));
    g.drawVerticalLine(getWidth() - 1, 0.0f, float(getHeight()));

    g.setColour(Colours::lightgrey.withAlpha(0.2f));
    g.drawHorizontalLine(0, 0.0f, float(getWidth()));
}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

DockableComponentTab::DockableComponentTab(DockableComponentWrapper& owner_, DockableWindowManager& manager_):
    DockableComponentDraggable(owner_, manager_)
{
}

int DockableComponentTab::getIdealWidth()
{
    return Font(12.0).boldened().getStringWidth(getDockableComponent().getWindowTitle()) + 4;
}

void DockableComponentTab::paint(Graphics& g)
{
    g.fillAll(frontTab ? Colours::grey : Colours::darkgrey);

    g.setFont(Font(12.0).boldened());
    g.drawText(getDockableComponent().getWindowTitle(), getLocalBounds(), Justification::centred, true);
}

void DockableComponentTab::setIsFrontTab(bool nowFrontTab)
{
    frontTab = nowFrontTab;
    repaint();
}

void DockableComponentTab::mouseUp(const MouseEvent& e)
{
    if (!isDragging())
        getDockableComponent().tabButtonClicked();
    else
        DockableComponentDraggable::mouseUp(e);
}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

WindowDockVertical::WindowDockVertical(DockableWindowManager& manager_)
    :
    DockBase(manager_, this),
    manager(manager_)
{
}

WindowDockVertical::~WindowDockVertical()
{
}

void WindowDockVertical::addComponentToDock(Component* comp)
{
    auto dockable = manager.createDockableComponent(comp);
    dockableComponentWrappers.add(dockable);
    addAndMakeVisible(dockable);
    resized();
}

void WindowDockVertical::resized()
{
    auto visibleComponentCount = int{ 0 };

    for (auto d : dockableComponentWrappers)
        if (d->isVisible())
            visibleComponentCount++;

    if (visibleComponentCount == 0)
        return;

    auto windowHeight = getHeight() / visibleComponentCount;
    auto area = getLocalBounds();

    for (auto d : dockableComponentWrappers)
    {
        d->setShowTabButton(false, 0, false);

        if (!d->isVisible())
            continue;

        // intelligent resize code codes here
        d->setBounds(area.withHeight(windowHeight));
        area.translate(0, windowHeight);
    }
}

void WindowDockVertical::paint(Graphics& g)
{
    g.fillAll(Colours::black);

}

void WindowDockVertical::paintOverChildren(Graphics& g)
{
    if (highlight)
    {
        g.setColour(Colours::red);
        g.fillRect(0, highlightYPosition-2, getWidth(), 4);
    }
}

WindowDockVertical::PlacementPosition WindowDockVertical::getPlacementPositionForPoint(Point<int> pointRelativeToComponent) const
{
    int result{0};
    int componentIndex{0};

    auto target = pointRelativeToComponent.getY();
    auto distance = abs(result - target);

    int count{ 0 };

    for (auto wrapper: dockableComponentWrappers)
    {
        count++;

        if (!wrapper->isVisible())
            continue;

        auto compBottom = wrapper->getBounds().getBottom();
        auto newDistance = abs(compBottom - target);

        if (newDistance < distance)
        {
            result = compBottom;
            distance = newDistance;
            componentIndex = count;
        }
    }

    return{ result, componentIndex };
}

void WindowDockVertical::startDockableComponentDrag(DockableComponentWrapper* component)
{
    component->setVisible(false);
    resized();
}

void WindowDockVertical::detachDockableComponent(DockableComponentWrapper* component)
{
    if (!dockableComponentWrappers.contains(component))
        return;

    removeChildComponent(component);
    dockableComponentWrappers.removeAllInstancesOf(component);
}

void WindowDockVertical::hideDockableComponentPlacement()
{
    highlight = false;
    repaint();
}

void WindowDockVertical::showDockableComponentPlacement(DockableComponentWrapper*, Point<int> position)
{
    highlight = true;
    auto pos = getPlacementPositionForPoint(getLocalPoint(nullptr, position));
    highlightYPosition = pos.yPosition;
    repaint();
}

bool WindowDockVertical::attachDockableComponent(DockableComponentWrapper* component, Point<int> position)
{
    addAndMakeVisible(component);
    auto pos = getPlacementPositionForPoint(getLocalPoint(nullptr, position));
    dockableComponentWrappers.insert(pos.insertAfterComponentIndex, component);
    resized();
    return true;
}


//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

TabDock::TabDock(DockableWindowManager& manager_):
    DockBase(manager_, this),
    manager(manager_)
{
}

void TabDock::addComponentToDock(Component* comp)
{
    auto dockable = manager.createDockableComponent(comp);
    dockedComponents.add(dockable);
    addAndMakeVisible(dockable);
    resized();
}

void TabDock::resized()
{
    auto area = getLocalBounds();

    int x = 0;

    auto lastComponent = getChildComponent(getNumChildComponents() - 1);

    for (auto& dockedCompWrapper : dockedComponents)
    {
        if (dockedCompWrapper->isVisible())
        {
            dockedCompWrapper->setBounds(area);
            dockedCompWrapper->setShowTabButton(true, x, dockedCompWrapper == lastComponent);
            x += dockedCompWrapper->getTabWidth() + 2;
        }
    }
}

void TabDock::paintOverChildren(Graphics& g)
{
    if (!highlight)
        return;

    g.setColour(Colours::red);
    g.fillRect(highlightXPosition - 1, getHeight() - tabHeight, 3, tabHeight);
}

void TabDock::revealComponent(DockableComponentWrapper* dockableComponent)
{
    dockableComponent->toFront(false);
    resized();
}

TabDock::PlacementPosition TabDock::getPlacementPositionForPoint(Point<int> pointRelativeToComponent) const
{
    int result{0};
    int componentIndex{0};

    auto target = pointRelativeToComponent.getX();
    auto distance = abs(result - target);

    int count{0};

    for (auto dockableComponentWrapper : dockedComponents)
    {
        count++;

        if (!dockableComponentWrapper->isVisible())
            continue;

        auto tabRightSide = dockableComponentWrapper->getTabButtonBounds().getRight();
        auto newDistance = abs(tabRightSide - target);

        if (newDistance < distance)
        {
            result = tabRightSide;
            distance = newDistance;
            componentIndex = count;
        }
    }

    return {result, componentIndex};
}

void TabDock::startDockableComponentDrag(DockableComponentWrapper* component)
{
    component->setVisible(false);
    resized();
}

void TabDock::showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition)
{
    auto placement = getPlacementPositionForPoint(getLocalPoint(nullptr, screenPosition));
    highlight = true;
    highlightXPosition = placement.xPosition;
    repaint();
}

bool TabDock::attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition)
{
    auto placement = getPlacementPositionForPoint(getLocalPoint(nullptr, screenPosition));
    addAndMakeVisible(component);
    dockedComponents.insert(placement.insertAfterComponentIndex, component);
    resized();
    return true;
}

void TabDock::detachDockableComponent(DockableComponentWrapper* component)
{
    if (!dockedComponents.contains(component))
        return;

    removeChildComponent(component);
    dockedComponents.removeAllInstancesOf(component);
    resized();
}

void TabDock::hideDockableComponentPlacement()
{
    highlight = false;
    repaint();
}
//==============================================================================

