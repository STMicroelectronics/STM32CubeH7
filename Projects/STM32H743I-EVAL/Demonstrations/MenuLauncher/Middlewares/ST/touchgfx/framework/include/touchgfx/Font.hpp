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

#ifndef FONT_HPP
#define FONT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>

namespace touchgfx
{
/**
 * @enum GlyphFlags
 *
 * @brief Glyph flag definitions.
 *
 *        Glyph flag definitions.
 */
enum GlyphFlags
{
    GLYPH_DATA_KERNINGTABLEPOS_BIT8_10 = 0x07, ///< The 8th, 9th and 10th bit of the kerningTablePos
    GLYPH_DATA_WIDTH_BIT8 = 0x08, ///< The 9th bit of "width"
    GLYPH_DATA_HEIGHT_BIT8 = 0x10, ///< The 9th bit of "height"
    GLYPH_DATA_TOP_BIT8 = 0x20, ///< The 9th bit of "top"
    GLYPH_DATA_TOP_BIT9 = 0x40, ///< The sign bit of "top"
    GLYPH_DATA_ADVANCE_BIT8 = 0x80  ///< The 9th bit of "advance"
};

#pragma pack(2)

/**
 * @struct GlyphNode Font.hpp touchgfx/Font.hpp
 *
 * @brief struct providing information about a glyph.
 *
 *        struct providing information about a glyph. Used by LCD when rendering.
 */
typedef struct
{
    uint32_t             dataOffset;       ///< The index to the data of this glyph
    Unicode::UnicodeChar unicode;          ///< The unicode of this glyph.
    uint8_t              _width;           ///< Width of the actual glyph data
    uint8_t              _height;          ///< Height of the actual glyph data
    uint8_t              _top;             ///< Vertical offset from baseline of the glyph
    int8_t               left;             ///< Horizontal offset from the left of the glyph
    uint8_t              _advance;         ///< Width of the glyph (including space to the left and right)
    uint8_t              _kerningTablePos; ///< Where are the kerning information for this glyph stored in the kerning table
    uint8_t              kerningTableSize; ///< How many entries are there in the kerning table (following kerningTablePos) for this glyph
    uint8_t              flags;            ///< Additional glyph flags (font encoding and extra precision for width/height/top/advance)

    /**
     * @fn uint16_t kerningTablePos() const
     *
     * @brief Gets the "kerningTablePos" value where the 8th and 9th bits is stored in flags.
     *
     *        Gets the "kerningTablePos" value where the 8th and 9th bits is stored in flags.
     *
     * @return the right value of "kerningTablePos".
     */
    uint16_t kerningTablePos() const
    {
        return (((uint16_t)(flags & GLYPH_DATA_KERNINGTABLEPOS_BIT8_10)) << 8) | (uint16_t)_kerningTablePos;
    }

    /**
     * @fn uint16_t width() const
     *
     * @brief Gets the "width" value where the 9th bit is stored in flags.
     *
     *        Gets the "width" value where the 9th bit is stored in flags.
     *
     * @return the right value of "width".
     */
    uint16_t width() const
    {
        return ((flags & GLYPH_DATA_WIDTH_BIT8) ? 0x0100 : 0) | (uint16_t)_width;
    }

    /**
     * @fn uint16_t height() const
     *
     * @brief Gets the "height" value where the 9th bit is stored in flags.
     *
     *        Gets the "height" value where the 9th bit is stored in flags.
     *
     * @return the right value of "height".
     */
    uint16_t height() const
    {
        return ((flags & GLYPH_DATA_HEIGHT_BIT8) ? 0x0100 : 0) | (uint16_t)_height;
    }

    /**
     * @fn int16_t top() const
     *
     * @brief Gets the "top" value where the 9th bit and the sign bit are stored in flags.
     *
     *        Gets the "top" value where the 9th bit and the sign bit are stored in flags.
     *
     * @return the right value of "top".
     */
    int16_t top() const
    {
        return (int16_t)(((flags & GLYPH_DATA_TOP_BIT9) ? 0xFE00 : 0) | ((flags & GLYPH_DATA_TOP_BIT8) ? 0x100 : 0) | (uint16_t)_top);
    }

    /**
     * @fn void setTop(int16_t newTop)
     *
     * @brief Sets a top
     *
     * @param newTop The new top.
     */
    void setTop(int16_t newTop)
    {
        _top = newTop & 0xFF;
        flags &= ~(GLYPH_DATA_TOP_BIT8 | GLYPH_DATA_TOP_BIT9);
        if (newTop & 0x100)
        {
            flags |= GLYPH_DATA_TOP_BIT8;
        }
        if (newTop & 0x200)
        {
            flags |= GLYPH_DATA_TOP_BIT9;
        }
    }

    /**
     * @fn uint16_t advance() const
     *
     * @brief Gets the "advance" value where the 9th bit is stored in flags.
     *
     *        Gets the "advance" value where the 9th bit is stored in flags.
     *
     * @return the right value of "advance".
     */
    uint16_t advance() const
    {
        return ((flags & GLYPH_DATA_ADVANCE_BIT8) ? 0x0100 : 0) | (uint16_t)_advance;
    }
} GlyphNode;
#pragma pack()

#pragma pack(2)

/**
 * @struct KerningNode Font.hpp touchgfx/Font.hpp
 *
 * @brief Structure providing information about a kerning for a given char pair.
 *
 *        Structure providing information about a kerning for a given char pair. Used by LCD
 *        when rendering.
 */
typedef struct
{
    Unicode::UnicodeChar unicodePrevChar; ///< The unicode for the first character in the kerning pair
    int8_t               distance;        ///< The kerning distance
} KerningNode;
#pragma pack()

/**
 * @typedef uint16_t FontId
 *
 * @brief Defines an alias representing identifier for the font.
 */
typedef uint16_t FontId;

/**
 * @class Font Font.hpp touchgfx/Font.hpp
 *
 * @brief The font base class.
 *
 *        The font base class. This class is abstract and requires the implementation of
 *        getGlyph.
 *
 *        It provides utility functions such as obtaining string width and font height.
 */
class Font
{
public:
    virtual ~Font() { }

