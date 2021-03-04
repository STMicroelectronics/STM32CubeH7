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

#ifndef FLASHDATAREADER_HPP
#define FLASHDATAREADER_HPP

#include <touchgfx/hal/Types.hpp>
namespace touchgfx
{
/**
 * @class FlashDataReader FlashDataReader.hpp touchgfx/hal/FlashDataReader.hpp
 *
 * @brief This class is an abstract interface for a class reading data from a flash.
 *
 *        This class is an abstract interface for a class reading data
 *        from a flash. The flash can be any type, but is mostly used
 *        for flashes that are not memory mapped. Applications must
 *        implement access to the flash through this interface.
 */
class FlashDataReader
{
public:

    virtual ~FlashDataReader() {}

    /**
     * @fn virtual bool addressIsAddressable(const void* address) = 0;
     *
     * @brief Compute if an address is directly addressable by the MCU.
     *
     *        Compute if an address is directly addressable by the
     *        MCU. The data is addressable it should be read direct
     *        through a pointer and not through this interface.
     *
     * @param address The address in the flash.
     * @return True if the address is addressable by the MCU.
     */
    virtual bool addressIsAddressable(const void* address) = 0;

    /**
     * @fn virtual void copyData(const void* src, void* dst, uint32_t bytes) = 0;
     *
     * @brief Copy data from flash to a buffer.
     *
     *        Copy data from flash to a buffer. This must be a
     *        synchrony method that does not return until the
     *        copy is done.
     *
     * @param src   Address of source data in the flash
     * @param dst   Address of destination buffer in RAM
     * @param bytes Number of bytes to copy.
     */
    virtual void copyData(const void* src, void* dst, uint32_t bytes) = 0;

    /**
     * @fn virtual void startFlashLineRead(const void* src, uint32_t bytes);
     *
     * @brief Initiate a read operation from flash to a buffer.
     *
     *        Initiate a read operation from flash to a buffer. This
     *        can be an asynchrony operation that is still running
     *        after this function returns. Buffers must be handled by
     *        the subclass. LCD16bbbSerialFlash will at most copy 4
     *        bytes times the width of the display.
     *
     * @param src   Address of source data in the flash
     * @param bytes Number of bytes to copy.
     */
    virtual void startFlashLineRead(const void* src, uint32_t bytes);

    /**
     * @fn virtual const uint8_t* waitFlashReadComplete();
     *
     * @brief Waits until the previous startFlashLineRead operation is complete.
     *
     *        Waits until the previous startFlashLineRead operation is
     *        complete. If the startFlashLineRead method is asynchrony,
     *        this method must wait until the previous operation has
     *        completed.
     *
     * @return The address of a buffer containing the read data.
     */
    virtual const uint8_t* waitFlashReadComplete();
};
} // namespace touchgfx
#endif // FLASHDATAREADER_HPP
