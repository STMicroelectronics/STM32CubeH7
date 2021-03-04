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

#ifndef PRESENTER_HPP
#define PRESENTER_HPP

namespace touchgfx
{
/**
 * @class Presenter Presenter.hpp mvp/Presenter.hpp
 *
 * @brief The Presenter base class that all application-specific presenters should derive from.
 *
 *        The Presenter base class that all application-specific presenters should derive from.
 *        Only contains activate and deactivate virtual functions which are called
 *        automatically during screen transition.
 */
class Presenter
{
public:

    /**
     * @fn virtual void Presenter::activate()
     *
     * @brief Place initialization code for the Presenter here.
     *
     *        The activate function is called automatically when a screen transition causes
     *        this Presenter to become active. Place initialization code for the Presenter here.
     */
    virtual void activate()
    {
    }

    /**
     * @fn virtual void Presenter::deactivate()
     *
     * @brief Place cleanup code for the Presenter here.
     *
     *        The deactivate function is called automatically when a screen transition causes
     *        this Presenter to become inactive. Place cleanup code for the Presenter here.
     */
    virtual void deactivate()
    {
    }

    /**
     * @fn virtual Presenter::~Presenter()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Presenter()
    {
    }

protected:

    /**
     * @fn Presenter::Presenter()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Presenter()
    {
    }
};
} // namespace touchgfx
#endif // PRESENTER_HPP
