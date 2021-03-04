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

#ifndef DMA_HPP
#define DMA_HPP

#include <touchgfx/hal/BlitOp.hpp>
#include <touchgfx/hal/Atomic.hpp>

namespace touchgfx
{
class DMA_Interface;

/**
 * @class DMA_Queue DMA.hpp touchgfx/hal/DMA.hpp
 *
 * @brief This class provides an interface for a FIFO (circular) list used by DMA_Interface and
 *        descendants for storing BlitOp's.
 *
 *        This class provides an interface for a FIFO (circular) list used by DMA_Interface and
 *        descendants for storing BlitOp's.
 */
class DMA_Queue
{
    friend class DMA_Interface;

public:

    /**
     * @fn virtual bool DMA_Queue::isEmpty() = 0;
     *
     * @brief Query if this object is empty.
     *
     *        Query if this object is empty.
     *
     * @return true if the queue is empty.
     */
    virtual bool isEmpty() = 0;

    /**
     * @fn virtual bool DMA_Queue::isFull() = 0;
     *
     * @brief Query if this object is full.
     *
     *        Query if this object is full.
     *
     * @return true if the queue is full.
     */
    virtual bool isFull() = 0;

    /**
     * @fn virtual void DMA_Queue::pushCopyOf(const BlitOp& op) = 0;
     *
     * @brief Adds the specified blitop to the queue.
     *
     *        Adds the specified blitop to the queue.
     *
     * @param op The blitop to add.
     */
    virtual void pushCopyOf(const BlitOp& op) = 0;

    /**
     * @fn virtual DMA_Queue::~DMA_Queue()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~DMA_Queue()
    {
    }

protected:

    /**
     * @fn DMA_Queue::DMA_Queue()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    DMA_Queue()
    {
    }

    /**
     * @fn virtual void DMA_Queue::pop() = 0;
     *
     * @brief Pops an element from the queue.
     *
     *        Pops an element from the queue.
     */
    virtual void pop() = 0;

    /**
     * @fn virtual const BlitOp* DMA_Queue::first() = 0;
     *
     * @brief Gets the first element in the queue.
     *
     *        Gets the first element in the queue.
     *
     * @return The first element in the queue.
     */
    virtual const BlitOp* first() = 0;
};

/**
 * @class LockFreeDMA_Queue DMA.hpp touchgfx/hal/DMA.hpp
 *
 * @brief This implements a simple lock-free FIFO queue (single producer, single consumer).
 *
 *        This implements a simple lock-free FIFO queue (single producer, single consumer)
 *
 * @see DMA_Queue
 */
class LockFreeDMA_Queue : public DMA_Queue
{
public:

    /**
     * @fn LockFreeDMA_Queue::LockFreeDMA_Queue(BlitOp* mem, atomic_t n);
     *
     * @brief Constructs a lockfree queue.
     *
     *        Constructs a lockfree queue.
     *
     * @param [out] mem Pointer to the memory used by the queue to store elements.
     * @param n         Number of elements the memory provided can contain.
     */
    LockFreeDMA_Queue(BlitOp* mem, atomic_t n);

    /**
     * @fn virtual bool LockFreeDMA_Queue::isEmpty();
     *
     * @brief Query if this object is empty.
     *
     *        Query if this object is empty.
     *
     * @return true if empty, false if not.
     */
    virtual bool isEmpty();

    /**
     * @fn virtual bool LockFreeDMA_Queue::isFull();
     *
     * @brief Query if this object is full.
     *
     *        Query if this object is full.
     *
     * @return true if full, false if not.
     */
    virtual bool isFull();

    /**
     * @fn virtual void LockFreeDMA_Queue::pushCopyOf(const BlitOp& op);
     *
     * @brief Push copy of the given operation.
     *
     *        Push copy of the given operation.
     *
     * @param op The operation.
     */
    virtual void pushCopyOf(const BlitOp& op);

protected:

    /**
     * @fn virtual void LockFreeDMA_Queue::pop();
     *
     * @brief Removes the top-of-stack object.
     *
     *        Removes the top-of-stack object.
     */
    virtual void pop();

    /**
     * @fn virtual const BlitOp* LockFreeDMA_Queue::first();
     *
     * @brief Gets the first blit operation.
     *
     * @return the first blitop.
     */
    virtual const BlitOp* first();

