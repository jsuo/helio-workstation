/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "ProjectMapScrollerScreen.h"
#include "ProjectMapScroller.h"

#define TRACK_SCROLLER_MINIMAP_HAS_ACTIVE_BORDER 0

ProjectMapScrollerScreen::ProjectMapScrollerScreen(ProjectMapScroller &scrollerRef) :
    colour(findDefaultColour(ColourIDs::TrackScroller::screenRangeFill)),
    scroller(scrollerRef)
{
    this->setPaintingIsUnclipped(true);
    this->setMouseClickGrabsKeyboardFocus(false);

    this->moveConstrainer.reset(new ComponentBoundsConstrainer());
    this->moveConstrainer->setMinimumSize(4, 4);
    this->moveConstrainer->setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);

#if TRACK_SCROLLER_MINIMAP_HAS_ACTIVE_BORDER
    this->resizeConstrainer.reset(new ResizeConstrainer(this->scroller));
    this->resizeConstrainer->setMinimumSize(4, 4);
    this->resizeConstrainer->setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);

    this->border.reset(new ResizableBorderComponent(this, this->resizeConstrainer));
    this->addAndMakeVisible(this->border);
    this->border->setBorderThickness(BorderSize<int>(3));
    this->border->setRepaintsOnMouseActivity(false);
#endif
}

//===----------------------------------------------------------------------===//
// Component
//===----------------------------------------------------------------------===//

void ProjectMapScrollerScreen::mouseDown(const MouseEvent &e)
{
    this->dragger.startDraggingComponent(this, e);
}

void ProjectMapScrollerScreen::mouseDrag(const MouseEvent &e)
{
    Point<float> lastPosition = this->getPosition().toFloat();
    this->dragger.dragComponent(this, e, this->moveConstrainer.get());
    const Point<float> moveDelta = this->getPosition().toFloat() - lastPosition;
    this->realBounds.translate(moveDelta.getX(), moveDelta.getY());
    this->scroller.xyMoveByUser();
}

void ProjectMapScrollerScreen::paint(Graphics &g)
{
    g.setColour(this->colour);
    g.fillRect(this->getLocalBounds());
}

void ProjectMapScrollerScreen::resized()
{
#if TRACK_SCROLLER_MINIMAP_HAS_ACTIVE_BORDER
    this->border->setBounds(this->getLocalBounds());
#endif
}

//===----------------------------------------------------------------------===//
// Constrainers
//===----------------------------------------------------------------------===//

void ProjectMapScrollerScreen::ResizeConstrainer::applyBoundsToComponent(Component &component,
        Rectangle<int> bounds)
{
    ComponentBoundsConstrainer::applyBoundsToComponent(component, bounds);
    this->scroller.resizeByUser();
    this->scroller.xyMoveByUser();
}
