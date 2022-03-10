/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
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
