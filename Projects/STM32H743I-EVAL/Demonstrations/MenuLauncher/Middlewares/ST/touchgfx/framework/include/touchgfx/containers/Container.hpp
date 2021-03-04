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

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class Container Container.hpp touchgfx/containers/Container.hpp
 *
 * @brief A Container is a Drawable that can have child nodes.
 *
 *        A Container is a Drawable that can have child nodes. The z-order of children is
 *        determined by the order in which Drawables are added to the container - the Drawable
 *        added last will be front-most on the screen.
 *
 *        This class overrides a few functions in Drawable in order to traverse child nodes.
 *
 *        Note that containers act as view ports - that is, only the parts of children that
 *        intersect with the geometry of the container will be visible (e.g. setting a
 *        container's width to 0 will render all children invisible).
 *
 * @see Drawable
 */
class Container : public Drawable
{
public:

    /**
     * @fn Container::Container()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Container() : Drawable(),
        firstChild(0)
    {
    }

    /**
     * @fn virtual Container::~Container()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Container() { }

    /**
     * @fn virtual void Container::add(Drawable& d);
     *
     * @brief Adds a Drawable instance as child to this Container.
     *
     *        Adds a Drawable instance as child to this Container.
     *
     * @note Never add a drawable more than once (will loop forever)!
     *
     * @param [in] d The Drawable to add.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void Container::remove(Drawable& d);
     *
     * @brief Removes a Drawable instance from the list of children.
     *
     *        Removes a Drawable instance from the list of children.
     *
     * @note This is safe to call even if d is not a child (in which case nothing happens).
     *
     * @param [in] d The Drawable to remove.
     */
    virtual void remove(Drawable& d);

    /**
     * @fn virtual void Container::removeAll();
     *
     * @brief Removes all children by resetting their parent and sibling pointers.
     *
     *        Removes all children by resetting their parent and sibling pointers.
     */
    virtual void removeAll();

    /**
     * @fn virtual void Container::unlink();
     *
     * @brief Removes all children by unlinking the first child.
     *
     *        Removes all children by unlinking the first child. The parent and sibling
     *        pointers of the children are not reset.
     */
    virtual void unlink();

    /**
     * @fn virtual bool Container::contains(const Drawable& d);
     *
     * @brief Query if this object contains the given drawable.
     *
     *        Query if this object contains the given drawable.
     *
     * @param d The Drawable to look for.
     *
     * @return True if the specified Drawable instance is direct child of this container, false
     *         otherwise.
     */
    virtual bool contains(const Drawable& d);

    /**
     * @fn virtual void Container::insert(Drawable* previous, Drawable& d);
     *
     * @brief Inserts a Drawable instance after the specified child node.
     *
     *        Inserts a Drawable instance after the specified child node. If previous is null, the
     *        drawable will be inserted as the first element in the list.
     *
     * @note As with add, do not add the same drawable twice.
     *
     * @param [in] previous The Drawable to insert after. If null, insert as header.
     * @param [in] d        The Drawable to insert.
     */
    virtual void insert(Drawable* previous, Drawable& d);

    /**
     * @fn virtual void Container::getLastChild(int16_t x, int16_t y, Drawable** last);
     *
     * @brief Gets the last child of this container.
     *
     *        Gets the last (=highest Z-order) child of this container that is enabled, visible
     *        and intersects with the specified point. Recursive function.
     *
     * @param x          The x coordinate of the intersection.
     * @param y          The y coordinate of the intersection.
     * @param [out] last out parameter in which the result is placed.
     *
     * @see Drawable::getLastChild
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last);

    /**
     * @fn virtual void Container::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the children of this container.
     *
     *        Draws the children of this container. Only used when JSMOC is disabled.
     *
     * @param invalidatedArea The area to draw.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect Container::getSolidRect() const;
     *
     * @brief Gets solid rectangle of a Container.
     *
     *        Gets solid rectangle of a Container. JSMOC does not operate directly on
     *        containers.
     *
     * @return An empty rectangle per default.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual void Container::forEachChild(GenericCallback<Drawable&>* function);
     *
     * @brief Calls the specified function for each child in the container.
     *
     *        Calls the specified function for each child in the container. Function to call
     *        must have the following prototype: void T::func(Drawable&amp;)
     *
     * @param [in] function The function to be executed for each child.
     *
     * @see ListLayout::insert for a usage example.
     */
    virtual void forEachChild(GenericCallback<Drawable&>* function);

    /**
     * @fn virtual uint16_t Container::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_CONTAINER.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_CONTAINER;
    }

    /**
     * @fn Drawable* Container::getFirstChild()
     *
     * @brief Obtain a pointer to the first child of this container.
     *
     *        Useful if you want to manually iterate the children added to this container.
     *
     * @return Pointer to the first drawable added to this container. If nothing has been added
     *         return zero.
     */
    Drawable* getFirstChild()
    {
        return firstChild;
    }
protected:

    /**
     * @fn virtual Rect Container::getContainedArea() const;
     *
     * @brief Gets a rectangle describing the total area covered by the children of this container.
     *
     *        Gets a rectangle describing the total area covered by the children of this
     *        container.
     *
     * @return Rectangle covering all children.
     */
    virtual Rect getContainedArea() const;

    /**
     * @fn virtual void Container::moveChildrenRelative(int16_t deltaX, int16_t deltaY);
     *
     * @brief Calls moveRelative on all children.
     *
     *        Calls moveRelative on all children.
     *
     * @param deltaX Horizontal displacement.
     * @param deltaY Vertical displacement.
     */
    virtual void moveChildrenRelative(int16_t deltaX, int16_t deltaY);

    Drawable* firstChild; ///< Pointer to the first child of this container. Subsequent children can be found through firstChild->nextSibling.

    friend class Screen;
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);
};
} // namespace touchgfx
#endif // CONTAINER_HPP
