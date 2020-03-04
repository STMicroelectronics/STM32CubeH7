#include <gui/mainmenucarousel_screen/MainMenuCarouselView.hpp>
#include <BitmapDatabase.hpp>

MainMenuCarouselView::MainMenuCarouselView() :
    currentState(NO_STATE),
    carouselAnimationEnded(this, &MainMenuCarouselView::carouselAnimationEndedhandler),
    dragDistance(0),
    dragInsideCarousel(false),
    dragInsideCenterButton(false),
    hasAnimatedDrag(false)
{
}

void MainMenuCarouselView::setupScreen()
{
    setupCarousel();
    add(carousel);
}

void MainMenuCarouselView::tearDownScreen()
{
}

void MainMenuCarouselView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        dragDistance = 0;
        hasAnimatedDrag = false;
        dragInsideCarousel = carousel.getAbsoluteRect().intersect(evt.getX(), evt.getY());
        dragInsideCenterButton = Rect(carousel.getX() + ((carousel.getWidth() - 150) / 2), carousel.getY() + ((carousel.getHeight() - 150) / 2), 150, 150).intersect(evt.getX(), evt.getY());
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        if (dragInsideCarousel && !hasAnimatedDrag)
        {
            if (dragInsideCenterButton)
            {
                currentState = ELEMENT_SELECTED;
                pauses = 4;
                carousel.highlightSelectedElement();
            }
            else if (evt.getX() < carousel.getX() + carousel.getWidth() / 2)
            {
                carousel.selectedPrevious();
            }
            else
            {
                carousel.selectedNext();
            }
        }
    }
    View::handleClickEvent(evt);
}

void MainMenuCarouselView::handleDragEvent(const DragEvent& evt)
{
    View::handleDragEvent(evt);

    if (dragInsideCarousel)
    {
        dragDistance += evt.getDeltaX();

        if (abs(dragDistance) > 30)
        {
            dragDistance = 0;
            hasAnimatedDrag = true;
            if (abs(evt.getDeltaX()) > 0)
            {
                if (evt.getDeltaX() < 0)
                {
                    carousel.selectedNext();
                }
                else
                {
                    carousel.selectedPrevious();
                }
            }
        }
    }
}

void MainMenuCarouselView::setupCarousel()
{
    carousel.setPosition(172, 192, 450, 180);
    carousel.setCarouselAnimationEndedCallback(carouselAnimationEnded);

    // The y position difference between the images
    uint16_t yDiff = 4;

    uint16_t widthOfSmallElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_SMALL_ID).getWidth();
    uint16_t widthOfMediumElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_MEDIUM_ID).getWidth();
    uint16_t widthOfLargeElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_LARGE_ID).getWidth();
    uint16_t widthOfInvisibleElements = widthOfSmallElements - 30;

    uint16_t heightOfSmallElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_SMALL_ID).getHeight();
    uint16_t heightOfMediumElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_MEDIUM_ID).getHeight();
    uint16_t heightOfLargeElements = Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_LARGE_ID).getHeight();
    uint16_t heightOfInvisibleElements = heightOfSmallElements - 30;

    uint16_t offsetWidthForInvisibleEndElements = 20;
    uint16_t visibleWidthOfSmallElements = 50;
    uint16_t visibleWidthOfMediumElements = 80;

    carousel.setPositionOfCarouselElement(0, 0,
                                          (3 * yDiff), widthOfInvisibleElements, heightOfInvisibleElements);

    carousel.setPositionOfCarouselElement(1, offsetWidthForInvisibleEndElements,
                                          (2 * yDiff), widthOfSmallElements, heightOfSmallElements);

    carousel.setPositionOfCarouselElement(2, carousel.getXPositionOfVisibleElement(1) + visibleWidthOfSmallElements,
                                          yDiff + 4, widthOfMediumElements, heightOfMediumElements);

    carousel.setPositionOfCarouselElement(3, carousel.getXPositionOfVisibleElement(2) + visibleWidthOfMediumElements,
                                          0, widthOfLargeElements, heightOfLargeElements);

    carousel.setPositionOfCarouselElement(4, carousel.getXPositionOfVisibleElement(3) + widthOfLargeElements - widthOfMediumElements + visibleWidthOfMediumElements,
                                          yDiff + 4, widthOfMediumElements, heightOfMediumElements);

    carousel.setPositionOfCarouselElement(5, carousel.getXPositionOfVisibleElement(4) + widthOfMediumElements - widthOfSmallElements + visibleWidthOfSmallElements,
                                          (2 * yDiff), widthOfSmallElements, heightOfSmallElements);

    carousel.setPositionOfCarouselElement(6, carousel.getXPositionOfVisibleElement(5) + offsetWidthForInvisibleEndElements + 20,
                                          (3 * yDiff), widthOfInvisibleElements, heightOfInvisibleElements);

    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_2048_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_CONTROLS_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_CONTROLS_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_CONTROLS_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_LARGE_ID));

    demoIDs[0] = Defines::GAME2048;
    headlines[0] = T_CAROUSEL_GAME_2048_HEADLINE;
    texts[0] = T_CAROUSEL_GAME_2048_TEXT;

    demoIDs[1] = Defines::CONTROLS;
    headlines[1] = T_CAROUSEL_CONTROLS_HEADLINE;
    texts[1] = T_CAROUSEL_CONTROLS_TEXT;

    demoIDs[2] = Defines::GAME2D;
    headlines[2] = T_CAROUSEL_GAME_2D_HEADLINE;
    texts[2] = T_CAROUSEL_GAME_2D_TEXT;

    demoIDs[3] = Defines::HOME_AUTOMATION;
    headlines[3] = T_CAROUSEL_HOME_AUTOMATION_HEADLINE;
    texts[3] = T_CAROUSEL_HOME_AUTOMATION_TEXT;

    demoIDs[4] = Defines::SETTINGS;
    headlines[4] = T_CAROUSEL_SETTINGS_HEADLINE;
    texts[4] = T_CAROUSEL_SETTINGS_TEXT;

    demoIDs[5] = Defines::GRAPHICS_EFFECT;
    headlines[5] = T_CAROUSEL_GRAPHICS_EFFECT_HEADLINE;
    texts[5] = T_CAROUSEL_GRAPHICS_EFFECT_TEXT;

    // Set the carousel to the element that was selected last
    for (int i = 0; i < Defines::NUMBER_OF_DEMO_SCREENS; i++)
    {
        // Map demoId to position in carousel
        if (demoIDs[i] == presenter->getSelectedDemoScreen())
        {
            carousel.setSelectedElement(i);
        }
    }

