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

#ifndef NOTRANSITION_HPP
#define NOTRANSITION_HPP

#include <touchgfx/transitions/Transition.hpp>

namespace touchgfx
{
/**
 * @class NoTransition NoTransition.hpp touchgfx/transitions/NoTransition.hpp
 *
 * @brief The most simple Transition without any visual effects.
 *
 *        The most simple Transition without any visual effects.
 *
 * @see Transition
 */
class NoTransition : public Transition
{
public:

    /**
     * @fn NoTransition::NoTransition()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    NoTransition() : Transition() { }

    /**
     * @fn virtual NoTransition::~NoTransition()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~NoTransition() { }

    /**
     * @fn virtual void NoTransition::handleTickEvent()
     *
     * @brief Indicates that the transition is done after the first tick.
     *
     *        Indicates that the transition is done after the first tick.
     */
    virtual void handleTickEvent()
    {
        done = true;
    }
};
} // namespace touchgfx
#endif // NOTRANSITION_HPP