    /**
     * @fn virtual const GlyphNode* Font::getGlyph(Unicode::UnicodeChar unicode, const uint8_t*& pixelData, uint8_t& bitsPerPixel) const = 0;
     *
     * @brief Gets the glyph data associated with the specified unicode.
     *
     *        Gets the glyph data associated with the specified unicode. Please note that in case
     *        of Thai letters where diacritics can be placed relative to the previous character(s),
     *        please use TextProvider::getGlyph() instead as it will make a GlyphNode that will be
     *        correct with respect to X/Y position.
     *
     * @param       unicode      The character to look up.
     * @param       pixelData    Pointer to the pixel data for the glyph if the glyph is found. This
     *                           is set by this method.
     * @param [out] bitsPerPixel Reference where to place the number of bits per pixel.
     *
     * @return A pointer to the glyph node or null if the glyph was not found.
     */
    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar unicode, const uint8_t*& pixelData, uint8_t& bitsPerPixel) const = 0;

    /**
     * @fn virtual const GlyphNode* Font::getGlyph(Unicode::UnicodeChar unicode) const
     *
     * @brief Gets the glyph data associated with the specified unicode.
     *
     *        Gets the glyph data associated with the specified unicode. Please note that in case
     *        of Thai letters where diacritics can be placed relative to the previous character(s),
     *        please use TextProvider::getGlyph() instead as it will make a GlyphNode that will be
     *        correct with respect to X/Y position.
     *
     * @param unicode The character to look up.
     *
     * @return A pointer to the glyph node or null if the glyph was not found.
     *
     * @see TextProvider::getGlyph
     */
    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar unicode) const
    {
        const uint8_t* dummyPixelDataPointer = 0;
        uint8_t bitsPerPixelDummy = 0;
        const GlyphNode* glyph = getGlyph(unicode, dummyPixelDataPointer, bitsPerPixelDummy);
        return glyph;
    }

    /**
     * @fn virtual Unicode::UnicodeChar Font::getFallbackChar() const
     *
     * @brief Gets fallback character.
     *
     *        Gets fallback character for the given font. In case there is no glyph for a
     *        character, use the glyph for the character returned by this function. If 0 (zero)
     *        is returned, there is no default character.
     *
     * @return The default character for the typography in case no glyph is available.
     */
    virtual Unicode::UnicodeChar getFallbackChar() const
    {
        return falllbackCharacter;
    }

    /**
    * @fn virtual Unicode::UnicodeChar Font::getEllipsisChar() const
    *
    * @brief Gets ellipsis character.
    *
    *        Gets ellipsis character for the given font. This is the character which is used
    *        when truncating long lines.
    *
    * @return The ellipsis character for the typography.
    */
    virtual Unicode::UnicodeChar getEllipsisChar() const
    {
        return ellipsisCharacter;
    }

    /**
      * @fn virtual uint16_t Font::getStringWidth(const Unicode::UnicodeChar* text, ...) const;
      *
      * @brief Gets the width in pixels of the specified string.
      *
      *        Gets the width in pixels of the specified string. If the string contains multiple
      *        lines, the width of the widest line is found. Please note that the correct number
      *        of arguments must be given if the text contains wildcards.
      *
      *        It is recommended to use the getStringWidth() implementation with the
      *        TextDirection parameter to ensure correct calculation of the width. Kerning could
      *        result in different results depending on the TextDirection. This method assumes
      *        TextDirection to be TEXT_DIRECTION_LTR.
      *
      * @param text A zero-terminated unicode string with arguments to insert if the text
      *             contains wildcards.
      * @param ...  Variable arguments providing additional information.
      *
      * @return The width in pixels of the longest line of the specified string.
      */
    virtual uint16_t getStringWidth(const Unicode::UnicodeChar* text, ...) const;

    /**
     * @fn virtual uint16_t Font::getStringWidth(TextDirection textDirection, const Unicode::UnicodeChar* text, ...) const;
     *
     * @brief Gets the width in pixels of the specified string.
     *
     *        Gets the width in pixels of the specified string. If the string contains multiple
     *        lines, the width of the widest line is found. Please note that the correct number
     *        of arguments must be given if the text contains wildcards.
     *
     *        The TextDirection should be set correctly for the text supplied. For example the
     *        string "10 20 30" will be calculated differently depending on the TextDirection.
     *        If TextDirection is TEXT_DIRECTION_LTR the width is calculated as the with of "10
     *        20 30" (with kerning between all characters) but for TEXT_DIRECTION_RTL it is
     *        calculated as "10"+" "+"20"+" "+"30" (with kerning only between characters in the
     *        substrings and not between substrings). For most fonts there might not be a
     *        difference between the two calculations, but some fonts might cause different
     *        results.
     *
     * @param textDirection The text direction.
     * @param text          A zero-terminated unicode string with arguments to insert if the text
     *                      contains wildcards.
     * @param ...           Variable arguments providing additional information.
     *
     * @return The width in pixels of the longest line of the specified string.
     */
    virtual uint16_t getStringWidth(TextDirection textDirection, const Unicode::UnicodeChar* text, ...) const;

    /**
     * @fn virtual uint16_t Font::getCharWidth(const Unicode::UnicodeChar c) const;
     *
     * @brief Gets the width in pixels of the specified character.
     *
     *        Gets the width in pixels of the specified character.
     *
     * @param c The unicode character.
     *
     * @return The width in pixels of the specified character.
     */
    virtual uint16_t getCharWidth(const Unicode::UnicodeChar c) const;

    /**
     * @fn virtual uint8_t Font::getSpacingAbove(const Unicode::UnicodeChar* text, ...) const;
     *
     * @brief Gets the number of blank pixels at the top of the given text.
     *
     *        Gets the number of blank pixels at the top of the given text.
     *
     * @param text A zero-terminated unicode string.
     * @param ...  Variable arguments providing additional information.
     *
     * @return The number of blank pixels above the text.
     */
    virtual uint8_t getSpacingAbove(const Unicode::UnicodeChar* text, ...) const;

    /**
     * @fn virtual uint16_t Font::getMaxTextHeight(const Unicode::UnicodeChar* text, ...) const;
     *
     * @brief Gets the height of the highest character in a given string.
     *
     *        Gets the height of the highest character in a given string. The height includes
     *        the spacing above the text which is included in the font.
     *
     * @param text A zero-terminated unicode string.
     * @param ...  Variable arguments providing additional information.
     *
     * @return The height if the given text.
     */
    virtual uint16_t getMaxTextHeight(const Unicode::UnicodeChar* text, ...) const;

    /**
     * @fn virtual uint16_t Font::getFontHeight() const
     *
     * @brief Returns the height in pixels of this font.
     *
     *        Returns the height in pixels of this font. The returned value corresponds to the
     *        maximum height occupied by a character in the font.
     *
     * @note It is not sufficient to allocate text areas with this height. Use
     *       getMinimumTextHeight for this.
     *
     * @return The height in pixels of this font.
     */
    virtual uint16_t getFontHeight() const
    {
        return fontHeight;
    }

    /**
     * @fn virtual uint16_t Font::getMinimumTextHeight() const
     *
     * @brief Returns the minimum height needed for a text field that uses this font.
     *
     *        Returns the minimum height needed for a text field that uses this font. Takes
     *        into account that certain characters (eg 'g') have pixels below the baseline,
     *        thus making the text height larger than the font height.
     *
     * @return The minimum height needed for a text field that uses this font.
     */
    virtual uint16_t getMinimumTextHeight() const
    {
        return fontHeight + pixelsBelowBaseline;
    }

    /**
     * @fn virtual uint8_t Font::getBitsPerPixel() const
     *
     * @brief Gets bits per pixel for this font.
     *
     *        Gets bits per pixel for this font.
     *
     * @return The number of bits used per pixel in this font.
     */
    virtual uint8_t getBitsPerPixel() const
    {
        return bPerPixel;
    }

    /**
     * @fn virtual uint8_t Font::getDataFormatA4() const
     *
     * @brief Are the glyphs saved using ST A4 format.
     *
     *        Are the glyphs saved using ST A4 format.
     *
     * @return True if the font is stored using A4 format, false otherwise.
     */
    virtual uint8_t getDataFormatA4() const
    {
        return a4;
    }

    /**
     * @fn uint8_t Font::getMaxPixelsLeft() const
     *
     * @brief Gets maximum pixels left.
     *
     *        Gets maximum pixels left for any glyph in the font. This is the max value of
     *        "left" for all glyphs. The value is negated so if a "g" has left=-6 maxPixelsLeft
     *        is 6. This value is calculated by the font converter.
     *
     * @return The maximum pixels left.
     */
    uint8_t getMaxPixelsLeft() const
    {
        return maxPixelsLeft;
    }

    /**
     * @fn uint8_t Font::getMaxPixelsRight() const
     *
     * @brief Gets maximum pixels right.
     *
     *        Gets maximum pixels right for any glyph in the font. This is the max value of
     *        "width+left-advance" for all glyphs. The is the number of pixels a glyph reaches
     *        to the right of its normal area. This value is calculated by the font converter.
     *
     * @return The maximum pixels right.
     */
    uint8_t getMaxPixelsRight() const
    {
        return maxPixelsRight;
    }

    /**
     * @fn virtual int8_t Font::getKerning(Unicode::UnicodeChar prevChar, const GlyphNode* glyph) const
     *
     * @brief Gets the kerning distance between two characters.
     *
     *        Gets the kerning distance between two characters.
     *
     * @param prevChar The unicode value of the previous character.
     * @param glyph    the glyph object for the current character.
     *
     * @return The kerning distance between prevChar and glyph char.
     */
    virtual int8_t getKerning(Unicode::UnicodeChar prevChar, const GlyphNode* glyph) const
    {
        return 0;
    }

    /**
     * @fn virtual uint16_t Font::getNumberOfLines(const Unicode::UnicodeChar* text, ...) const;
     *
     * @brief Gets number of lines.
     *
     *        Count the number of lines in a given text.
     *
     * @param text The text.
     * @param ...  Variable arguments providing additional information.
     *
     * @return The number of lines.
     */
    virtual uint16_t getNumberOfLines(const Unicode::UnicodeChar* text, ...) const;

    /**
     * @fn virtual uint16_t* getGSUBTable() const;
     *
     * @brief Gets GSUB table.
     *
     *        Gets GSUB table.
     *
     * @return The GSUB table or null if font has GSUB no table
     */
    virtual const uint16_t* getGSUBTable() const
    {
        return 0;
    }