#ifdef SIMULATOR
    carousel.setDuration(14);
#else
    carousel.setDuration(4);
#endif

    updateDemoText();

    if (presenter->getPreviousSelectedMenuType() != Defines::NO_MENU)
    {
        // Returning to the menu from a demo screen (and not toggle menu) so
        // setup the carousel to the right element and animate out.
        carousel.setHighlightedNoAnimation();
        carousel.cancelHighlightOfSelectedElement();
    }

    presenter->setPreviousSelectedMenuType(Defines::CAROUSEL_MENU);
}

void MainMenuCarouselView::carouselAnimationEndedhandler(const Carousel& c)
{
    if (currentState == ELEMENT_SELECTED)
    {
        if (pauses-- > 0)
        {
            // Pauses implemented by just re initiating some highlight requests
            carousel.highlightSelectedElement();
        }
        else
        {
            setSelectedDemoScreen(demoIDs[carousel.getSelectedElementIndex()]);
            gotoSelectedDemoScreen();
        }
    }
    else
    {
        // Carousel moved to previous or next element
        updateDemoText();
    }
}

void MainMenuCarouselView::updateDemoText()
{
    demoHeadline.setTypedText(TypedText(headlines[carousel.getSelectedElementIndex()]));
    demoText.setTypedText(TypedText(texts[carousel.getSelectedElementIndex()]));
    demoHeadline.invalidate();
    demoText.invalidate();
}

void MainMenuCarouselView::screenSaverMinorTick()
{
    carousel.selectedPrevious();
}

void MainMenuCarouselView::setSelectedDemoScreen(Defines::DemoID demoId)
{
    selectedDemoScreen = demoId;
}

void MainMenuCarouselView::gotoSelectedDemoScreen()
{
    presenter->setSelectedDemoScreen(selectedDemoScreen);

    switch (selectedDemoScreen)
    {
    case Defines::GAME2048:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2048ScreenNoTransition();
        break;
    case Defines::GAME2D:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2DScreenNoTransition();
        break;
    case Defines::CONTROLS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoControlsScreenNoTransition();
        break;
    case Defines::HOME_AUTOMATION:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeAutomationScreenNoTransition();
        break;
    case Defines::SETTINGS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoSettingsScreenNoTransition();
        break;
    case Defines::GRAPHICS_EFFECT:
        static_cast<FrontendApplication*>(Application::getInstance())->gotographics_effectScreenNoTransition();
        break;
    case Defines::NUMBER_OF_DEMO_SCREENS:
    case Defines::NO_DEMO_SCREEN:
    default:
        break;
    }
}

void MainMenuCarouselView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}
