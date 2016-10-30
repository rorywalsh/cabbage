/*
  ==============================================================================

    JAdvancedDock.cpp
    Created: 6 Jul 2016 10:27:02pm
    Author:  jim

  ==============================================================================
*/

#include "JAdvancedDock.h"

/**
Manages a row of windows complete with vertical resizer bars.
*/
class JAdvancedDock::RowType
{
public:
	RowType()
	{
		layout = std::make_unique<StretchableLayoutManager>();
	}

	RowType(RowType && rhs)
	{
		resizers = std::move(rhs.resizers);
		columns = std::move(rhs.columns);
		layout = std::move(rhs.layout);
	}

	RowType & operator=(RowType && rhs)
	{
		resizers = std::move(rhs.resizers);
		columns = std::move(rhs.columns);
		layout = std::move(rhs.layout);
		return *this;
	}

	typedef std::vector<std::unique_ptr<DockableComponentWrapper>> TabDockType;

	void add(TabDockType && newTabDock, int position, Component * parent)
	{
		columns.insert(columns.begin() + position, std::move(newTabDock));
		rebuildResizers(parent);
	}

	/**
	Called from the docks resize method.

	@note this and the layoutRows() function are similar, but not similar enough...
	*/
	void layoutColumns(const Rectangle<int> & area)
	{
		std::vector<Component *> comps;

		for (int i = 0; i < columns.size(); ++i)
		{
			comps.push_back(columns[i].front().get());

			if (i < resizers.size())
				comps.push_back(resizers[i].get());
		}

		if (comps.size() == 1)
		{
			comps[0]->setBounds(area); // layoutComponents doesn't seem to cope with only one component passed to it.
		}
		else
		{
			layout->layOutComponents(comps.data(), comps.size(),
				area.getX(), area.getY(), area.getWidth(), area.getHeight(), false, true);
		}

		/* Make all other tabs match our new size...*/
		for (auto & c : columns)
		{
			if (c.size() > 1)
				for (auto & t : c)
					t->setBounds(c[0]->getBounds());
		}
	}

	void rebuildResizers(Component * parent)
	{
		const double resizerWidth = 5.0;
		resizers.clear();

		int itemIndex = 0;

		for (int pos = 0; pos < columns.size(); ++pos)
		{
			layout->setItemLayout(itemIndex++, 10.0, 2000.0, columns[pos][0]->getWidth());

			if (pos < columns.size() - 1)
			{
				resizers.push_back(std::make_unique<StretchableLayoutResizerBar>(layout.get(), pos * 2 + 1, true));
				parent->addAndMakeVisible(resizers.back().get());
				layout->setItemLayout(itemIndex++, resizerWidth, resizerWidth, resizerWidth);
			}
		}
	}

	/** Sets up the correct tab configuration for a docked component that needs to display tabs */
	static void configureTabs(const TabDockType & vector)
	{
		int tabXPos = 0;

		struct FrontComponent
		{
			int zOrder{ -1 };
			int x;
			DockableComponentWrapper * component;
		} frontComponent;

		if (vector.size() < 2)
			return;

		for (auto& dockedCompWrapper : vector)
		{
			if (dockedCompWrapper->isVisible())
			{
				auto parent = dockedCompWrapper->getParentComponent();
				auto myIndex = parent->getIndexOfChildComponent(dockedCompWrapper.get());

				if (myIndex > frontComponent.zOrder)
				{
					frontComponent.zOrder = myIndex;
					frontComponent.component = dockedCompWrapper.get();
					frontComponent.x = tabXPos;
				}

				dockedCompWrapper->setShowTabButton(true, tabXPos, false);
				tabXPos += dockedCompWrapper->getTabWidth() + 2;
			}
		}

		if (frontComponent.zOrder != -1)
			frontComponent.component->setShowTabButton(true, frontComponent.x, true);
	}

	std::unique_ptr<StretchableLayoutManager> layout;
	std::vector<TabDockType> columns;
	std::vector<std::unique_ptr<StretchableLayoutResizerBar>> resizers;
};


