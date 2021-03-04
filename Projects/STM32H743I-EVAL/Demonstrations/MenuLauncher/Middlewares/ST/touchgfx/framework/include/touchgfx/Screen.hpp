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

#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/JSMOCHelper.hpp>

namespace touchgfx
{
class Drawable;
class Transition;

/**
 * @class Screen Screen.hpp touchgfx/Screen.hpp
 *
 * @brief A Screen represents a full-screen drawable area.  Applications create specific screens by
 *        subclassing this class.
 *
 *        A Screen represents a full-screen drawable area.  Applications create specific
 *        screens by subclassing this class.
 *
 *        Each screen has a root container to which drawables can be added.
 *
 *        This class makes sure to delegate draw requests and various events to the appropriate
 *        drawables in correct order.
 */
class Screen
{
public:

    /**
     * @fn Screen::Screen();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Screen();

    /**
     * @fn virtual Screen::~Screen()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Screen() { }

    /**
     * @fn void Screen::draw();
     *
     * @brief Tells the screen to draw its entire area.
     *
     *        Tells the screen to draw its entire area.
     *
     * @note The more specific draw(Rect&amp;) version is preferred when possible.
     */
    void draw();

    /**
     * @fn void Screen::startSMOC(const Rect& invalidatedArea);
     *
     * @brief Starts a JSMOC run, analyzing what parts of what widgets should be redrawn.
     *
     *        Starts a JSMOC run, analyzing what parts of what widgets should be redrawn.
     *
     * @param [in] invalidatedArea The area to redraw, expressed in absolute coordinates.
     */
    void startSMOC(const Rect& invalidatedArea);

    /**
     * @fn void Screen::JSMOC(const Rect& invalidatedArea, Drawable* widgetToDraw);
     *
     * @brief Recursive JSMOC function. This is the actual occlusion culling implementation.
     *
     *        Recursive JSMOC function. This is the actual occlusion culling implementation.
     *
     * @param [in] invalidatedArea The area to redraw, expressed in absolute coordinates.
     * @param [in] widgetToDraw    Widget currently being drawn.
     */
    void JSMOC(const Rect& invalidatedArea, Drawable* widgetToDraw);

    /**
     * @fn virtual void Screen::draw(Rect& rect);
     *
     * @brief Tell the screen to draw the specified area.
     *
     *        Tell the screen to draw the specified area. Will traverse the drawables tree in z-
     *        order and delegate draw to them.
     *
     * @note The given rect must be in absolute coordinates.
     *
     * @param [in] rect The area in absolute coordinates.
     */
    virtual void draw(Rect& rect);

    /**
     * @fn virtual void Screen::setupScreen()
     *
     * @brief Called by Application::switchScreen() when this screen is going to be displayed.
     *
     *        Called by Application::switchScreen() when this screen is going to be displayed.
     *        Base version does nothing, but place any screen specific initialization code in
     *        an overridden version.
     *
     * @see touchgfx::Application::switchScreen()
     */
    virtual void setupScreen() { }

    /**
     * @fn virtual void Screen::afterTransition()
     *
     * @brief Called by Application::handleTick() when the transition to the screen is done.
     *
     *        Called by Application::handleTick() when the transition to the screen is done.
     *        Base version does nothing, but override to do screen specific initialization code
     *        that has to be done after the transition to the screen.
     *
     * @see touchgfx::Application::handleTick()
     */
    virtual void afterTransition() { }

    /**
     * @fn virtual void Screen::tearDownScreen()
     *
     * @brief Called by Application::switchScreen() when this screen will no longer be displayed.
     *
     *        Called by Application::switchScreen() when this screen will no longer be
     *        displayed. Base version does nothing, but place any screen specific cleanup code
     *        in an overridden version.
     *
     * @see touchgfx::Application::switchScreen()
     */
    virtual void tearDownScreen() { }

