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

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/FontManager.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{
/**
 * @class Keyboard Keyboard.hpp touchgfx/widgets/Keyboard.hpp
 *
 * @brief The keyboard provides text input for touch devices.
 *
 *        The keyboard provides text input for touch devices. It is configured using a Layout
 *        and a KeyMappingList, which both can be changed at runtime. The class using the
 *        keyboard must provide a buffer where the entered text is placed. The Layout contains
 *        a bitmap id for the image to display and two mappings: rectangles to key ids and
 *        rectangles to callback methods.
 *
 *        The KeyMappingList maps key ids to unicode characters. When the user presses a key,
 *        the keyboard looks in its layout for a rectangle containing the coordinates pressed.
 *        If it finds a mapping to a callback method, it will invoke that method. If it finds a
 *        mapping to a key it will look up the unicode character for that key and place it in a
 *        text buffer. The sequence is: (x,y) -> KeyId -> UnicodeChar.
 *
 *        A keyboard with multiple key mappings e.g. lower case alpha, upper case alpha and
 *        numeric mappings can be created by implementing callback methods for shift and mode
 *        areas in the provided bitmap and then changing the KeyMappingList when those areas
 *        are pressed.
 *
 * @see Container
 */
class Keyboard : public Container
{
public:

    /**
     * @struct Key Keyboard.hpp touchgfx/widgets/Keyboard.hpp
     *
     * @brief Mapping from rectangle to key id.
     */
    typedef struct
    {
        uint8_t  keyId;             ///< The id of a key
        Rect     keyArea;           ///< The area occupied by the key
        BitmapId highlightBitmapId; ///< A bitmap to show when the area is "pressed"
    } Key;

    /**
     * @struct CallbackArea Keyboard.hpp touchgfx/widgets/Keyboard.hpp
     *
     * @brief Mapping from rectangle to a callback method to execute.
     */
    typedef struct
    {
        Rect               keyArea;           ///< The area occupied by a key
        GenericCallback<>* callback;          ///< The callback to execute, when the area is "pressed". The callback should be a Callback<YourClass> member in the class using the keyboard
        BitmapId           highlightBitmapId; ///< A bitmap to show when the area is "pressed"
    } CallbackArea;

    /**
     * @struct Layout Keyboard.hpp touchgfx/widgets/Keyboard.hpp
     *
     * @brief Definition of the keyboard layout. The keyboard can handle changing layouts, so
     *        different keyboard modes can be implemented by changing layouts and key mappings.
     */
    typedef struct
    {
        BitmapId      bitmap;                ///< The bitmap used for the keyboard layout
        const Key*    keyArray;              ///< The keys on the keyboard layout
        uint8_t       numberOfKeys;          ///< The number of keys on this keyboard layout
        CallbackArea* callbackAreaArray;     ///< The array of areas and corresponding callbacks
        uint8_t       numberOfCallbackAreas; ///< The number of areas and corresponding callbacks
        Rect          textAreaPosition;      ///< The area where text is written
        TypedText     textAreaFont;          ///< The font used for typing text
        colortype     textAreaFontColor;     ///< The color used for the typing text
        FontId        keyFont;               ///< The font used for the keys
        colortype     keyFontColor;          ///< The color used for the keys
    } Layout;

    /**
     * @struct KeyMapping Keyboard.hpp touchgfx/widgets/Keyboard.hpp
     *
     * @brief Mapping from key id to Unicode character.
     */
    typedef struct
    {
        uint8_t              keyId;    ///< Id of a key
        Unicode::UnicodeChar keyValue; ///< Unicode equivalent of the key id
    } KeyMapping;

    /**
     * @struct KeyMappingList Keyboard.hpp touchgfx/widgets/Keyboard.hpp
     *
     * @brief List of KeyMappings to use.
     */
    typedef struct
    {
        const KeyMapping* keyMappingArray; ///< The array of key mappings used by the keyboard
        uint8_t           numberOfKeys;    ///< The number of keys in the list
    } KeyMappingList;

    /**
     * @fn Keyboard::Keyboard();
     *
     * @brief Default Constructor.
     *
     *        Creates a new Keyboard.
     */
    Keyboard();

    /**
     * @fn virtual Keyboard::~Keyboard()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Keyboard() { }

    /**
     * @fn void Keyboard::setBuffer(Unicode::UnicodeChar* newBuffer, uint16_t newBufferSize);
     *
     * @brief Sets the buffer to be used by the keyboard.
     *
     *        Sets the buffer to be used by the keyboard.
     *
     * @param [in] newBuffer Pointer to a buffer holding the text edited by the keyboard. If the
     *                       buffer is not empty, the edit position for the keyboard will be set
     *                       to the end of the provided text.
     * @param newBufferSize  Length of the buffer, i.e. number of UnicodeChar's.
     */
    void setBuffer(Unicode::UnicodeChar* newBuffer, uint16_t newBufferSize);

    /**
     * @fn void Keyboard::setLayout(const Layout* newLayout);
     *
     * @brief Set/change the Keyboard::Layout to use.
     *
     *        Set/change the Keyboard::Layout to use.The Keyboard will invalidate the space it
     *        occupies to request a redraw.
     *
     * @param newLayout The new layout.
     */
    void setLayout(const Layout* newLayout);

    /**
     * @fn void Keyboard::setTextIndentation();
     *
     * @brief Sets text indentation.
     *
     *        Sets text indentation by making the area for entered text slightly larger. The
     *        result is that some characters (often 'j' and '_') will not be cut off.
     *        Indentation is added to both sides of the text area in case the text is right-to-
     *        left. Indentation is automatically set so all characters will display properly.
     */
    void setTextIndentation();

    /**
     * @fn const Layout* Keyboard::getLayout() const
     *
     * @brief Gets the layout.
     *
     *        Gets the layout.
     *
     * @return The layout used by the Keyboard.
     */
    const Layout* getLayout() const
    {
        return layout;
    }

    /**
     * @fn void Keyboard::setKeymappingList(const KeyMappingList* newKeyMappingList);
     *
     * @brief Set/change the KeyMappingList to use.
     *
     *        Set/change the KeyMappingList to use. The Keyboard will invalidate the space it
     *        occupies to request a redraw.
     *
     * @param newKeyMappingList The new KeyMappingList.
     */
    void setKeymappingList(const KeyMappingList* newKeyMappingList);

    /**
     * @fn const KeyMappingList* Keyboard::getKeyMappingList() const
     *
     * @brief Gets key mapping list.
     *
     *        Gets key mapping list.
     *
     * @return The KeyMappingList used by the Keyboard.
     */
    const KeyMappingList* getKeyMappingList() const
    {
        return keyMappingList;
    }

    /**
     * @fn void Keyboard::setBufferPosition(uint16_t newPos);
     *
     * @brief Change the buffer position.
     *
     *        Change the buffer position i.e. the next index to place a character when a key is
     *        pressed. This can be used to implement backspace functionality if the class using
     *        the Keyboard implements a callback and maps it to a backspace implementation.
     *        Setting the position will cause the TextArea displaying the text to be
     *        invalidated to request a redraw.
     *
     * @param newPos The buffer position.
     */
    void setBufferPosition(uint16_t newPos);

    /**
     * @fn uint16_t Keyboard::getBufferPosition()
     *
     * @brief Gets buffer position.
     *
     *        Gets buffer position.
     *
     * @return the buffer position, i.e. the current index where new characters will be placed.
     */
    uint16_t getBufferPosition()
    {
        return bufferPosition;
    };

    /**
     * @fn Unicode::UnicodeChar* Keyboard::getBuffer() const
     *
     * @brief Gets the buffer.
     *
     *        Gets the buffer.
     *
     * @return The buffer containing entered text currently being displayed.
     */
    Unicode::UnicodeChar* getBuffer() const
    {
        return buffer;
    }

    /**
     * @fn virtual void Keyboard::draw(const Rect& invalidatedArea) const;
     *
     * @brief Overrides the draw implementation on the Container.
     *
     *        Overrides the draw implementation on the Container. First invokes the container
     *        draw implementation to draw the keyboard bitmap and text area holding the entered
     *        text. If additional drawables have been added to the keyboard, they will also be
     *        draw. After invoking the container draw, the glyphs mapped to keys are drawn and
     *        if a key has been pressed, it will be highlighted.
     *
     * @param invalidatedArea The area to draw.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual void Keyboard::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Overrides the handleClickEvent on the container.
     *
     *        Overrides the handleClickEvent on the container. The keyboard handles all click
     *        events internally and click events are _not_ propagated to drawables added to the
     *        keyboard.
     *
     * @param evt The ClickEvent.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void Keyboard::handleDragEvent(const DragEvent& evt)
     *
     * @brief Overrides the handleDragEvent on the container.
     *
     *        Overrides the handleDragEvent on the container. The keyboard handles drag
     *        events to enable the container to, emit a CANCEL, if the user drags outside the
     *        currently pressed key.
     *
     * @param evt The DragEvent.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn void Keyboard::setKeyListener(GenericCallback<Unicode::UnicodeChar>& callback)
     *
     * @brief Sets the callback for the keyboard.
     *
     *        Sets the callback for the keyboard. The callback will be executed every time a
     *        key is clicked. The callback argument contains the key that was just pressed.
     *
     * @note Backspace, shift and mode keys report a 0 as value.
     *
     * @param [in] callback The Callback to invoke.
     */
    void setKeyListener(GenericCallback<Unicode::UnicodeChar>& callback)
    {
        keyListener = &callback;
    }

    /**
     * @fn virtual uint16_t Keyboard::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_KEYBOARD.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_KEYBOARD;
    }

protected:
    GenericCallback<Unicode::UnicodeChar>* keyListener; ///< Pointer to callback being executed when a key is pressed.

    Unicode::UnicodeChar*   buffer;         ///< Pointer to zero-terminated buffer where the entered text is being displayed.
    uint16_t                bufferSize;     ///< Size of the buffer
    uint16_t                bufferPosition; ///< Current position in buffer.
    Image                   image;          ///< Layout bitmap.
    TextAreaWithOneWildcard enteredText;    ///< Widget capable of displaying the entered text buffer.
    const Layout*           layout;         ///< Pointer to layout.
    const KeyMappingList*   keyMappingList; ///< Pointer to key mapping.
    Image                   highlightImage; ///< Image to display when a key is highlighted.
    bool                    cancelIsEmitted;///< Tells if a cancel is emitted to check when a key is released

    /**
     * @fn Key Keyboard::getKeyForCoordinates(int16_t x, int16_t y) const;
     *
     * @brief Gets key for coordinates.
     *
     *        Gets key for coordinates.
     *
     * @param x The x coordinate to perform key look up with.
     * @param y The y coordinate to perform key look up with.
     *
     * @return The key for the given coordinates.
     */
    Key getKeyForCoordinates(int16_t x, int16_t y) const;

    /**
     * @fn Unicode::UnicodeChar Keyboard::getCharForKey(uint8_t keyId) const;
     *
     * @brief Maps a keyId to the UnicodeChar being displayed by that key.
     *
     *        Maps a keyId to the UnicodeChar being displayed by that key.
     *
     * @param keyId The id of the key to perform lookup with.
     *
     * @return the UnicodeChar used for the specified key.
     */
    Unicode::UnicodeChar getCharForKey(uint8_t keyId) const;

    /**
     * @fn CallbackArea Keyboard::getCallbackAreaForCoordinates(int16_t x, int16_t y) const;
     *
     * @brief Gets the callback area defined by the layout for the specified coordinates.
     *
     *        Gets the callback area defined by the layout for the specified coordinates.
     *
     * @param x The x coordinate to perform key look up with.
     * @param y The y coordinate to perform key look up with.
     *
     * @return The CallbackArea, which is empty if not found.
     */
    CallbackArea getCallbackAreaForCoordinates(int16_t x, int16_t y) const;

    /**
     * @fn virtual void Keyboard::setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);
     *
     * @brief Add to draw chain.
     *
     * @note For TouchGFX internal use only.
     *
     * @param invalidatedArea              Include drawables that intersect with this area only.
     * @param [in,out] nextPreviousElement Modifiable element in linked list.
     */
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);
};
} // namespace touchgfx

#endif // KEYBOARD_HPP
