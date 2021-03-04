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

#ifndef TYPEDTEXT_HPP
#define TYPEDTEXT_HPP

#include <cassert>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Texts.hpp>

namespace touchgfx
{
/**
 * This type shall be used by the application to define unique IDs for all
 * typed texts in the system. The application shall define typed text IDs in
 * the range [0,number of typed texts - 1].
 */

const TypedTextId TYPED_TEXT_INVALID = 0xFFFFU; ///< The ID of an invalid text

#ifdef __ARMCC_VERSION
// Keil compiler issues irrelevant warning relating to missing ctor initialization for TypedTextData.
#pragma diag_suppress 368
#endif

/**
 * @class TypedText TypedText.hpp touchgfx/TypedText.hpp
 *
 * @brief TypedText represents text (as in characters) and typography (as in font and alignment).
 *
 *        TypedText represents text (as in characters) and typography (as in font and
 *        alignment). TypedText provides methods for interacting with the text, font and
 *        alignment.
 *
 *        Example text_example shows how to use TypedText.
 *
 * @see TextArea
 */
class TypedText
{
public:

    /**
     * @struct TypedTextData TypedText.hpp touchgfx/TypedText.hpp
     *
     * @brief The data structure for typed texts.
     *
     The data structure for typed texts.
     */
    struct TypedTextData
    {
        const unsigned char fontIdx;       ///< The font associated with the typed text
        const Alignment     alignment : 2; ///< The alignment of the typed text
        const TextDirection direction : 2; ///< The text direction (LTR,RTL,...) of the typed text
    };

    /**
     * @fn explicit TypedText::TypedText(const TypedTextId id = TYPED_TEXT_INVALID)
     *
     * @brief Construct a typed text.
     *
     *        Construct a typed text.
     *
     * @param id The id of the TypedText.
     */
    explicit TypedText(const TypedTextId id = TYPED_TEXT_INVALID) : typedTextId(id)
    {
    }

    /**
     * @fn TypedTextId TypedText::getId() const
     *
     * @brief Gets the id of the typed text.
     *
     *        Gets the id of the typed text.
     *
     * @return The id.
     */
    TypedTextId getId() const
    {
        return typedTextId;
    }

    /**
     * @fn bool TypedText::hasValidId() const
     *
     * @brief Has the TypedText been set to a proper value.
     *
     *        Has the TypedText been set to a proper value.
     *
     * @return Is the id valid.
     */
    bool hasValidId() const
    {
        return typedTextId != TYPED_TEXT_INVALID;
    }

    /**
     * @fn const Unicode::UnicodeChar* TypedText::getText() const
     *
     * @brief Gets the text associated with this TypedText.
     *
     *        Gets the text associated with this TypedText.
     *
     * @return The text.
     */
    const Unicode::UnicodeChar* getText() const
    {
        assertValid();
        return texts->getText(typedTextId);
    }

    /**
     * @fn const Font* TypedText::getFont() const
     *
     * @brief Gets the font associated with this TypedText.
     *
     *        Gets the font associated with this TypedText.
     *
     * @return The font.
     */
    const Font* getFont() const
    {
        assertValid();
        return fonts[typedTexts[typedTextId].fontIdx];
    }

    /**
     * @fn FontId TypedText::getFontId() const
     *
     * @brief Gets the font ID associated with this TypedText.
     *
     *        Gets the font ID associated with this TypedText.
     *
     * @return The font.
     */
    FontId getFontId() const
    {
        assertValid();
        return typedTexts[typedTextId].fontIdx;
    }

    /**
     * @fn Alignment TypedText::getAlignment() const
     *
     * @brief Gets the alignment associated with this TypedText.
     *
     *        Gets the alignment associated with this TypedText.
     *
     * @return The alignment.
     */
    Alignment getAlignment() const
    {
        assertValid();
        return typedTexts[typedTextId].alignment;
    }

    /**
     * @fn TextDirection TypedText::getTextDirection() const
     *
     * @brief Gets the text direction associated with this TypedText.
     *
     *        Gets the text direction associated with this TypedText.
     *
     * @return The alignment.
     */
    TextDirection getTextDirection() const
    {
        assertValid();
        return typedTexts[typedTextId].direction;
    }

    /**
     * @fn static void TypedText::registerTypedTextDatabase(const TypedTextData* data, const Font* const* f, const uint16_t n)
     *
     * @brief Registers an array of typed texts.
     *
     *        Registers an array of typed texts. All typed text instances are bound to this
     *        database.
     *
     * @param data A reference to the TypedTextData storage array.
     * @param f    The fonts associated with the array.
     * @param n    The number of typed texts in the array.
     */
    static void registerTypedTextDatabase(const TypedTextData* data, const Font* const* f, const uint16_t n)
    {
        typedTexts = data;
        fonts = f;
        numberOfTypedTexts = n;
    }

    /**
     * @fn static void TypedText::registerTexts(const Texts* t)
     *
     * @brief Registers an array of texts.
     *
     *        Registers an array of texts.
     *
     * @param t The array of texts.
     */
    static void registerTexts(const Texts* t)
    {
        texts = t;
    }

private:
    void assertValid() const
    {
        assert(typedTexts != 0 && "TypedText database has not been initialized.");
        assert(typedTextId < numberOfTypedTexts && "typedTextId larger than numberOfTypedTexts.");
    }

    TypedTextId typedTextId;

    static const TypedTextData* typedTexts;
    static const Texts* texts;
    static const Font* const* fonts;
    static uint16_t numberOfTypedTexts;
};
} // namespace touchgfx

#endif // TYPEDTEXT_HPP