/**
Displays some buttons that let the user decide where they want to place the window.
*/
class AdvancedDockPlacementDialog
	:
	public Component
{
public:
	AdvancedDockPlacementDialog()
	{
		for (int i = AdvancedDockPlaces::top; i <= AdvancedDockPlaces::centre; ++i)
			buttons.add(new PlacementButton(AdvancedDockPlaces::Places(i)));

		for (auto b : buttons)
			addAndMakeVisible(b);
	}


	void setLeftRightVisible(bool nowVisible) 
	{
		buttons[AdvancedDockPlaces::left]->setVisible(nowVisible);
		buttons[AdvancedDockPlaces::right]->setVisible(nowVisible);
	}

	void setTopBottomVisible(bool nowVisible) 
	{
		buttons[AdvancedDockPlaces::top]->setVisible(nowVisible);
		buttons[AdvancedDockPlaces::bottom]->setVisible(nowVisible);
	}

	void setMousePosition(const Point<int>& screenPos)
	{
		auto p = getLocalPoint(nullptr, screenPos);

		for (auto b : buttons)
			b->setIsMouseOver(b->contains(b->getLocalPoint(this, p)));
		
		repaint();
	}

	class PlacementButton : public Component
	{
	public:
		PlacementButton(AdvancedDockPlaces::Places place): place(place) {}

		void paint(Graphics & g) override
		{
			g.setColour(Colours::white);

			if (mouseOver)
				g.fillRect(getLocalBounds().toFloat().reduced(2.0f));
			else
				g.drawRect(getLocalBounds().toFloat().reduced(2.0f), 1.0f);

			g.fillRect(getLocalBounds().toFloat().reduced(2.0f).withHeight(3.0f));
		}


		void setIsMouseOver(bool isOver)
		{
			if (isOver != mouseOver)
			{
				mouseOver = isOver;
				repaint();
			}
		};

		bool mouseOver{false};
		AdvancedDockPlaces::Places place;
	};

	void resized() override
	{
		auto area = getLocalBounds().toFloat().reduced(padding);
		auto h3 = area.getHeight() / 3.0f;
		auto w3 = area.getWidth() / 3.0f;

		topArea = area.removeFromTop(h3).translated(h3, 0.0f).withWidth(h3);
		buttons[AdvancedDockPlaces::top]->setBounds(topArea.toNearestInt());

		midArea = area.removeFromTop(h3);

		auto midAreaChop = midArea;

		buttons[AdvancedDockPlaces::left]->setBounds(midAreaChop.removeFromLeft(w3).toNearestInt());
		centreArea = midAreaChop.removeFromLeft(w3);
		buttons[AdvancedDockPlaces::centre]->setBounds(centreArea.toNearestInt());
		buttons[AdvancedDockPlaces::right]->setBounds(midAreaChop.toNearestInt());

		bottomArea = area.translated(h3, 0.0f).withWidth(h3);
		buttons[AdvancedDockPlaces::bottom]->setBounds(bottomArea.toNearestInt());
	}

	AdvancedDockPlaces::Places getSelectionForCoordinates(Point<int> position) const
	{
		for (auto b : buttons)
			if (b->getBounds().contains(position) && b->isVisible())
				return b->place;

		return AdvancedDockPlaces::none;
	}

	void paint(Graphics & g) override
	{
		g.setColour(Colours::black);
		g.beginTransparencyLayer(0.4f);

		if (buttons[AdvancedDockPlaces::top]->isVisible() && buttons[AdvancedDockPlaces::left]->isVisible())
		{
			g.fillRoundedRectangle(topArea.expanded(padding), rounding);
			g.fillRoundedRectangle(midArea.expanded(padding), rounding);
			g.fillRoundedRectangle(bottomArea.expanded(padding), rounding);
		}
		else
		{
			g.fillRoundedRectangle(centreArea.expanded(padding), rounding);
		}

		g.endTransparencyLayer();
	}

private:
	Rectangle<float> topArea, midArea, bottomArea, centreArea;
	OwnedArray<PlacementButton> buttons;
	const float rounding = 4.0f;
	const float padding = 4.0f;
};

JAdvancedDock::JAdvancedDock(DockableWindowManager& manager_): DockBase(manager_, this),manager(manager_)
{
	placementDialog = new AdvancedDockPlacementDialog();
	addChildComponent(placementDialog);
}