protected:

    /**
     * @fn uint16_t Font::getStringWidthLTR(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;
     *
     * @brief Gets the width in pixels of the specified string.
     *
     *        Gets the width in pixels of the specified string. If the string contains multiple
     *        lines, the width of the widest line is found. Please note that the correct number
     *        of arguments must be given if the text contains wildcards.
     *
     *        The string is assumed to be purely left-to-right.
     *
     * @param textDirection The text direction.
     * @param text          A zero-terminated unicode string with arguments to insert if the text
     *                      contains wildcards.
     * @param pArg          Variable arguments providing additional information.
     *
     * @return The width in pixels of the longest line of the specified string.
     */
    uint16_t getStringWidthLTR(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;

    /**
     * @fn uint16_t Font::getStringWidthRTL(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;
     *
     * @brief Gets the width in pixels of the specified string.
     *
     *        Gets the width in pixels of the specified string. If the string contains multiple
     *        lines, the width of the widest line is found. Please note that the correct number
     *        of arguments must be given if the text contains wildcards.
     *
     *        The string is handled as a right-to-left string and subdivided into smaller text
     *        strings to correctly handle mixing of left-to-right and right-to-left strings.
     *
     * @param textDirection The text direction.
     * @param text          A zero-terminated unicode string with arguments to insert if the text
     *                      contains wildcards.
     * @param pArg          The argument.
     *
     * @return The string width RTL.
     */
    uint16_t getStringWidthRTL(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;

    /**
     * @typedef uint16_t (Font::*StringWidthFunctionPointer)(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const
     *
     * @brief Defines an alias representing the constant.
     */
    typedef uint16_t(Font::*StringWidthFunctionPointer)(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;

    static StringWidthFunctionPointer getStringWidthFunction;   ///< The getStringWidth function, either LTR (supporting LTR only) or RTL (supporting RTL and LTR)

    /**
     * @fn Font::Font(uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t dataFormatA4, uint8_t maxLeft, uint8_t maxRight, const Unicode::UnicodeChar fallbackChar, const Unicode::UnicodeChar ellipsisChar)
     *
     * @brief Constructor.
     *
     *        The protected constructor of a Font.
     *
     * @param height       The font height in pixels.
     * @param pixBelowBase The number of pixels below the base line.
     * @param bitsPerPixel The number of bits per pixel.
     * @param dataFormatA4 The glyphs are saved using ST A4 format.
     * @param maxLeft      The maximum left extend for a glyph in the font.
     * @param maxRight     The maximum right extend for a glyph in the font.
     * @param fallbackChar The fallback character for the typography in case no glyph is
     *                     available.
     * @param ellipsisChar The ellipsis character used for truncating long texts.
     */
    Font(uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t dataFormatA4, uint8_t maxLeft, uint8_t maxRight, const Unicode::UnicodeChar fallbackChar, const Unicode::UnicodeChar ellipsisChar)
        : fontHeight(height),
          pixelsBelowBaseline(pixBelowBase),
          bPerPixel(bitsPerPixel),
          a4(dataFormatA4),
          maxPixelsLeft(maxLeft),
          maxPixelsRight(maxRight),
          falllbackCharacter(fallbackChar),
          ellipsisCharacter(ellipsisChar)
    { }

    uint16_t fontHeight;                     ///< The font height in pixels
    uint8_t  pixelsBelowBaseline;            ///< The number of pixels below the base line
    uint8_t  bPerPixel : 7;                  ///< The number of bits per pixel
    uint8_t  a4 : 1;                         ///< Are glyphs encoded using A4 format
    uint8_t  maxPixelsLeft;                  ///< The maximum number of pixels a glyph extends to the left
    uint8_t  maxPixelsRight;                 ///< The maximum number of pixels a glyph extends to the right
    Unicode::UnicodeChar falllbackCharacter; ///< The fallback character to use when no glyph exists for the wanted character
    Unicode::UnicodeChar ellipsisCharacter;  ///< The ellipsis character used for truncating long texts.

private:
    Font();
};
} // namespace touchgfx

#endif // FONT_HPP
