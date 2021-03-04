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

#ifndef TEXTPROVIDER_HPP
#define TEXTPROVIDER_HPP

#include <stdarg.h>
#include <touchgfx/Font.hpp>
#include <touchgfx/Unicode.hpp>

namespace touchgfx
{
/**
 * @class TextProvider TextProvider.hpp touchgfx/TextProvider.hpp
 *
 * @brief The TextProvider is used in drawing basic and wildcard strings.
 *
 *        The TextProvider is used in drawing basic and wildcard strings. The TextProvider
 *        enables wildcard expansion of the string at the time it is written to the LCD.
 *
 *        It provides printf formatted text strings one character at the time, without the need
 *        for a user provided buffer to store the text string.
 */
class TextProvider
{
public:
    static const uint32_t MAX_32BIT_INTEGER_DIGITS = 33U; ///< Max number of digits used for the text representation of a 32 bit integer.

    /**
     * @fn TextProvider::TextProvider();
     *
     * @brief Default constructor.
     *
     *        Empty constructor. The user must call initialize() before characters can be
     *        provided.
     */
    TextProvider();

    /**
     * @fn void TextProvider::initialize(const Unicode::UnicodeChar* format, va_list pArg);
     *
     * @brief Initializes the TextProvider.
     *
     *        Initializes the TextProvider. Each '\2' character in the format is replaced by one UnicodeChar* argument from pArg.
     *
     * @param stringFormat The string to format.
     * @param pArg         Format arguments in the form of a va_list.
     * @param gsubTable    Pointer to GSUB table with unicode substitution rules.
     */
    void initialize(const Unicode::UnicodeChar* stringFormat, va_list pArg, const uint16_t* gsubTable = 0);

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextChar(TextDirection direction = TEXT_DIRECTION_LTR);
     *
     * @brief Gets the next character.
     *
     *        Gets the next character. For Arabic and Thai, it is important to use the getNextLigature instead.
     *
     * @return The next character of the expanded string or 0 if end of string is reached.
     *
     * @see TextProvider::getNextLigature()
     */
    Unicode::UnicodeChar getNextChar();

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextLigature(TextDirection direction);
     *
     * @brief Gets the next ligature.
     *
     *        Gets the next ligature. For most languages this is the same as getNextChar() but eg.
     *        Arabic has different ligatures for each character. Thai character placement might
     *        also depend on previous characters. It is recommended to use getNextLigature with
     *        font and glyph parameters to ensure coming glyphs in a text are placed correctly.
     *
     * @note Functions getNextLigature() and getNextChar() will advance through the same buffer and
     *       mixing the use of those functions is not recommended and may cause undesired results.
     *       Instead create two TextProviders and user getNextChar() on one and getNextLigature() on
     *       the other.
     *
     * @param direction The direction.
     *
     * @return The next character of the expanded string or 0 if end of string is reached.
     *
     * @see TextProvider::getNextChar()
     */
    Unicode::UnicodeChar getNextLigature(TextDirection direction);

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextLigature(TextDirection direction, const Font* font, const GlyphNode*& glyph);
     *
     * @brief Gets the next ligature.
     *
     *        Gets the next ligature. For most languages this is the same as getNextChar() but eg.
     *        Arabic has different ligatures for each character.
     *
     *        Also gets a glyph for the ligature in a font. For non-Thai unicodes, this is
     *        identical to using Font::getGlyph(), but for Thai characters where diacritics glyphs
     *        are not always placed at the same relative position, an adjusted GlyphNode will be
     *        generated with correct relative X/Y coordinates.
     *
     * @note Functions getNextLigature() and getNextChar() will advance through the same buffer and
     *       mixing the use of those functions is not recommended and may cause undesired results.
     *       Instead create two TextProviders and user getNextChar() on one and getNextLigature() on
     *       the other.
     *
     * @param       direction The direction.
     * @param       font      The font.
     * @param [out] glyph     The glyph.
     *
     * @return The next character of the expanded string or 0 if end of string i reached.
     *
     * @see TextProvider::getNextChar()
     * @see Font::getGlyph
     */
    Unicode::UnicodeChar getNextLigature(TextDirection direction, const Font* font, const GlyphNode*& glyph);

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextLigature(TextDirection direction, const Font* font, const GlyphNode*& glyph, const uint8_t*& pixelData, uint8_t& bitsPerPixel);
     *
     * @brief Gets the next ligature.
     *
     *        Gets the next ligature. For most languages this is the same as getNextChar() but eg.
     *        Arabic has different ligatures for each character.
     *
     *        Also gets a glyph for the ligature in a font. For non-Thai unicodes, this is
     *        identical to using Font::getGlyph(), but for Thai characters where diacritics glyphs
     *        are not always placed at the same relative position, an adjusted GlyphNode will be
     *        generated with correct relative X/Y coordinates.
     *
     *        Furthermore a pointer to the glyph data and the bit depth of the font are returned in
     *        parameters.
     *
     * @note Functions getNextLigature() and getNextChar() will advance through the same buffer and
     *       mixing the use of those functions is not recommended and may cause undesired results.
     *       Instead create two TextProviders and user getNextChar() on one and getNextLigature() on
     *       the other.
     *
     * @param       direction    The direction.
     * @param       font         The font.
     * @param [out] glyph        The glyph.
     * @param [out] pixelData    Information describing the pixel.
     * @param [out] bitsPerPixel The bits per pixel.
     *
     * @return The next character of the expanded string or 0 if end of string is reached.
     *
     * @see TextProvider::getNextChar()
     * @see Font::getGlyph
     */
    Unicode::UnicodeChar getNextLigature(TextDirection direction, const Font* font, const GlyphNode*& glyph, const uint8_t*& pixelData, uint8_t& bitsPerPixel);

private:
    Unicode::UnicodeChar getNextCharInternal();
    const Unicode::UnicodeChar* original_format_string;
    const Unicode::UnicodeChar* format;
    va_list formatArgs;
    const Unicode::UnicodeChar* substringPointer;
    bool isWritingWildcard;
    template <unsigned int size>
    class circularBuffer
    {
    public:
        circularBuffer()
            : pos(0), used(0)
        {
        }
        void flush()
        {
            used = 0;
        }
        bool getSize() const
        {
            return size;
        }
        bool isEmpty() const
        {
            return used == 0;
        }
        bool isFull() const
        {
            return used == size;
        }
        Unicode::UnicodeChar peekChar(uint16_t offset = 0)
        {
            assert(offset < used);
            return buffer[(pos + offset) % size];
        }
        void dropFront(uint16_t num = 1)
        {
            assert(used >= num);
            pos = (pos + num) % size;
            used -= num;
        }
        Unicode::UnicodeChar popFront()
        {
            assert(used > 0);
            const Unicode::UnicodeChar ch = buffer[pos];
            pos = (pos + 1) % size;
            used--;
            return ch;
        }
        Unicode::UnicodeChar popBack()
        {
            assert(used > 0);
            used--;
            return buffer[(pos + used) % size];
        }
        void allocateFront(uint16_t num)
        {
            assert(used + num <= size);
            pos = ((pos + size) - num) % size;
            used += num;
        }
        void pushFront(Unicode::UnicodeChar newChar)
        {
            allocateFront(1);
            replaceAt(0, newChar);
        }
        void pushBack(Unicode::UnicodeChar newChar)
        {
            assert(used < size);
            buffer[(pos + used) % size] = newChar;
            used++;
        }
        void replaceAt(uint16_t offset, Unicode::UnicodeChar newChar)
        {
            assert(offset < used);
            buffer[(pos + offset) % size] = newChar;
        }

