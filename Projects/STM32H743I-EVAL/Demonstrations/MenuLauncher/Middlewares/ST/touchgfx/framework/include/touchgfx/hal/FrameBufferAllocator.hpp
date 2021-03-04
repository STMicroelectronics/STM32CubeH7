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

#ifndef FRAMEBUFFERALLOCATOR_HPP
#define FRAMEBUFFERALLOCATOR_HPP
#include <touchgfx/Utils.hpp>

namespace touchgfx
{

/**
 * @fn void FrameBufferAllocatorWaitOnTransfer();
 *
 * @brief Called by FrameBufferAllocator to wait for a LCD Transfer.
 *
 *        Called by FrameBufferAllocator to wait for a LCD Transfer,
 *        when the allocator has no free blocks. The LCD driver can
 *        use this function to synchronize the UI thread with the
 *        transfer logic.
 */
void FrameBufferAllocatorWaitOnTransfer();

/**
 * @fn void FrameBufferAllocatorSignalBlockDrawn();
 *
 * @brief Called by FrameBufferAllocator when a block is drawn.
 *
 *        Called by FrameBufferAllocator when a block is drawn and
 *        therefore ready for transfer. The LCD driver should use this
 *        method to start a transfer.
 */
void FrameBufferAllocatorSignalBlockDrawn();

/**
 * @class FrameBufferAllocator FrameBufferAllocator.hpp touchgfx/hal/FrameBufferAllocator.hpp
 *
 * @brief This class is an abstract interface for a class allocating partial framebuffer blocks.
 *
 *        This class is an abstract interface for a class allocating
 *        partial framebuffer blocks. The interface must be implemented by a subclass.
 *
 * @see SingleBlockAllocator, ManyBlockAllocator
 */
class FrameBufferAllocator
{
public:
    /**
     * @fn virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block) = 0;
     *
     * @brief Allocates a framebuffer block.
     *
     *        Allocates a framebuffer block. The block will have at
     *        least the width requested. The height of the allocated
     *        block can be lower than requested if not enough memory
     *        is available.
     *
     * @param x            The absolute x coordinate of the block on the screen.
     * @param y            The absolute y coordinate of the block on the screen.
     * @param width        The width of the block.
     * @param height       The height of the block.
     * @param block        Pointer to pointer to return the block address in.
     *
     * @returns The height of the allocated block.
     *
     */
    virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block) = 0;

    /**
     * @fn virtual void markBlockReadyForTransfer() = 0;
     *
     * @brief Marks a previously allocated block as ready to be transferred to the LCD.
     *
     *        Marks a previously allocated block as ready to be
     *        transferred to the LCD.
     *
     */
    virtual void markBlockReadyForTransfer() = 0; //only one block can be allocated for drawing

    /**
     * @fn virtual bool hasBlockReadyForTransfer() = 0;
     *
     * @brief Check if a block is ready for transfer to the LCD.
     *
     *        Check if a block is ready for transfer to the LCD.
     *
     * @returns True if a block is ready for transfer.
     */
    virtual bool hasBlockReadyForTransfer() = 0;

    /**
     * @fn virtual const uint8_t* getBlockForTransfer(Rect& rect) = 0;
     *
     * @brief Get the block ready for transfer.
     *
     *        Get the block ready for transfer.
     *
     * @param rect  Reference to rect to write block x, y, width, and height.
     * @return Returns the address of the block ready for transfer.
     */
    virtual const uint8_t* getBlockForTransfer(Rect& rect) = 0;

    /**
     * @fn virtual void freeBlockAfterTransfer() = 0;
     *
     * @brief Free a block after transfer to the LCD.
     *
     *        Marks a previously allocated block as transferred and
     *        ready to reuse.
     */
    virtual void freeBlockAfterTransfer() = 0;

    virtual ~FrameBufferAllocator() { }
};

/**
 * @class SingleBlockAllocator FrameBufferAllocator.hpp touchgfx/hal/FrameBufferAllocator.hpp
 *
 * @brief This class is partial framebuffer allocator using just one block.
 *
 *        This class is partial framebuffer allocator using just one
 *        block. No new buffer can be allocated until the block has
 *        been transferred to LCD.
 *
 * @see ManyBlockAllocator
 */
