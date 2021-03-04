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

#ifndef SNAPSHOTWIDGET_HPP
#define SNAPSHOTWIDGET_HPP

#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * @class SnapshotWidget SnapshotWidget.hpp touchgfx/widgets/SnapshotWidget.hpp
 *
 * @brief A widget that is able to make a snapshot of the area the SnapshotWidget covers.
 *
 *        A widget that is able to make a snapshot of the area the SnapshotWidget covers. The
 *        SnapshotWidget will show the snapshot captured when it is drawn.
 *        Note: The snapshot must be taken from a byte aligned position.
 *        On BPP=4, this means on even positions, x=0, 2, 4, 8,...
 *        On BPP=2, this means on positions, x= 0, 4, 8, 12,...
 *        On BPP=1, this means on positions, x= 0, 8, 16,...
 *
 * @see Widget
 */
class SnapshotWidget : public Widget
{
public:

    /**
     * @fn SnapshotWidget::SnapshotWidget();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    SnapshotWidget();

    /**
     * @fn virtual SnapshotWidget::~SnapshotWidget();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~SnapshotWidget();

    /**
     * @fn virtual void SnapshotWidget::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the SnapshotWidget.
     *
     *        Draws the SnapshotWidget. It supports partial drawing, so it only redraws the
     *        area described by invalidatedArea.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect SnapshotWidget::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return The solid rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual void SnapshotWidget::makeSnapshot();
     *
     * @brief Makes a snapshot of the area the SnapshotWidget currently covers.
     *
     *        Makes a snapshot of the area the SnapshotWidget currently covers. This area is
     *        defined by setting the dimensions and the position of the SnapshotWidget.
     *        The snapshot is stored in Animation Storage.
     */
    virtual void makeSnapshot();

    /**
     * @fn virtual void SnapshotWidget::makeSnapshot(const BitmapId bmp);
     *
     * @brief Makes a snapshot of the area the SnapshotWidget currently to a bitmap.
     *
     *        Makes a snapshot of the area the SnapshotWidget
     *        currently covers. This area is defined by setting the
     *        dimensions and the position of the SnapshotWidget. The
     *        snapshot is stored in the provided dynamic bitmap.
     *
     * @param bmp The target dynamic bitmap.
     */
    virtual void makeSnapshot(const BitmapId bmp);

    /**
     * @fn void SnapshotWidget::setAlpha(const uint8_t a)
     *
     * @brief Sets the alpha value.
     *
     *        Sets the alpha value.
     *
     * @param a The alpha value.
     */
    void setAlpha(const uint8_t a)
    {
        alpha = a;
    }

    /**
     * @fn uint8_t SnapshotWidget::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual uint16_t SnapshotWidget::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_SNAPSHOTWIDGET.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_SNAPSHOTWIDGET;
    }

protected:
    BitmapId bitmapId; ///< BitmapId where copy is stored s copied to.
    uint8_t  alpha;    ///< The alpha with which to draw this snapshot.
};
} // namespace touchgfx

#endif // SNAPSHOTWIDGET_HPP