    BlitOp*  q;        ///< Pointer to the queue memory.
    atomic_t capacity; ///< The number of elements the queue can contain.
    atomic_t head;     ///< Index to the head element.
    atomic_t tail;     ///< Index to the tail element.
};

/**
 * @class DMA_Interface DMA.hpp touchgfx/hal/DMA.hpp
 *
 * @brief DMA_Interface provides basic functionality and structure for processing "blit" operations
 *        using DMA.
 *
 *        DMA_Interface provides basic functionality and structure for processing "blit"
 *        operations using DMA.
 */
class DMA_Interface
{
public:

    /**
     * @fn virtual BlitOperations DMA_Interface::getBlitCaps() = 0;
     *
     * @brief Gets the blit capabilities of this DMA.
     *
     *        Gets the blit capabilities of this DMA.
     *
     * @return The blit operations supported by this DMA implementation.
     */
    virtual BlitOperations getBlitCaps() = 0;

    /**
     * @fn virtual void DMA_Interface::addToQueue(const BlitOp& op);
     *
     * @brief Inserts a BlitOp for processing.
     *
     *        Inserts a BlitOp for processing. This also potentially starts the DMA controller,
     *        if not already running.
     *
     * @param op The operation to add.
     */
    virtual void addToQueue(const BlitOp& op);

    /**
     * @fn virtual void DMA_Interface::flush()
     *
     * @brief This function blocks until all DMA transfers in the queue have been completed.
     *
     *        This function blocks until all DMA transfers in the queue have been completed.
     */
    virtual void flush()
    {
        waitForFrameBufferSemaphore();
    }

    /**
     * @fn virtual void DMA_Interface::initialize()
     *
     * @brief Perform initialization.
     *
     *        Perform initialization. Does nothing in this base class.
     */
    virtual void initialize()
    {
    }

    /**
     * @fn bool DMA_Interface::isDMARunning()
     *
     * @brief Query if the DMA is running.
     *
     *        Query if the DMA is running.
     *
     * @return true if a DMA operation is currently in progress.
     */
    bool isDMARunning()
    {
        return isRunning;
    }

    /**
     * @fn void DMA_Interface::setAllowed(bool allowed)
     *
     * @brief Sets whether or not a DMA operation is allowed to begin.
     *
     *        Sets whether or not a DMA operation is allowed to begin. Used in single-buffering
     *        to avoid changing the frame buffer while display is being updated.
     *
     * @param allowed true if DMA transfers are allowed.
     */
    void setAllowed(bool allowed)
    {
        isAllowed = allowed;
    }

    /**
     * @fn bool DMA_Interface::getAllowed() const
     *
     * @brief Gets whether a DMA operation is allowed to begin.
     *
     *        Gets whether a DMA operation is allowed to begin. Used in single-buffering to
     *        avoid changing the frame buffer while display is being updated.
     *
     * @return true if DMA is allowed to start, false if not.
     */
    bool getAllowed() const
    {
        return isAllowed;
    }

    /**
     * @fn virtual void DMA_Interface::start();
     *
     * @brief Signals that DMA transfers can start.
     *
     *        Signals that DMA transfers can start. If any elements are in the queue, start it.
     */
    virtual void start();

    /**
     * @fn virtual void DMA_Interface::signalDMAInterrupt() = 0;
     *
     * @brief This function is called automatically by the framework when a DMA interrupt has been
     *        received.
     *
     *        This function is called automatically by the framework when a DMA interrupt has
     *        been received.
     */
    virtual void signalDMAInterrupt() = 0;

    /**
     * @fn uint8_t DMA_Interface::isDmaQueueEmpty();
     *
     * @brief Query if the DMA queue is empty.
     *
     *        Query if the DMA queue is empty.
     *
     * @return 1 if DMA queue is empty, else 0.
     */
    uint8_t isDmaQueueEmpty();

    /**
     * @fn uint8_t DMA_Interface::isDmaQueueFull();
     *
     * @brief Query if the DMA queue is full.
     *
     *        Query if the DMA queue is full.
     *
     * @return 1 if DMA queue is full, else 0.
     */
    uint8_t isDmaQueueFull();