    /**
     * @fn virtual void Screen::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Traverse the drawables in reverse z-order and notify them of a click event.
     *
     *        Traverse the drawables in reverse z-order and notify them of a click event.
     *
     * @param evt The event to handle.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void Screen::handleDragEvent(const DragEvent& evt);
     *
     * @brief Traverse the drawables in reverse z-order and notify them of a drag event.
     *
     *        Traverse the drawables in reverse z-order and notify them of a drag event.
     *
     * @param evt The event to handle.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn virtual void Screen::handleGestureEvent(const GestureEvent& evt);
     *
     * @brief Handle gestures. Traverses drawables in reverse-z and notifies them of the gesture.
     *
     *        Handle gestures. Traverses drawables in reverse-z and notifies them of the
     *        gesture.
     *
     * @param evt The event to handle.
     */
    virtual void handleGestureEvent(const GestureEvent& evt);

    /**
     * @fn virtual void Screen::handleTickEvent()
     *
     * @brief Called by the Application on the current screen with a frequency of
     *        Application::TICK_INTERVAL_MS.
     *
     *        Called by the Application on the current screen with a frequency of
     *        Application::TICK_INTERVAL_MS.
     */
    virtual void handleTickEvent()
    {
    }

    /**
     * @fn virtual void Screen::handleKeyEvent(uint8_t key)
     *
     * @brief Called by the Application on the reception of a "key", the meaning of which is
     *        platform/application specific.
     *
     *        Called by the Application on the reception of a "key", the meaning of which is
     *        platform/application specific. Default implementation does nothing.
     *
     * @param key The key to handle.
     */
    virtual void handleKeyEvent(uint8_t key)
    {
    }

    /**
     * @fn bool Screen::usingSMOC() const
     *
     * @brief Determines if using JSMOC.
     *
     * @return true if this screen uses the JSMOC drawing algorithm.
     */
    bool usingSMOC() const
    {
        return useSMOC;
    }

    /**
     * @fn void Screen::bindTransition(Transition& trans);
     *
     * @brief Enables the transition to access the containers.
     *
     *        Enables the transition to access the containers.
     *
     * @param [in] trans The transition to bind.
     */
    void bindTransition(Transition& trans);

    /**
     * @fn Container& Screen::getRootContainer()
     *
     * @brief Obtain a reference to the root container of this screen.
     *
     *        Obtain a reference to the root container of this screen.
     *
     * @return The root container.
     */
    Container& getRootContainer()
    {
        return container;
    }

protected:

    /**
     * @fn void Screen::useSMOCDrawing(bool enabled);
     *
     * @brief Determines whether to use JSMOC or painter's algorithm for drawing.
     *
     *        Determines whether to use JSMOC or painter's algorithm for drawing.
     *
     * @param enabled true if JSMOC should be enabled, false if disabled (meaning painter's
     *                algorithm is employed instead).
     */
    void useSMOCDrawing(bool enabled);

    /**
     * @fn void Screen::add(Drawable& d)
     *
     * @brief Add a drawable to the content container.
     *
     *        Add a drawable to the content container.
     *
     * @note Must not be called with a Drawable that was already added to the screen. If in doubt,
     *       call remove() first.
     *
     * @param [in] d The Drawable to add.
     */
    void add(Drawable& d)
    {
        container.add(d);
    }

    /**
     * @fn void Screen::remove(Drawable& d)
     *
     * @brief Removes a drawable from the content container.
     *
     *        Removes a drawable from the content container. Safe to call even if the drawable
     *        was never added (in which case nothing happens).
     *
     * @param [in] d The Drawable to remove.
     */
    void remove(Drawable& d)
    {
        container.remove(d);
    }

    Container container;    ///< The container contains the contents of the screen.

protected:
    Drawable* focus;      ///< The drawable currently in focus (set when DOWN_PRESSED is received).

private:
    int16_t fingerAdjustmentX;
    int16_t fingerAdjustmentY;
    bool useSMOC;
};
} // namespace touchgfx

#endif // SCREEN_HPP
