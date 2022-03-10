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

#ifndef DRAGGABLE_HPP
#define DRAGGABLE_HPP

#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class Draggable Draggable.hpp touchgfx/mixins/Draggable.hpp
 *
 * @brief Mix-in class that extends a class to become draggable.
 *
 *        Mix-in class that extends a class to become draggable.
 *
 * @tparam T specifies the type to extend with the draggable behavior.
 */
template<class T>
class Draggable : public T
{
public:

    /**
     * @fn Draggable::Draggable()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Draggable() : T()
    {
        T::setTouchable(true);
    }

    /**
     * @fn virtual Draggable::~Draggable()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Draggable() { }

    /**
     * @fn virtual void Draggable::handleDragEvent(const DragEvent& evt)
     *
     * @brief Called when dragging the draggable object.
     *
     *        Called when dragging the draggable object. The object is moved according to the
     *        drag event.
     *
     * @param evt The drag event.
     */
    virtual void handleDragEvent(const DragEvent& evt)
    {
        T::handleDragEvent(evt);
        T::moveRelative(evt.getDeltaX(), evt.getDeltaY());
    }
};
} //namespace touchgfx
#endif // DRAGGABLE_HPP
