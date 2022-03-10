/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "SwipeContainerWithCallback.hpp"
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
    SwipeContainerWithCallback::SwipeContainerWithCallback() :
        Container(),
        currentState(NO_ANIMATION),
        numberOfPages(0),
        animationCounter(0),
        swipeCutoff(80),
        dragX(0),
        animateDistance(0),
        startX(0),
        currentPage(0),
        endElasticWidth(30),
        pages(EAST),
        pageIndicator()
    {
        touchgfx::Application::getInstance()->registerTimerWidget(this);

        setTouchable(true);

        Container::add(pages);
        Container::add(pageIndicator);
    }

    SwipeContainerWithCallback::~SwipeContainerWithCallback()
    {
        touchgfx::Application::getInstance()->unregisterTimerWidget(this);
    }

    void SwipeContainerWithCallback::add(Drawable& page)
    {
        pages.add(page);
        numberOfPages++;

        pageIndicator.setNumberOfPages(numberOfPages);

        setWidth(page.getWidth());
        setHeight(page.getHeight());
    }

    void SwipeContainerWithCallback::remove(Drawable& page)
    {
        Drawable* tmp = pages.getFirstChild();

        if (!numberOfPages)
        {
            return;
        }

        // looks for the child matching page
        // to ensure that the page indicator only counts down if a page is removed
        while (tmp)
        {
            if (tmp == &page)
            {
                pages.remove(page);
                numberOfPages--;

                if (!numberOfPages)
                {
                    setWidth(0);
                    setHeight(0);
                }
                else
                {
                    pageIndicator.setNumberOfPages(numberOfPages);
                }
                return;
            }
            else
            {
                tmp = tmp->getNextSibling();
            }
        }
    }

    void SwipeContainerWithCallback::setEndSwipeElasticWidth(uint16_t width)
    {
        endElasticWidth = width;
    }

    void SwipeContainerWithCallback::setSwipeCutoff(uint16_t cutoff)
    {
        swipeCutoff = cutoff;
    }

    void SwipeContainerWithCallback::setPageIndicatorBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage)
    {
        pageIndicator.setBitmaps(normalPage, highlightedPage);
    }

    void SwipeContainerWithCallback::setPageIndicatorXY(int16_t x, int16_t y)
    {
        pageIndicator.setXY(x, y);
    }

    void SwipeContainerWithCallback::setPageIndicatorXYWithCenteredX(int16_t x, int16_t y)
    {
        pageIndicator.setXY(x - pageIndicator.getWidth() / 2, y);
    }

    void SwipeContainerWithCallback::setSelectedPage(uint8_t pageIndex)
    {
        currentPage = pageIndex;
        pageIndicator.setHighlightPosition(currentPage);
        adjustPages();
    }

    void SwipeContainerWithCallback::handleTickEvent()
    {
        if (currentState == ANIMATE_SWIPE_CANCELLED_LEFT)
        {
            animateSwipeCancelledLeft();
        }
        else if (currentState == ANIMATE_SWIPE_CANCELLED_RIGHT)
        {
            animateSwipeCancelledRight();
        }
        else if (currentState == ANIMATE_LEFT)
        {
            animateLeft();
        }
        else if (currentState == ANIMATE_RIGHT)
        {
            animateRight();
        }
    }

    void SwipeContainerWithCallback::handleClickEvent(const ClickEvent& evt)
    {
        // If an animation is already in progress do not
        // react to clicks
        if (currentState != NO_ANIMATION)
        {
            return;
        }

        if (evt.getType() == ClickEvent::RELEASED)
        {
            // Save current position for use during animation
            animateDistance = dragX;
            startX = pages.getX();

            if (dragX < 0)
            {
                if (currentPage == getNumberOfPages() - 1 || dragX > -swipeCutoff)
                {
                    currentState = ANIMATE_SWIPE_CANCELLED_LEFT;
                }
                else
                {
                    currentState = ANIMATE_LEFT;
                }
            }
            else if (dragX > 0)
            {
                if (currentPage == 0 || dragX < swipeCutoff)
                {
                    currentState = ANIMATE_SWIPE_CANCELLED_RIGHT;
                }
                else
                {
                    currentState = ANIMATE_RIGHT;
                }
            }
        }
    }

    void SwipeContainerWithCallback::handleDragEvent(const DragEvent& evt)
    {
        // If an animation is already in progress do not
        // react to drags
        if (currentState != NO_ANIMATION)
        {
            return;
        }

        dragX += evt.getDeltaX();

        // Do not show too much background next to end pages
        if (currentPage == 0 && dragX > endElasticWidth)
        {
            dragX = static_cast<int16_t>(endElasticWidth);
        }
        else if (currentPage == getNumberOfPages() - 1 && dragX < -endElasticWidth)
        {
            dragX = -static_cast<int16_t>(endElasticWidth);
        }

        adjustPages();
    }

    void SwipeContainerWithCallback::handleGestureEvent(const GestureEvent& evt)
    {
        // Do not accept gestures while animating
        if (currentState != NO_ANIMATION)
        {
            return;
        }

        if (evt.getType() == evt.SWIPE_HORIZONTAL)
        {
            // Save current position for use during animation
            animateDistance = dragX;
            startX = pages.getX();

            if (evt.getVelocity() < 0 && currentPage < getNumberOfPages() - 1)
            {
                currentState = ANIMATE_LEFT;
            }
            else if (evt.getVelocity() > 0 && currentPage > 0)
            {
                currentState = ANIMATE_RIGHT;
            }
        }
    }

    void SwipeContainerWithCallback::adjustPages()
    {
        pages.moveTo(-static_cast<int16_t>(currentPage * getWidth()) + dragX, 0);
    }

    void SwipeContainerWithCallback::animateSwipeCancelledLeft()
    {
        uint8_t duration = 14;

        if (animationCounter <= duration)
        {
            int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, -animateDistance, duration);
            dragX = animateDistance + delta;

            adjustPages();
        }
        else
        {
            // Final step: stop the animation
            currentState = NO_ANIMATION;
            animationCounter = 0;
            dragX = 0;
            adjustPages();

            if (newCenterCanceldCallback->isValid())
            {
                newCenterCanceldCallback->execute();
            }
        }
        animationCounter++;
    }

    void SwipeContainerWithCallback::animateSwipeCancelledRight()
    {
        uint8_t duration = 14;

        if (animationCounter <= duration)
        {
            int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, animateDistance, duration);
            dragX = animateDistance - delta;

            adjustPages();
        }
        else
        {
            // Final step: stop the animation
            currentState = NO_ANIMATION;
            animationCounter = 0;
            dragX = 0;
            adjustPages();

            if (newCenterCanceldCallback->isValid())
            {
                newCenterCanceldCallback->execute();
            }
        }
        animationCounter++;
    }

    void SwipeContainerWithCallback::animateLeft()
    {
        uint8_t duration = 10;

        if (animationCounter <= duration)
        {
            int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() + animateDistance, duration);
            dragX = animateDistance - delta;

            adjustPages();
        }
        else
        {
            // Final step: stop the animation
            currentState = NO_ANIMATION;
            animationCounter = 0;
            currentPage++;
            dragX = 0;
            adjustPages();
            pageIndicator.goRight();

            if (newCenterCallback->isValid())
            {
                newCenterCallback->execute(true);
            }
        }
        animationCounter++;
    }

    void SwipeContainerWithCallback::animateRight()
    {
        uint8_t duration = 10;

        if (animationCounter <= duration)
        {
            int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() - animateDistance, duration);
            dragX = animateDistance + delta;

            adjustPages();
        }
        else
        {
            // Final step: stop the animation
            currentState = NO_ANIMATION;
            animationCounter = 0;
            currentPage--;
            dragX = 0;
            adjustPages();
            pageIndicator.goLeft();

            if (newCenterCallback->isValid())
            {
                newCenterCallback->execute(false);
            }
        }
        animationCounter++;
    }

    SwipeContainerWithCallback::PageIndicator::PageIndicator() :
        Container(),
        numberOfPages(0),
        currentPage(0)
    {
        unselectedPages.setXY(0, 0);
        selectedPage.setXY(0, 0);

        Container::add(unselectedPages);
        Container::add(selectedPage);
    }

    SwipeContainerWithCallback::PageIndicator::~PageIndicator()
    {
    }

    void SwipeContainerWithCallback::PageIndicator::setNumberOfPages(uint8_t size)
    {
        numberOfPages = size;

        assert(numberOfPages > 0 && "At least one dot is needed");

        numberOfPages = size;

        if (unselectedPages.getBitmap() != BITMAP_INVALID)
        {
            int dotWidth = Bitmap(unselectedPages.getBitmap()).getWidth();
            unselectedPages.setWidth(dotWidth * size);

            // adjust size of container according to the actual bitmaps
            setWidth(unselectedPages.getWidth());
            setHeight(unselectedPages.getHeight());
            setHighlightPosition(currentPage = 0);

            invalidate();
        }
    }

    void SwipeContainerWithCallback::PageIndicator::setBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage)
    {
        selectedPage.setBitmap(highlightedPage);
        unselectedPages.setBitmap(normalPage);
        if (numberOfPages > 0)
        {
            setNumberOfPages(numberOfPages);
        }
    }

    void SwipeContainerWithCallback::PageIndicator::goRight()
    {
        setHighlightPosition(currentPage = (currentPage + 1) % numberOfPages);
    }

    void SwipeContainerWithCallback::PageIndicator::goLeft()
    {
        setHighlightPosition(currentPage = (currentPage + numberOfPages - 1) % numberOfPages);
    }

    void SwipeContainerWithCallback::PageIndicator::setHighlightPosition(uint8_t index)
    {
        currentPage = index;
        // note that index is unsigned
        if (index < numberOfPages)
        {
            int dotWidth = Bitmap(unselectedPages.getBitmap()).getWidth();
            selectedPage.setX(index * dotWidth);
        }
        invalidate();
    }

    void SwipeContainerWithCallback::setNewCenterAction(GenericCallback<bool>& callback)
    {
        newCenterCallback = &callback;
    }

    void SwipeContainerWithCallback::setNewCenterCanceledAction(GenericCallback<>& callback)
    {
        newCenterCanceldCallback = &callback;
    }
}