JAdvancedDock::~JAdvancedDock()
{
}

JAdvancedDock::WindowLocation::WindowLocation(int y, int x, int t): y(y), x(x), tab(t)
{
}

JAdvancedDock::WindowLocation JAdvancedDock::getWindowLocationAtPoint(const Point<int>& screenPosition)
{
	auto localPos = getLocalPoint(nullptr, screenPosition);

	for (int y = 0; y < rows.size(); ++y)
	{
		auto& row = rows[y];

		for (int x = 0; x < row.columns.size(); ++x)
		{
			auto& col = row.columns[x];

			for (int z = 0; z < col.size(); ++z)
			{
				auto& tab = col[z];

				if (tab->getBounds().contains(localPos))
					return {y, x, z};
			}
		}
	}

	return {0,0,0};
}

Rectangle<int> JAdvancedDock::getWindowBoundsAtPoint(const Point<int>& p)
{
	auto loc = getWindowLocationAtPoint(p);

	if (rows.size() == 0)
		return getLocalBounds();

	return rows[loc.y].columns[loc.x][loc.tab]->getBounds();
}

void JAdvancedDock::showDockableComponentPlacement(DockableComponentWrapper* component, Point<int> screenPosition)
{
	placementDialog->setTopBottomVisible(rows.size() > 0);
	placementDialog->setLeftRightVisible(rows.size() > 0);

	placementDialog->setVisible(true);
	placementDialog->toFront(false);
	placementDialog->setBounds(getWindowBoundsAtPoint(screenPosition).withSizeKeepingCentre(100, 100));
	placementDialog->setMousePosition(screenPosition);
}

void JAdvancedDock::hideDockableComponentPlacement()
{
	placementDialog->setVisible(false);
}

void JAdvancedDock::startDockableComponentDrag(DockableComponentWrapper* component)
{
}

void JAdvancedDock::insertNewDock(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc)
{
	auto& row = rows[loc.y];
	RowType::TabDockType newTabDock;
	newTabDock.push_back(std::unique_ptr<DockableComponentWrapper>(comp));
	row.add(std::move(newTabDock), loc.x, this);
}

void JAdvancedDock::insertNewRow(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc)
{
	RowType newRow;
	newRow.columns.push_back(RowType::TabDockType());
	newRow.columns[0].push_back(std::unique_ptr<DockableComponentWrapper>(comp));
	rows.insert(rows.begin() + loc.y, std::move(newRow));
	rebuildRowResizers();
}

void JAdvancedDock::insertToNewTab(DockableComponentWrapper* comp, JAdvancedDock::WindowLocation loc)
{
	auto & location = rows[loc.y].columns[loc.x];
	location.push_back(std::unique_ptr<DockableComponentWrapper>(comp));
	RowType::configureTabs(location);
}

void JAdvancedDock::insertWindow(const Point<int>& screenPos, AdvancedDockPlaces::Places places, DockableComponentWrapper* comp)
{
	auto loc = getWindowLocationAtPoint(screenPos);

	switch (places)
	{
	case AdvancedDockPlaces::top:
		insertNewRow(comp, loc);
		break;

	case AdvancedDockPlaces::bottom:
		loc.y += rows.size() > 0 ? 1 : 0;
		insertNewRow(comp, loc);
		break;

	case AdvancedDockPlaces::left:
		insertNewDock(comp, loc);
		break;

	case AdvancedDockPlaces::right:
		loc.x++;
		insertNewDock(comp, loc);
		break;

	case AdvancedDockPlaces::centre:
		if (rows.size() > 0)
			insertToNewTab(comp, loc);
		else
			insertNewRow(comp, loc);
		break;

	case AdvancedDockPlaces::none:
		jassertfalse;
		break;
	}
}

void JAdvancedDock::addComponentToDock(Component* comp)
{
    auto dockable = manager.createDockableComponent(comp);
    addAndMakeVisible(dockable);
    auto loc = WindowLocation{0, 0, 0};
    
    if (rows.size() > 0)
        insertToNewTab(dockable, loc);
    else
        insertNewRow(dockable, loc);

    resized();
}

