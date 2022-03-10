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

#ifndef LISTLAYOUT_HPP
#define LISTLAYOUT_HPP

#include <touchgfx/containers/Container.hpp>
#include <assert.h>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class ListLayout ListLayout.hpp touchgfx/containers/ListLayout.hpp
 *
 * @brief This class provides a layout mechanism for arranging Drawable instances adjacently in the
 *        specified Direction.
 *
 *        This class provides a layout mechanism for arranging Drawable instances adjacently in
 *        the specified Direction.  The first element in the ListLayout is positioned in the
 *        ListLayout origin (0,0). The dimension of this class is automatically expanded to
 *        cover the area of the added Drawable instances.
 *
 * @see Container
 */
class ListLayout : public Container
{
public:

    /**
     * @fn ListLayout::ListLayout(const Direction d = SOUTH)
     *
     * @brief Constructor.
     *
     *        Constructor. Constructs a ListLayout instance that arranges the added elements in
     *        the specified Direction.
     *
     * @param d The direction to grow in when adding children.
     */
    ListLayout(const Direction d = SOUTH)
        : Container(), direction(d), offset(0)
    {
        assert((d == SOUTH || d == EAST) && "Chosen direction not supported");
    }

    /**
     * @fn virtual ListLayout::~ListLayout()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ListLayout() { }

    /**
     * @fn virtual void ListLayout::setDirection(const Direction d);
     *
     * @brief Sets the direction of the ListLayout.
     *
     *        Sets the direction of the ListLayout. If elements have already been added to the
     *        ListLayout, these elements will be repositioned to adhere to the new direction.
     *
     * @param d The new Direction to grow in when added children (either SOUTH or EAST).
     *
     * @see getDirection()
     */
    virtual void setDirection(const Direction d);

    /**
     * @fn virtual Direction ListLayout::getDirection() const
     *
     * @brief Gets the direction of the ListLayout.
     *
     *        Gets the direction of the ListLayout.
     *
     * @return The current direction to grow in when added children (either SOUTH or EAST).
     *
     * @see setDirection()
     */
    virtual Direction getDirection() const
    {
        return direction;
    }

    /**
     * @fn virtual void ListLayout::add(Drawable& d);
     *
     * @brief Adds a Drawable instance to the end of the list.
     *
     *        Adds a Drawable instance to the end of the list. The Drawable dimensions shall be
     *        set prior to addition.
     *
     * @param [in] d The Drawable to add.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void ListLayout::remove(Drawable& d);
     *
     * @brief Removes a Drawable.
     *
     *        Removes a Drawable. Safe to call even if drawable has not been added.
     *
     * @param [in] d The drawable to remove.
     */
    virtual void remove(Drawable& d);

    /**
     * @fn virtual void ListLayout::insert(Drawable* previousElement, Drawable& d);
     *
     * @brief Inserts a Drawable.
     *
     *        Inserts a Drawable.
     *
     * @param [in] previousElement The element to insert the new element after.
     * @param [in] d               The element to insert.
     */
    virtual void insert(Drawable* previousElement, Drawable& d);

    /**
     * @fn virtual void ListLayout::removeAll();
     *
     * @brief Removes all children.
     *
     *        Removes all children by resetting their parent and sibling pointers. In addition, the
     *        geometry is reset and any parent is signaled of the change.
     */
    virtual void removeAll();

    /**
     * @fn virtual uint16_t ListLayout::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_LISTLAYOUT.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_LISTLAYOUT;
    }

private:
    void internalAddElementAt(Drawable& d, int16_t coord);
    void internalAddElement(Drawable& d);
    void internalRemoveElement(Drawable& d, int16_t coord);
    Direction direction;
    int16_t offset;
};
} // namespace touchgfx
#endif // LISTLAYOUT_HPP
