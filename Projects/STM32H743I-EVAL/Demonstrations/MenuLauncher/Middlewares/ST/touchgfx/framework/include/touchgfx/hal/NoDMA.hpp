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

#ifndef NODMA_HPP
#define NODMA_HPP

#include <touchgfx/hal/DMA.hpp>
#include <assert.h>

namespace touchgfx
{
/**
 * @class NoDMA NoDMA.hpp touchgfx/hal/NoDMA.hpp
 *
 * @brief This is an "empty" DMA subclass that does nothing except assert if accidentally used.
 *
 *        This is an "empty" DMA subclass that does nothing except assert if accidentally used.
 *        An instance of this object can be used if DMA support is not desired.
 *
 * @see DMA_Interface
 */
class NoDMA : public DMA_Interface
{
public:

    /**
     * @fn NoDMA::NoDMA()
     *
     * @brief Default constructor.
     *
     *        Constructs a NoDMA object, with a queue of 1 element.
     */
    NoDMA() : DMA_Interface(q), q(&b, 1) { }

    virtual ~NoDMA() { }

    /**
     * @fn virtual BlitOperations NoDMA::getBlitCaps()
     *
     * @brief No blit operations supported by this DMA implementation.
     *
     *        No blit operations supported by this DMA implementation.
     *
     * @return Zero (no blit ops supported).
     */
    virtual BlitOperations getBlitCaps()
    {
        return static_cast<BlitOperations>(0);
    }

    /**
     * @fn virtual void NoDMA::setupDataCopy(const BlitOp& blitOp)
     *
     * @brief Asserts if used.
     *
     *        Asserts if used.
     *
     * @param blitOp The blit operation to be performed by this DMA instance.
     */
    virtual void setupDataCopy(const BlitOp& blitOp)
    {
        assert(0 && "DMA operation not supported");
    }

    /**
     * @fn virtual void NoDMA::setupDataFill(const BlitOp& blitOp)
     *
     * @brief Asserts if used.
     *
     *        Asserts if used.
     *
     * @param blitOp The blit operation to be performed by this DMA instance.
     */
    virtual void setupDataFill(const BlitOp& blitOp)
    {
        assert(0 && "DMA operation not supported");
    }

    /**
     * @fn virtual void NoDMA::signalDMAInterrupt()
     *
     * @brief Does nothing.
     *
     *        Does nothing.
     */
    virtual void signalDMAInterrupt()
    {
    }

    /**
     * @fn virtual void NoDMA::flush()
     *
     * @brief Does nothing.
     *
     *        Block until all DMA transfers are complete. Since this particular DMA does not do
     *        anything, return immediately.
     */
    virtual void flush() { }

private:
    LockFreeDMA_Queue q;
    BlitOp            b;
};
} // namespace touchgfx
#endif // NODMA_HPP
