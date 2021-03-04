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

#ifndef BUTTONWITHLABEL_HPP
#define BUTTONWITHLABEL_HPP

#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/TypedText.hpp>

namespace touchgfx
{
/**
 * @class ButtonWithLabel ButtonWithLabel.hpp touchgfx/widgets/ButtonWithLabel.hpp
 *
 * @brief A Button specialization that also displays a text on top of the button bitmap.
 *
 *        A Button specialization that also displays a text on top of the button bitmap.
 *
 * @see Button
 */
class ButtonWithLabel : public Button
{
public:

    /**
     * @fn ButtonWithLabel::ButtonWithLabel();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ButtonWithLabel();

    /**
     * @fn void ButtonWithLabel::setLabelText(TypedText t)
     *
     * @brief Sets the text to display on the button.
     *
     *        Sets the text to display on the button. Texts with wildcards are not supported.
     *
     * @param t The text to display.
     */
    void setLabelText(TypedText t)
    {
        typedText = t;
        updateTextPosition();
    }

    /**
     * @fn TypedText ButtonWithLabel::getLabelText() const
     *
     * @brief Gets the text used for the label.
     *
     *        Gets the text used for the label.
     *
     * @return The text used for the label.
     */
    TypedText getLabelText() const
    {
        return typedText;
    }

    /**
     * @fn void ButtonWithLabel::setLabelColor(colortype col, bool performInvalidate = false)
     *
     * @brief Sets label color.
     *
     *        Sets label color.
     *
     * @param col               The color with which the text label should be drawn.
     * @param performInvalidate Optional parameter. If true, performs an instant invalidation of
     *                          the button.
     */
    void setLabelColor(colortype col, bool performInvalidate = false)
    {
        color = col;
        if (performInvalidate)
        {
            invalidate();
        }
    }

    /**
     * @fn void ButtonWithLabel::setLabelColorPressed(colortype col, bool performInvalidate = false)
     *
     * @brief Sets label color when the button is pressed.
     *
     *        Sets label color when the button is pressed.
     *
     * @param col               The color with which the text label should be drawn when the
     *                          button is pressed.
     * @param performInvalidate Optional parameter. If true, performs an instant invalidation of
     *                          the button.
     */
    void setLabelColorPressed(colortype col, bool performInvalidate = false)
    {
        colorPressed = col;
        if (performInvalidate)
        {
            invalidate();
        }
    }

    /**
     * @fn void ButtonWithLabel::setLabelRotation(TextRotation rotation)
     *
     * @brief Sets the rotation of the text on the label.
     *
     *        Sets the rotation of the text on the label. Please note that this will not rotate
     *        the bitmap of the label, only the text.
     *
     * @param rotation the rotation of the text. Default is TEXT_ROTATE_0.
     */
    void setLabelRotation(TextRotation rotation)
    {
        this->rotation = rotation;
    }

    /**
     * @fn TextRotation ButtonWithLabel::getLabelRotation()
     *
     * @brief Gets the current rotation of the text on the label.
     *
     *        Gets the current rotation of the text on the label.
     *
     * @return The current rotation of the text.
     */
    TextRotation getLabelRotation()
    {
        return rotation;
    }

    /**
     * @fn void ButtonWithLabel::updateTextPosition()
     *
     * @brief Positions the label text as horizontally centered.
     *
     *        If the text changes due to a language change you may need to reposition the label
     *        text to stay horizontally centered.
     *
     * @note The method does not invalidate the button. This must be done manually.
     */
    void updateTextPosition()
    {
        if (typedText.hasValidId())
        {
            const Font* f = typedText.getFont();
            const Unicode::UnicodeChar* s = typedText.getText();
            textHeightIncludingSpacing = f->getMaxTextHeight(s) * f->getNumberOfLines(s) + f->getSpacingAbove(s);
        }
        else
        {
            textHeightIncludingSpacing = 0;
        }
    }

    virtual Rect getSolidRect() const
    {
        return Button::getSolidRect();
    };

    virtual void draw(const Rect& area) const;

    /**
     * @fn virtual uint16_t ButtonWithLabel::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_BUTTONWITHLABEL.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BUTTONWITHLABEL;
    }
protected:
    TypedText    typedText;                  ///< The TypedText used for the button label.
    colortype    color;                      ///< The color used for the label when not pressed.
    colortype    colorPressed;               ///< The color used for the label when pressed.
    TextRotation rotation;                   ///< The rotation used for the label.
    uint8_t      textHeightIncludingSpacing; ///< Total height of the label (text height + spacing).
};
} // namespace touchgfx

#endif // BUTTONWITHLABEL_HPP
