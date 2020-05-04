


#include <gui/main_menu_screen/MainMenuCarouselView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Color.hpp>

MainMenuCarouselView::MainMenuCarouselView() :
    MainMenuView(), 
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
    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_CAROUSEL_BACKGROUND_ID));
    add(background);

    if (EMBED_IN_ST_DEMO)
    {
        mcuLoadTxt.setXY(620, 328);
        mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right() - 6, mcuLoadTxt.getY(), 50, 30);
        toggleMenuButton.setXY(710, 32);
        add(gotoSTMenuButton);
    }

    add(toggleMenuButton);

    setupCarousel();
    add(carousel);

    demoHeadline.setPosition(0, 366, 800, 34);
    demoHeadline.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(demoHeadline); 

    demoText.setPosition(50, 400, 700, 90);
    demoText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(demoText);

    add(mcuLoadTxt);
    add(mcuLoadValueTxt);
    bg.setPosition(0, 0, 800, 480);
    bg.setColor(0xFF);
  //  add(bg);
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
    dragInsideCarousel = carousel.getAbsoluteRect().intersect(evt.getX(),evt.getY());
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
      else if (evt.getX() < carousel.getX() + carousel.getWidth()/2)
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
        (2* yDiff), widthOfSmallElements, heightOfSmallElements);

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
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_AUDIO_PLAYER_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_AUDIO_PLAYER_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_AUDIO_PLAYER_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GAME_2D_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_HOME_AUTOMATION_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_SETTINGS_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_VIDEO_PLAYER_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_VIDEO_PLAYER_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_VIDEO_PLAYER_LARGE_ID));
    carousel.addElement(Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_SMALL_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_MEDIUM_ID), Bitmap(BITMAP_CAROUSEL_MENU_ICONS_GRAPHICS_EFFECT_LARGE_ID));


    demoIDs[0] = Defines::GAME2048;
    headlines[0] = T_CAROUSEL_GAME_2048_HEADLINE;
    texts[0] = T_CAROUSEL_GAME_2048_TEXT;

    demoIDs[1] = Defines::CONTROLS;
    headlines[1] = T_CAROUSEL_CONTROLS_HEADLINE;
    texts[1] = T_CAROUSEL_CONTROLS_TEXT;

    demoIDs[2] = Defines::AUDIO_PLAYER;
    headlines[2] = T_CAROUSEL_AUDIO_PLAYER_HEADLINE;
    texts[2] = T_CAROUSEL_AUDIO_PLAYER_TEXT;

    demoIDs[3] = Defines::GAME2D;
    headlines[3] = T_CAROUSEL_GAME_2D_HEADLINE;
    texts[3] = T_CAROUSEL_GAME_2D_TEXT;

    demoIDs[4] = Defines::HOME_AUTOMATION;
    headlines[4] = T_CAROUSEL_HOME_AUTOMATION_HEADLINE;
    texts[4] = T_CAROUSEL_HOME_AUTOMATION_TEXT;

    demoIDs[5] = Defines::SETTINGS;
    headlines[5] = T_CAROUSEL_SETTINGS_HEADLINE;
    texts[5] = T_CAROUSEL_SETTINGS_TEXT;

    demoIDs[6] = Defines::VIDEO_PLAYER;
    headlines[6] = T_CAROUSEL_VIDEO_PLAYER_HEADLINE;
    texts[6] = T_CAROUSEL_VIDEO_PLAYER_TEXT;

    demoIDs[7] = Defines::GRAPHICS_EFFECT;
    headlines[7] = T_CAROUSEL_GRAPHICS_EFFECT_HEADLINE;
    texts[7] = T_CAROUSEL_GRAPHICS_EFFECT_TEXT;

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
    carousel.setDuration(4);
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

void MainMenuCarouselView::toggleMenu()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreen();
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
}

void MainMenuCarouselView::caruselMove(bool right)
{
    if (right)
    {
        carousel.selectedNext();
    }
    else
    {
        carousel.selectedPrevious();
    }
}
void MainMenuCarouselView::caruselSelectElement()
{
    ClickEvent temp_evt(ClickEvent::RELEASED, 391, 276);

    dragInsideCarousel = true;
    dragInsideCenterButton = true;
    hasAnimatedDrag = false;
    handleClickEvent(temp_evt);
}
