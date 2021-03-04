/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef SLIDEMENU_HPP
#define SLIDEMENU_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * @class SlideMenu touchgfx/containers/SlideMenu.hpp
 *
 * @brief SlideMenu is a container that has the functionality of being either collapsed or expanded.
 *
 *        SlideMenu is a container that has the functionality of being either collapsed or
 *        expanded. The SlideMenu consists of a background and a activate button that toggles
 *        the SlideMenus collapsed/expanded state.
 *
 *        The relative positions of the background and state change button is configurable as
 *        is the direction in which the SlideMenu expands and collapses. How much of the
 *        SlideMenu that is visible when collapsed can be set with the
 *        setVisiblePixelsWhenCollapsed(..) method. It is, of course, important that the state
 *        change button is accessible when collapsed. The SlideMenu will animate back to the
 *        collapsed state after a expandedStateTimeout is reached. The timer can be reset, for
 *        example when the user interacts with elements in the list. Use the
 *        resetExpandedStateTimer(..) method for this.
 *
 *        Actual menu elements are added normally
 *        using the add(..) method and are positioned relative to the SlideMenu.
 */
class SlideMenu : public Container
{
public:

    /**
     * @enum State
     *
     * @brief Values that represent the SlideMenu states.
     */
    enum State
    {
        COLLAPSED,
        EXPANDED
    };

    /**
     * @enum ExpandDirection
     *
     * @brief Values that represent the expand directions.
     */
    enum ExpandDirection
    {
        SOUTH,
        NORTH,
        EAST,
        WEST
    };

    /**
     * @fn SlideMenu::SlideMenu();
     *
     * @brief Default constructor.
     */
    SlideMenu();

    /**
     * @fn virtual SlideMenu::~SlideMenu();
     *
     * @brief Destructor.
     */
    virtual ~SlideMenu();

    /**
     * @fn virtual void SlideMenu::setup(SlideMenu::ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP);
     *
     * @brief Setup the SlideMenu by positioning the stateChangeButton next to background image
     *        relative to the expand direction and center it in the other dimension.
     *
     *        Setup the SlideMenu by positioning the stateChangeButton next to background image
     *        relative to the expand direction and center it in the other dimension. The width and
     *        height of the SlideMenu will be automatically set to span both elements. Default
     *        values are: expandedStateTimeout = 200, visiblePixelsWhenCollapsed = 0,
     *        hiddenPixelsWhenExpanded = 0, animationDuration = 10, animationEquation =
     *        cubicEaseInOut.
     *
     * @param newExpandDirection          The new expand direction.
     * @param backgroundBMP               The background bitmap.
     * @param stateChangeButtonBMP        The state change button bitmap.
     * @param stateChangeButtonPressedBMP The state change button pressed bitmap.
     */
    virtual void setup(SlideMenu::ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP);

    /**
     * @fn virtual void SlideMenu::setup(SlideMenu::ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP, int16_t backgroundX, int16_t backgroundY, int16_t stateChangeButtonX, int16_t stateChangeButtonY);
     *
     * @brief Setup method for the SlideMenu. Positioning of the background image and the
     *        stateChangeButton is done by stating the X and Y coordinates for the elements
     *        (relative to the SlideMenu).
     *
     *        Setup method for the SlideMenu. Positioning of the background image and the
     *        stateChangeButton is done by stating the X and Y coordinates for the elements
     *        (relative to the SlideMenu). The width and height of the SlideMenu will be
     *        automatically set to span both elements. Default values are: expandedStateTimeout =
     *        200, visiblePixelsWhenCollapsed = 0, hiddenPixelsWhenExpanded = 0, animationDuration
     *        = 10, animationEquation = cubicEaseInOut.
     *
     * @param newExpandDirection          The new expand direction.
     * @param backgroundBMP               The background bitmap.
     * @param stateChangeButtonBMP        The state change button bitmap.
     * @param stateChangeButtonPressedBMP The state change button pressed bitmap.
     * @param backgroundX                 The background x coordinate.
     * @param backgroundY                 The background y coordinate.
     * @param stateChangeButtonX          The state change button x coordinate.
     * @param stateChangeButtonY          The state change button y coordinate.
     */
    virtual void setup(SlideMenu::ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP, int16_t backgroundX, int16_t backgroundY, int16_t stateChangeButtonX, int16_t stateChangeButtonY);

    /**
     * @fn virtual void SlideMenu::setExpandDirection(SlideMenu::ExpandDirection newExpandDirection);
     *
     * @brief Sets the expand direction.
     *
     * @param newExpandDirection The new expand direction.
     */
    virtual void setExpandDirection(SlideMenu::ExpandDirection newExpandDirection);

    /**
     * @fn virtual SlideMenu::ExpandDirection SlideMenu::getExpandDirection() const
     *
     * @brief Gets the expand direction.
     *
     * @return The expand direction.
     */
    virtual SlideMenu::ExpandDirection getExpandDirection() const;

