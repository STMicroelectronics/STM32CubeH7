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

#ifndef ICONBUTTONSTYLE_HPP
#define ICONBUTTONSTYLE_HPP

#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * @class IconButtonStyle IconButtonStyle.hpp touchgfx/containers/buttons/IconButtonStyle.hpp
 *
 * @brief An icon button style.
 *
 *        An icon button style. This class is supposed to be used
 *        with one of the ButtonTrigger classes to create a functional
 *        button. This class will show one of two icons depending on
 *        the state of the button (pressed or released).
 *
 *        To get a background behind the icon, use IconButtonStyle
 *        together with e.g. ImageButtonStyle:
 *        IconButtonStyle<ImageButtonStyle<ClickButtonTrigger> >
 *        myButton;
 *
 *        The IconButtonStyle will center the icon on the enclosing
 *        container (normally AbstractButtonContainer). Set the size
 *        of the button before setting the icons.
 *
 *        The position of the icon can be adjusted with setIconXY.
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
template<class T>
class IconButtonStyle : public T
{
public:
    /**
     * @fn IconButtonStyle::IconButtonStyle()
     *
     * @brief Default constructor.
     */
    IconButtonStyle() : T()
    {
        T::add(iconImage);
    }

    /**
     * @fn virtual IconButtonStyle::~IconButtonStyle()
     *
     * @brief Destructor.
     */
    virtual ~IconButtonStyle() { }

    /**
     * @fn virtual void IconButtonStyle::setIconBitmaps(const Bitmap& newIconReleased, const Bitmap& newIconPressed)
     *
     * @brief Sets icon bitmaps.
     *
     * @param newIconReleased The new icon released.
     * @param newIconPressed  The new icon pressed.
     */
    virtual void setIconBitmaps(const Bitmap& newIconReleased, const Bitmap& newIconPressed)
    {
        iconReleased = newIconReleased;
        iconPressed = newIconPressed;

        iconImage.setXY((T::getWidth() / 2) - (newIconPressed.getWidth() / 2), (T::getHeight() / 2) - (newIconPressed.getHeight() / 2));

        handlePressedUpdated();
    }

    /**
     * @fn void IconButtonStyle::setIconX(int16_t x)
     *
     * @brief Sets icon x coordinate.
     *
     * @param x The x coordinate.
     */
    void setIconX(int16_t x)
    {
        iconImage.setX(x);
    }

    /**
     * @fn void IconButtonStyle::setIconY(int16_t y)
     *
     * @brief Sets icon y coordinate.
     *
     * @param y The y coordinate.
     */
    void setIconY(int16_t y)
    {
        iconImage.setY(y);
    }

    /**
     * @fn void IconButtonStyle::setIconXY(int16_t x, int16_t y)
     *
     * @brief Sets icon xy.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setIconXY(int16_t x, int16_t y)
    {
        setIconX(x);
        setIconY(y);
    }

    /**
     * @fn Bitmap IconButtonStyle::getCurrentlyDisplayedIcon() const
     *
     * @brief Gets currently displayed icon.
     *
     * @return The currently displayed icon.
     */
    Bitmap getCurrentlyDisplayedIcon() const
    {
        return (T::getPressed() ? iconPressed : iconReleased);
    }

    /**
     * @fn int16_t IconButtonStyle::getIconX() const
     *
     * @brief Gets icon x coordinate.
     *
     * @return The icon x coordinate.
     */
    int16_t getIconX() const
    {
        return iconImage.getX();
    }

    /**
     * @fn int16_t IconButtonStyle::getIconY() const
     *
     * @brief Gets icon y coordinate.
     *
     * @return The icon y coordinate.
     */
    int16_t getIconY() const
    {
        return iconImage.getY();
    }

protected:
    Bitmap  iconReleased; ///< Icon to display when button is not pressed.
    Bitmap  iconPressed;  ///< Icon to display when button is pressed.
    Image iconImage;      ///< The icon image

    /**
     * @fn virtual void IconButtonStyle::handlePressedUpdated()
     *
     * @brief Handles the pressed updated.
     */
    virtual void handlePressedUpdated()
    {
        iconImage.setBitmap(T::getPressed() ? iconPressed : iconReleased);
        T::handlePressedUpdated();
    }

    /**
     * @fn virtual void IconButtonStyle::handleAlphaUpdated()
     *
     * @brief Handles the alpha updated.
     */
    virtual void handleAlphaUpdated()
    {
        iconImage.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};
} // namespace touchgfx

#endif // ICONBUTTONSTYLE_HPP