template <uint16_t block_size, uint32_t bytes_pr_pixel>
class SingleBlockAllocator : public FrameBufferAllocator
{
public:
    SingleBlockAllocator() : state(EMPTY) {}
    /**
     * @fn virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block);
     *
     * @brief Allocates a framebuffer block.
     *
     *        Allocates a framebuffer block. The block will have at
     *        least the width requested. The height of the allocated
     *        block can be lower than requested if not enough memory
     *        is available. This class calls
     *        FrameBufferAllocatorWaitOnTransfer() if no block is
     *        available.
     *
     * @param x            The absolute x coordinate of the block on the screen.
     * @param y            The absolute y coordinate of the block on the screen.
     * @param width        The width of the block.
     * @param height       The height of the block.
     * @param block        Pointer to pointer to return the block address in.
     *
     * @returns The height of the allocated block.
     */
    virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block)
    {
        while (state != EMPTY)
        {
            FrameBufferAllocatorWaitOnTransfer();
        }
        assert(state == EMPTY);
        state = ALLOCATED;
        const int32_t stride = width * bytes_pr_pixel;
        const int32_t lines = block_size / stride;
        *block = (uint8_t*)&memory[0];
        blockRect.x = x;
        blockRect.y = y;
        blockRect.width = width;
        blockRect.height = MIN(height, lines);
        return blockRect.height;
    }

    /**
     * @fn virtual void markBlockReadyForTransfer();
     *
     * @brief Marks a previously allocated block as ready to be transferred to the LCD.
     *
     *        Marks a previously allocated block as ready to be
     *        transferred to the LCD.
     *
     */
    virtual void markBlockReadyForTransfer()
    {
        assert(state == ALLOCATED);
        state = DRAWN;
        FrameBufferAllocatorSignalBlockDrawn();
    }

    /**
     * @fn virtual bool hasBlockReadyForTransfer();
     *
     * @brief Check if a block is ready for transfer to the LCD.
     *
     *        Check if a block is ready for transfer to the LCD.
     *
     * @returns True if a block is ready for transfer.
     */
    virtual bool hasBlockReadyForTransfer()
    {
        return (state == DRAWN);
    }

    /**
     * @fn virtual const uint8_t* getBlockForTransfer(Rect& rect);
     *
     * @brief Get the block ready for transfer.
     *
     *        Get the block ready for transfer.
     *
     * @param rect  Reference to rect to write block x, y, width, and height.
     * @return Returns the address of the block ready for transfer.
     */
    virtual const uint8_t* getBlockForTransfer(Rect& rect)
    {
        rect = const_cast<const Rect&>(blockRect);
        assert(state == DRAWN);
        state = SENDING;
        return (const uint8_t*)&memory[0];
    }

    /**
     * @fn virtual void freeBlockAfterTransfer();
     *
     * @brief Free a block after transfer to the LCD.
     *
     *        Marks a previously allocated block as transferred and
     *        ready to reuse.
     */
    virtual void freeBlockAfterTransfer()
    {
        assert(state == SENDING);
        state = EMPTY;
    }
private:
    enum BlockState
    {
        EMPTY,
        ALLOCATED,
        DRAWN,
        SENDING
    };
    volatile BlockState state;
    uint32_t memory[block_size / 4];
    volatile Rect blockRect;
};


/**
 * @class ManyBlockAllocator FrameBufferAllocator.hpp touchgfx/hal/FrameBufferAllocator.hpp
 *
 * @brief This class is partial framebuffer allocator using multiple blocks.
 *
 *        This class is partial framebuffer allocator using multiple
 *        blocks. New buffers can be allocated until no free blocks
 *        are available. After transfer to LCD, a block is queued for
 *        allocation again.
 *
 * @see SingleBlockAllocator
 */
template <uint32_t block_size, int32_t blocks, uint32_t bytes_pr_pixel>
class ManyBlockAllocator : public FrameBufferAllocator
{
public:
    ManyBlockAllocator()
    {
        sendingBlock = -1;
        drawingBlock = -1;
        for (int i = 0; i < blocks; i++)
        {
            state[i] = EMPTY;
        }
    }

