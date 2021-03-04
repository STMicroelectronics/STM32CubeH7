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

#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
class Container;
class SnapshotWidget;

/**
 * @class Transition Transition.hpp touchgfx/transitions/Transition.hpp
 *
 * @brief The Transition class is the base class for Transitions.
 *
 *        The Transition class is the base class for Transitions. Implementations of Transition
 *        defines what happens when transitioning between Screens, which typically involves
 *        visual effects. An example of a transition implementation can be seen in example
 *        custom_transition_example. The most basic transition is the NoTransition class that
 *        does a transition without any visual effects.
 *
 * @see NoTransition
 * @see SlideTransition
 */
class Transition
{
public:

    /**
     * @fn Transition::Transition()
     *
     * @brief Default constructor.
     *
     *        Constructs the Transition.
     */
    Transition() :
        screenContainer(0), done(false)
    {
    }

    /**
     * @fn virtual Transition::~Transition()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Transition()
    {
    }

    /**
     * @fn virtual void Transition::handleTickEvent()
     *
     * @brief Called for every tick when transitioning.
     *
     *        Called for every tick when transitioning. Base does nothing.
     */
    virtual void handleTickEvent() { }

    /**
     * @fn bool Transition::isDone() const
     *
     * @brief Query if the transition is done transitioning.
     *
     *        Query if the transition is done transitioning. It is the responsibility of the
     *        inheriting class to set the underlying done flag.
     *
     * @return True if the transition is done, false otherwise.
     */
    bool isDone() const
    {
        return done;
    }

    /**
     * @fn virtual void Transition::tearDown()
     *
     * @brief Tears down the Animation.
     *
     *        Tears down the Animation. Called before the d.tor. when the application changes
     *        the transition. Base version does nothing.
     */
    virtual void tearDown() { }

    /**
     * @fn virtual void Transition::init()
     *
     * @brief Initializes the transition.
     *
     *        Initializes the transition. Called after the c.tor. when the application changes
     *        the transition. Base version does nothing.
     */
    virtual void init() { }

    /**
     * @fn virtual void Transition::setScreenContainer(Container& cont)
     *
     * @brief Sets the screen container.
     *
     *        Sets the screen container. Is used by Screen to enable the transition to access
     *        the container.
     *
     * @param [in] cont The container the transition should have access to.
     */
    virtual void setScreenContainer(Container& cont)
    {
        screenContainer = &cont;
    }

protected:
    Container* screenContainer;   ///< The screen Container of the Screen transitioning to.
    bool       done;            ///< Flag that indicates when the transition is done. This should be set by implementing classes.
};
} // namespace touchgfx
#endif // TRANSITION_HPP
