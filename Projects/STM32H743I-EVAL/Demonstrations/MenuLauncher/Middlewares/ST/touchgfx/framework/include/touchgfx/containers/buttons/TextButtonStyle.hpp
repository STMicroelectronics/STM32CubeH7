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

#ifndef TEXTBUTTONSTYLE_HPP
#define TEXTBUTTONSTYLE_HPP

#include <touchgfx/widgets/TextArea.hpp>

namespace touchgfx
{
/**
 * @class TextButtonStyle TextButtonStyle.hpp touchgfx/containers/buttons/TextButtonStyle.hpp
 *
 * @brief A text button style.
 *
 *        An text button style. This class is supposed to be used with
 *        one of the ButtonTrigger classes to create a functional
 *        button. This class will show a text in one of two colors
 *        depending on the state of the button (pressed or released).
 *
 *        The TextButtonStyle does not set the size of the enclosing
 *        container (normally AbstractButtonContainer). The size must
 *        be set manually.
 *
 *        To get a background behind the text, use TextButtonStyle
 *        together with e.g. ImageButtonStyle:
 *        TextButtonStyle<ImageButtonStyle<ClickButtonTrigger> >
 *        myButton;
 *
 *        The position of the text can be adjusted with setTextXY
 *        (default is centered).
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
template<class T>
class TextButtonStyle : public T
{
public:
    /**
     * @fn TextButtonStyle::TextButtonStyle()
     *
     * @brief Default constructor.
     */
    TextButtonStyle() : T()
    {
        T::add(text);
    }

    /**
     * @fn virtual TextButtonStyle::~TextButtonStyle()
     *
     * @brief Destructor.
     */
    virtual ~TextButtonStyle() { }

    /**
     * @fn void TextButtonStyle::setText(TypedText t)
     *
     * @brief Sets a text.
     *
     * @param t A TypedText to process.
     */
    void setText(TypedText t)
    {
        text.setTypedText(t);
        text.setWidth(T::getWidth());
        text.setHeight(T::getHeight());
    }

    /**
     * @fn void TextButtonStyle::setTextX(int16_t x)
     *
     * @brief Sets text x coordinate.
     *
     * @param x The x coordinate.
     */
    void setTextX(int16_t x)
    {
        text.setX(x);
    }

    /**
     * @fn void TextButtonStyle::setTextY(int16_t y)
     *
     * @brief Sets text y coordinate.
     *
     * @param y The y coordinate.
     */
    void setTextY(int16_t y)
    {
        text.setY(y);
    }

    /**
     * @fn void TextButtonStyle::setTextXY(int16_t x, int16_t y)
     *
     * @brief Sets text xy.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setTextXY(int16_t x, int16_t y)
    {
        setTextX(x);
        setTextY(y);
    }

    /**
     * @fn void TextButtonStyle::setTextPosition(int16_t x, int16_t y, , int16_t width, int16_t height)
     *
     * @brief Sets text position.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param width The width of the text.
     * @param height The height of the text.
     */
    void setTextPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        text.setPosition(x, y, width, height);
    }

    /**
     * @fn void TextButtonStyle::setTextRotation(TextRotation rotation)
     *
     * @brief Sets text rotation.
     *
     * @param rotation The rotation.
     */
    void setTextRotation(TextRotation rotation)
    {
        text.setRotation(rotation);
    }

    /**
     * @fn void TextButtonStyle::setTextColors(colortype newColorReleased, colortype newColorPressed)
     *
     * @brief Sets text colors.
     *
     * @param newColorReleased The new color released.
     * @param newColorPressed  The new color pressed.
     */
    void setTextColors(colortype newColorReleased, colortype newColorPressed)
    {
        colorReleased = newColorReleased;
        colorPressed = newColorPressed;

        handlePressedUpdated();
    }

protected:
    TextArea text;           ///< The text
    colortype colorReleased; ///< The color released
    colortype colorPressed;  ///< The color pressed

    /**
     * @fn virtual void TextButtonStyle::handlePressedUpdated()
     *
     * @brief Handles the pressed updated.
     */
    virtual void handlePressedUpdated()
    {
        text.setColor(T::getPressed() ? colorPressed : colorReleased);
        T::handlePressedUpdated();
    }

    /**
     * @fn virtual void TextButtonStyle::handleAlphaUpdated()
     *
     * @brief Handles the alpha updated.
     */
    virtual void handleAlphaUpdated()
    {
        text.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};
} // namespace touchgfx

#endif // TEXTBUTTONSTYLE_HPP