    /**
     * @fn virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block) = 0;
     *
     * @brief Allocates a framebuffer block.
     *
     *        Allocates a framebuffer block. The block will have at
     *        least the width requested. The height of the allocated
     *        block can be lower than requested if not enough memory
     *        is available.
     *
     * @param x            The absolute x coordinate of the block on the screen.
     * @param y            The absolute y coordinate of the block on the screen.
     * @param width        The width of the block.
     * @param height       The height of the block.
     * @param block        Pointer to pointer to return the block address in.
     *
     * @returns The height of the allocated block.
     *
     */
    virtual uint16_t allocateBlock(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, uint8_t** block)
    {
        drawingBlock++;
        if (drawingBlock == blocks)
        {
            drawingBlock = 0;
        }
        while (state[drawingBlock] != EMPTY)
        {
            FrameBufferAllocatorWaitOnTransfer();
        }
        assert(state[drawingBlock] == EMPTY);
        state[drawingBlock] = ALLOCATED;
        const int32_t stride = width * bytes_pr_pixel;
        const int32_t lines = block_size / stride;
        *block = (uint8_t*)&memory[drawingBlock][0];
        blockRect[drawingBlock].x = x;
        blockRect[drawingBlock].y = y;
        blockRect[drawingBlock].width = width;
        blockRect[drawingBlock].height = MIN(height, lines);
        //touchgfx_printf("Draw: %d, %dx%d\n", drawingBlock, width, blockRect[drawingBlock].height);
        return blockRect[drawingBlock].height;
    }

    /**
     * @fn virtual void markBlockReadyForTransfer();
     *
     * @brief Marks a previously allocated block as ready to be transferred to the LCD.
     *
     *        Marks a previously allocated block as ready to be
     *        transferred to the LCD.
     *
     */
    virtual void markBlockReadyForTransfer()
    {
        assert(state[drawingBlock] == ALLOCATED);
        state[drawingBlock] = DRAWN;
        //drawingBlock = -1;
        FrameBufferAllocatorSignalBlockDrawn();
    }

    /**
     * @fn virtual bool hasBlockReadyForTransfer();
     *
     * @brief Check if a block is ready for transfer to the LCD.
     *
     *        Check if a block is ready for transfer to the LCD.
     *
     * @returns True if a block is ready for transfer.
     */
    virtual bool hasBlockReadyForTransfer()
    {
        for (int i = 0; i < blocks; i++)
        {
            if (state[i] == DRAWN)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @fn virtual const uint8_t* getBlockForTransfer(Rect& rect);
     *
     * @brief Get the block ready for transfer.
     *
     *        Get the block ready for transfer.
     *
     * @param rect  Reference to rect to write block x, y, width, and height.
     * @return Returns the address of the block ready for transfer.
     */
    virtual const uint8_t* getBlockForTransfer(Rect& rect)
    {
        sendingBlock++;
        if (sendingBlock == blocks)
        {
            sendingBlock = 0;
        }
        //touchgfx_printf("Trans: %d\n", sendingBlock);
        assert(state[sendingBlock] == DRAWN);
        rect = blockRect[sendingBlock];
        state[sendingBlock] = SENDING;
        sendingBlock = sendingBlock;
        return (const uint8_t*)&memory[sendingBlock][0];
    }

    /**
     * @fn virtual void freeBlockAfterTransfer();
     *
     * @brief Free a block after transfer to the LCD.
     *
     *        Marks a previously allocated block as transferred and
     *        ready to reuse.
     */
    virtual void freeBlockAfterTransfer()
    {
        assert(state[sendingBlock] == SENDING);
        state[sendingBlock] = EMPTY;
    }
private:
    enum BlockState
    {
        EMPTY,
        ALLOCATED,
        DRAWN,
        SENDING
    };
    volatile BlockState state[blocks];
    uint32_t memory[blocks][block_size / 4];
    Rect blockRect[blocks];
    int sendingBlock;
    int drawingBlock;
};

}

#endif // FRAMEBUFFERALLOCATOR_HPP