    /**
     * @fn virtual DMAType DMA_Interface::getDMAType()
     *
     * @brief Function for obtaining the DMA type of the concrete DMA_Interface implementation.
     *
     *        Function for obtaining the DMA type of the concrete DMA_Interface implementation.
     *        As default, will return DMA_TYPE_GENERIC type value.
     *
     * @return a DMAType value of the concrete DMA_Interface implementation.
     */
    virtual DMAType getDMAType(void)
    {
        return DMA_TYPE_GENERIC;
    }

    /**
     * @fn virtual DMA_Interface::~DMA_Interface()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~DMA_Interface()
    {
    }

protected:

    /**
     * @fn DMA_Interface::DMA_Interface(DMA_Queue& dmaQueue)
     *
     * @brief Constructs a DMA Interface object.
     *
     *        Constructs a DMA Interface object.
     *
     * @param [in] dmaQueue Reference to the queue of DMA operations.
     */
    DMA_Interface(DMA_Queue& dmaQueue)
        : queue(dmaQueue), isRunning(false), isAllowed(false)
    {
    }

    /**
     * @fn virtual void DMA_Interface::execute();
     *
     * @brief Performs a queued blit-op.
     *
     *        Performs a queued blit-op.
     */
    virtual void execute();

    /**
     * @fn virtual void DMA_Interface::executeCompleted();
     *
     * @brief To be called when blit-op has been performed.
     *
     *        To be called when blit-op has been performed.
     */
    virtual void executeCompleted();

    /**
     * @fn virtual void DMA_Interface::seedExecution();
     *
     * @brief Called when elements are added to the DMA-queue.
     *
     *        Called when elements are added to the DMA-queue.
     *
     * @note The frame buffer must be locked before this method returns if the DMA-queue is non-
     *       empty.
     */
    virtual void seedExecution();

    /**
     * @fn virtual void DMA_Interface::setupDataCopy(const BlitOp& blitOp) = 0;
     *
     * @brief Configures blit-op hardware for a 2D copy as specified by blitOp.
     *
     *        Configures blit-op hardware for a 2D copy as specified by blitOp.
     *
     * @param blitOp The operation to execute.
     */
    virtual void setupDataCopy(const BlitOp& blitOp) = 0;

    /**
     * @fn virtual void DMA_Interface::setupDataFill(const BlitOp& blitOp) = 0;
     *
     * @brief Configures blit-op hardware for a 2D fill as specified by blitOp.
     *
     *        Configures blit-op hardware for a 2D fill as specified by blitOp.
     *
     * @param blitOp The operation to execute.
     */
    virtual void setupDataFill(const BlitOp& blitOp) = 0;

    /**
     * @fn virtual void DMA_Interface::enableAlpha(uint8_t alpha);
     *
     * @brief Configures blit-op hardware for alpha-blending.
     *
     *        Configures blit-op hardware for alpha-blending.
     *
     * @param alpha The alpha-blending value to apply.
     */
    virtual void enableAlpha(uint8_t alpha);

    /**
     * @fn virtual void DMA_Interface::disableAlpha();
     *
     * @brief Configures blit-op hardware for solid operation (no alpha-blending).
     *
     *        Configures blit-op hardware for solid operation (no alpha-blending)
     */
    virtual void disableAlpha();

    /**
     * @fn virtual void DMA_Interface::enableCopyWithTransparentPixels(uint8_t alpha);
     *
     * @brief Configures blit-op hardware for alpha-blending while simultaneously skipping
     *        transparent pixels.
     *
     *        Configures blit-op hardware for alpha-blending while simultaneously skipping
     *        transparent pixels.
     *
     * @param alpha The alpha-blending value to apply.
     */
    virtual void enableCopyWithTransparentPixels(uint8_t alpha);

    /**
     * @fn virtual void DMA_Interface::waitForFrameBufferSemaphore();
     *
     * @brief Waits until frame buffer semaphore is available.
     *
     *        Waits until frame buffer semaphore is available (i.e. neither DMA or application
     *        is accessing the frame buffer).
     */
    virtual void waitForFrameBufferSemaphore();

    DMA_Queue&    queue;     ///< Reference to the DMA queue
    bool          isRunning; ///< true if a DMA transfer is currently ongoing.
    volatile bool isAllowed; ///< true if DMA transfers are currently allowed.
};
} // namespace touchgfx

#endif // DMA_HPP