void JAdvancedDock::addComponentToNewRow(Component * component, int rowPosition)
{
    auto dockable = manager.createDockableComponent(component);
    addAndMakeVisible(dockable);
    
    if (rowPosition > rows.size())
        rowPosition = rows.size();
    
    auto loc = WindowLocation{rowPosition, 0, 0};
    
    insertNewRow(dockable, loc);
    
    resized();
}

bool JAdvancedDock::attachDockableComponent(DockableComponentWrapper* component, Point<int> screenPosition)
{
	auto placement = placementDialog->getSelectionForCoordinates(placementDialog->getLocalPoint(nullptr, screenPosition));

	if (placement != AdvancedDockPlaces::none)
	{
		addAndMakeVisible(component);
		insertWindow(screenPosition, placement, component);
		resized();
		return true;
	}


	return false;
}

void JAdvancedDock::detachDockableComponent(DockableComponentWrapper* component)
{
	for (int y = 0; y < rows.size(); ++y)
	{
		auto& row = rows[y];

		for (int x = 0; x < row.columns.size(); ++x)
		{
			auto& col = row.columns[x];

			for (int z = 0; z < col.size(); ++z)
			{
				auto& tab = col[z];

				if (tab.get() == component)
				{
					tab.release();

					col.erase(col.begin() + z);

					if (col.size() == 1)
					{
						/* remove tab buttons if we don't need them any more */
						col[0]->setShowTabButton(false, 0, false);
					}
					else if (col.size() == 0)
					{ 
						/* remove tabs, rows and columns if now empty... */
						row.columns.erase(row.columns.begin() + x);

						if (row.columns.size() == 0)
							rows.erase(rows.begin() + y);
					}

					row.rebuildResizers(this);
					rebuildRowResizers();
					resized();

					return;
				}
			}
		}
	}
}

void JAdvancedDock::revealComponent(DockableComponentWrapper* dockableComponent)
{
	dockableComponent->toFront(true);

	for (auto& r : rows)
		for (auto& c : r.columns)
			RowType::configureTabs(c);

	resized();
}

void JAdvancedDock::rebuildRowResizers()
{
	const double resizerSize = 5.0;
	resizers.clear();

	int itemIndex = 0;

	for (int pos = 0; pos < rows.size(); ++pos)
	{
		layout.setItemLayout(itemIndex++, 10.0, 2000.0, rows[0].columns[0][0]->getHeight());

		if (pos < rows.size() - 1)
		{
			resizers.push_back(std::make_unique<StretchableLayoutResizerBar>(&layout, pos * 2 + 1, false));
			addAndMakeVisible(resizers.back().get());
			layout.setItemLayout(itemIndex++, resizerSize, resizerSize, resizerSize);
		}
	}
}

void JAdvancedDock::layoutRows(const Rectangle<int>& area)
{
	std::vector<Component *> comps;

	for (int i = 0; i < rows.size(); ++i)
	{
		comps.push_back(rows[i].columns.front().front().get());

		if (i < resizers.size())
			comps.push_back(resizers[i].get());
	}

	if (comps.size() == 1)
	{
		comps[0]->setBounds(area); // layoutComponents doesn't seem to cope with only one component passed to it.
	}
	else
	{
		layout.layOutComponents(comps.data(), comps.size(),
		                         area.getX(), area.getY(), area.getWidth(), area.getHeight(), true, false);
	}
}

void JAdvancedDock::resized()
{
	if (rows.size() == 0)
		return;

	auto area = getLocalBounds();

	for (auto & resizer : resizers)
		resizer->setSize(area.getWidth(), 5);

	layoutRows(area);

	for (auto& row : rows)
	{
		if (row.columns.size() == 0)
			continue; // shouldn't happen, but just for safety...

		auto area2 = row.columns[0][0]->getBounds().withWidth(area.getWidth()).withX(0); // the first component will have had bounds set by the row resizer so we copy these over.

		row.layoutColumns(area2);
	}
}

void JAdvancedDock::paint(Graphics& g)
{
	if (rows.size() == 0)
	{
		g.fillAll(Colours::darkgrey);
		g.setColour(Colours::white);
		g.drawText("Advanced Dock", getLocalBounds(), Justification::centred, false);
	}
}