    /**
     * @fn virtual void SlideMenu::setVisiblePixelsWhenCollapsed(int16_t visiblePixels);
     *
     * @brief Sets the amount of visible pixels when collapsed.
     *
     * @param visiblePixels The visible pixels.
     */
    virtual void setVisiblePixelsWhenCollapsed(int16_t visiblePixels);

    /**
     * @fn virtual int16_t SlideMenu::getVisiblePixelsWhenCollapsed() const
     *
     * @brief Gets the visible pixels when collapsed.
     *
     * @return The visible pixels when collapsed.
     */
    virtual int16_t getVisiblePixelsWhenCollapsed() const;

    /**
     * @fn virtual void SlideMenu::setHiddenPixelsWhenExpanded(int16_t hiddenPixels);
     *
     * @brief Sets the amount of hidden pixels when expanded.
     *
     * @param hiddenPixels The hidden pixels.
     */
    virtual void setHiddenPixelsWhenExpanded(int16_t hiddenPixels);

    /**
     * @fn virtual int16_t SlideMenu::getHiddenPixelsWhenExpanded() const
     *
     * @brief Gets the hidden pixels when expanded.
     *
     * @return The hidden pixels when expanded.
     */
    virtual int16_t getHiddenPixelsWhenExpanded() const;

    /**
     * @fn virtual void SlideMenu::setExpandedStateTimeout(uint16_t timeout)
     *
     * @brief Sets the expanded state timeout in ticks.
     *
     *        Sets the expanded state timeout in ticks. The SlideMenu will animate to the COLLAPSED
     *        state when this number of ticks has been executed while the SlideMenu is in the
     *        EXPANDED state. The timer can be reset with the resetExpandedStateTimer method.
     *
     * @param timeout The timeout in ticks.
     */
    virtual void setExpandedStateTimeout(uint16_t timeout);

    /**
     * @fn virtual uint16_t SlideMenu::getExpandedStateTimeout() const
     *
     * @brief Gets expanded state timeout.
     *
     * @return The expanded state timeout.
     */
    virtual uint16_t getExpandedStateTimeout() const;

    /**
     * @fn virtual void SlideMenu::setAnimationDuration(uint16_t duration)
     *
     * @brief Sets the animation duration.
     *
     * @param duration The animation duration.
     */
    virtual void setAnimationDuration(uint16_t duration);

    /**
     * @fn virtual uint16_t SlideMenu::getAnimationDuration() const
     *
     * @brief Gets the animation duration.
     *
     * @return The animation duration.
     */
    virtual uint16_t getAnimationDuration() const;

    /**
     * @fn virtual void SlideMenu::setAnimationEasingEquation(EasingEquation animationEasingEquation)
     *
     * @brief Sets the animation easing equation.
     *
     * @param animationEasingEquation The animation easing equation.
     */
    virtual void setAnimationEasingEquation(EasingEquation animationEasingEquation);

    /**
     * @fn virtual EasingEquation SlideMenu::getAnimationEasingEquation() const
     *
     * @brief Gets the animation easing equation.
     *
     * @return The animation easing equation.
     */
    virtual EasingEquation getAnimationEasingEquation() const;

    /**
     * @fn virtual void SlideMenu::setState(SlideMenu::State newState);
     *
     * @brief Sets the state of the SlideMenu. No animation is performed.
     *
     * @param newState The new state of the SlideMenu.
     */
    virtual void setState(SlideMenu::State newState);

    /**
     * @fn virtual void SlideMenu::animateToState(SlideMenu::State newState);
     *
     * @brief Animate to the given state.
     *
     * @param newState The new state of the SlideMenu.
     */
    virtual void animateToState(SlideMenu::State newState);

    /**
     * @fn virtual SlideMenu::State SlideMenu::getState()
     *
     * @brief Gets the current state.
     *
     * @return The current state.
     */
    virtual SlideMenu::State getState();

    /**
     * @fn virtual void SlideMenu::resetExpandedStateTimer()
     *
     * @brief Resets the expanded state timer.
     *
     *        Resets the expanded state timer. The SlideMenu will animate to the COLLAPSED state
     *        after a number of ticks (set with setExpandedStateTimeout(..)). This method resets
     *        this timer.
     */
    virtual void resetExpandedStateTimer();

    /**
     * @fn virtual uint16_t SlideMenu::getExpandedStateTimer() const
     *
     * @brief Gets the expanded state timer.
     *
     * @return The expanded state timer.
     */
    virtual uint16_t getExpandedStateTimer() const;

    /**
     * @fn virtual int16_t SlideMenu::getBackgroundX() const
     *
     * @brief Gets the background x coordinate.
     *
     * @return The background x coordinate.
     */
    virtual int16_t getBackgroundX() const;

    /**
     * @fn virtual int16_t SlideMenu::getBackgroundY() const
     *
     * @brief Gets the background y coordinate.
     *
     * @return The background y coordinate.
     */
    virtual int16_t getBackgroundY() const;