    private:
        Unicode::UnicodeChar buffer[size];
        uint16_t pos;
        uint16_t used;
    };
    static const int NUM_PREV_CHARS = 2;
    static const int NUM_NEXT_CHARS = 10; // input + lookahead + delta(substitution)
    static const int NUM_XTRA_CHARS = 2;
    circularBuffer<NUM_PREV_CHARS> prevCharacters;
    circularBuffer<NUM_NEXT_CHARS> nextCharacters;
    circularBuffer<NUM_XTRA_CHARS> xtraCharacters; // In case we insert
    void replaceInputCharacters(uint16_t existingNumChars, uint16_t newNumChars, const Unicode::UnicodeChar* newChars);
    void fillInputBuffer();
    const uint16_t* fontGsubTable;
    void substituteGlyphs();
    uint16_t gsubTableBinarySearch(const uint16_t numEntries, const uint16_t* unicodeLookupTable, const Unicode::UnicodeChar key) const;
    bool applyGsubRules(const uint16_t* nextTableEntry, const Unicode::UnicodeChar key);
    bool gsubRuleMatch(const uint16_t* tableEntry, uint16_t backtrack, uint16_t input, uint16_t lookahead);

    typedef void (TextProvider::*UnicodeConverterInitFunctionPointer)();
    typedef Unicode::UnicodeChar(TextProvider::*UnicodeConverterFunctionPointer)(const TextDirection direction);
    static UnicodeConverterInitFunctionPointer unicodeConverterInitFunction;
    static UnicodeConverterFunctionPointer unicodeConverterFunction;

    void initializeInternal();
    void unicodeConverterInit();
    Unicode::UnicodeChar unicodeConverter(const TextDirection direction);

    const Unicode::UnicodeChar* binarySearch(uint16_t key, const Unicode::UnicodeChar contextualFormTable[][5], int maxIndex) const;
    const Unicode::UnicodeChar* contextualFormForChar(const Unicode::UnicodeChar currChar) const;

    const GlyphNode* adjustHindiGlyph(const GlyphNode* glyph);
    const GlyphNode* thaiLookupGlyph(const GlyphNode* glyph, const Font* font, Unicode::UnicodeChar unicode) const;
    const GlyphNode* adjustThaiGlyph(const Font* font, const GlyphNode* glyph);
    GlyphNode modifiedGlyph;
    int16_t thaiGlyphTop;
    int16_t thaiGlyphBottom;
    int16_t thaiGlyphLeft;

    bool isContextualBeginning;
    bool lastGlyphIsAccent;
    static const Unicode::UnicodeChar contextualForms4Long[][5];
    static const Unicode::UnicodeChar contextualForms3Long[][5];
    static const Unicode::UnicodeChar contextualForms2Long[][5];
    static const Unicode::UnicodeChar contextualForms0621_063a[][4];
    static const Unicode::UnicodeChar contextualForms0641_064a[][4];
    static const Unicode::UnicodeChar contextualForms06XX[][5];
};
} // namespace touchgfx

#endif // TEXTPROVIDER_HPP
