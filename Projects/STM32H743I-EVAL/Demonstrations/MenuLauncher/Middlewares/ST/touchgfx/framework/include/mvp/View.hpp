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

#ifndef VIEW_HPP
#define VIEW_HPP

#include <touchgfx/Screen.hpp>
#include <mvp/Presenter.hpp>

namespace touchgfx
{
/**
 * @class View View.hpp mvp/View.hpp
 *
 * @brief This is a generic touchgfx::Screen specialization for normal applications.
 *
 *        This is a generic touchgfx::Screen specialization for normal applications. It
 *        provides a link to the Presenter class.
 *
 * @note All views in the application must be a subclass of this type.
 *
 * @tparam T The type of Presenter associated with this view.
 *
 * @see Screen
 */
template<class T>
class View : public Screen
{
public:

    /**
     * @fn View::View()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    View() : presenter(0)
    {
    }

    /**
     * @fn void View::bind(T& presenter)
     *
     * @brief Binds an instance of a specific Presenter type (subclass) to the View instance.
     *
     *        Binds an instance of a specific Presenter type (subclass) to the View instance.
     *        This function is called automatically when a new presenter/view pair is activated.
     *
     * @param [in] presenter The specific Presenter to be associated with the View.
     */
    void bind(T& presenter)
    {
        this->presenter = &presenter;
    }

protected:
    T* presenter;     ///< Pointer to the Presenter associated with this view.
};
} // namespace touchgfx
#endif // VIEW_HPP