    /**
     * @fn virtual int16_t SlideMenu::getStateChangeButtonX() const
     *
     * @brief Gets the state change button x coordinate.
     *
     * @return The state change button x coordinate.
     */
    virtual int16_t getStateChangeButtonX() const;

    /**
     * @fn virtual int16_t SlideMenu::getStateChangeButtonY() const
     *
     * @brief Gets the state change button y coordinate.
     *
     * @return The state change button y coordinate.
     */
    virtual int16_t getStateChangeButtonY() const;

    /**
     * @fn virtual void SlideMenu::setStateChangedCallback(GenericCallback< const SlideMenu& >& callback)
     *
     * @brief Set the state changed callback. This callback is called when the state change button
     *        is clicked.
     *
     * @param callback The callback.
     */
    virtual void setStateChangedCallback(GenericCallback< const SlideMenu& >& callback);

    /**
     * @fn virtual void SlideMenu::setStateChangedAnimationEndedCallback(GenericCallback< const SlideMenu& >& callback)
     *
     * @brief Set the state change animation ended callback. This callback is called when a state
     *        change animation has ended.
     *
     * @param callback The callback.
     */
    virtual void setStateChangedAnimationEndedCallback(GenericCallback< const SlideMenu& >& callback);

    /**
     * @fn virtual void SlideMenu::add(Drawable& d);
     *
     * @brief Adds a drawable to the container.
     *
     * @param d The drawable to add.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void SlideMenu::remove(Drawable& d);
     *
     * @brief Removes the drawable from the container.
     *
     * @param d The drawable to remove.
     */
    virtual void remove(Drawable& d);

protected:
    MoveAnimator<Container> menuContainer;       ///< The container holding the actual menu items. This is the container that performs the state change animation
    Button stateChangeButton;                    ///< The state change button that toggles the SlideMenu state
    Image background;                            ///< The background of the SlideMenu

    Callback<SlideMenu, const AbstractButton&> onStateChangeButtonClicked;       ///< The local state changed button clicked callback
    Callback<SlideMenu, const MoveAnimator<Container>&> animationEndedCallback;  ///< The local state changed animation ended callback

    GenericCallback<const SlideMenu& >* stateChangedCallback;                ///< The public state changed button clicked callback
    GenericCallback<const SlideMenu& >* stateChangedAnimationEndedCallback;  ///< The public state changed animation ended callback

    SlideMenu::State currentState;               ///< The current state of the SlideMenu
    SlideMenu::ExpandDirection expandDirection;  ///< The expand direction of the SlideMenu

    EasingEquation animationEquation;            ///< The easing equation used for the state change animation

    int16_t visiblePixelsWhenCollapsed;          ///< The number of visible pixels when collapsed
    int16_t hiddenPixelsWhenExpanded;            ///< The number of hidden pixels when expanded

    uint16_t expandedStateTimeout;               ///< The expanded state timeout.
    uint16_t expandedStateTimer;                 ///< The timer that counts towards the expandedStateTimeout. If reached the SlideMenu will animate to COLLAPSED.

    uint16_t animationDuration;                  ///< The animation duration of the state change animation

    /**
     * @fn void SlideMenu::stateChangeButtonClickedHandler(const AbstractButton& button);
     *
     * @brief Handler for the state change button clicked event.
     *
     * @param button The state change button.
     */
    void stateChangeButtonClickedHandler(const AbstractButton& button);

    /**
     * @fn void SlideMenu::animationEndedHandler(const MoveAnimator<Container>& container);
     *
     * @brief Handler for the state change animation ended event.
     *
     * @param container The menuContainer.
     */
    void animationEndedHandler(const MoveAnimator<Container>& container);

    /**
     * @fn virtual void SlideMenu::handleTickEvent();
     *
     * @brief Handles the tick event.
     */
    virtual void handleTickEvent();

    /**
     * @fn virtual int16_t SlideMenu::getCollapsedXCoordinate();
     *
     * @brief Gets the x coordinate for the collapsed state.
     *
     * @return The collapsed x coordinate.
     */
    virtual int16_t getCollapsedXCoordinate();

    /**
     * @fn virtual int16_t SlideMenu::getCollapsedYCoordinate();
     *
     * @brief Gets the y coordinate for the collapsed state.
     *
     * @return The collapsed y coordinate.
     */
    virtual int16_t getCollapsedYCoordinate();

    /**
     * @fn virtual int16_t SlideMenu::getExpandedXCoordinate();
     *
     * @brief Gets the x coordinate for the expanded state.
     *
     * @return The expanded x coordinate.
     */
    virtual int16_t getExpandedXCoordinate();

    /**
     * @fn virtual int16_t SlideMenu::getExpandedYCoordinate();
     *
     * @brief Gets the y coordinate for the expanded state.
     *
     * @return The expanded y coordinate.
     */
    virtual int16_t getExpandedYCoordinate();
};
}

#endif // SLIDEMENU_HPP
