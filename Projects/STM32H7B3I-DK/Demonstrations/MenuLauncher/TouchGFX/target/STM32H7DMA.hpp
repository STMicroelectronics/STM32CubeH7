
#ifndef STM32H7DMA_HPP
#define STM32H7DMA_HPP

#include <touchgfx/hal/DMA.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

/**
 * @class STM32H7DMA STM32H7DMA.hpp
 *
 * @brief This class specializes DMA_Interface for the ST F7xx processors.
 *
 * @sa touchgfx::DMA_Interface
 */
class STM32H7DMA : public touchgfx::DMA_Interface
{
    /**
     * @typedef touchgfx::DMA_Interface Base
     *
     * @brief Defines an alias representing the base.
     *
     Defines an alias representing the base.
     */
    typedef touchgfx::DMA_Interface Base;
public:
    /**
     * @fn STM32H7DMA::STM32H7DMA();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    STM32H7DMA();

    /**
     * @fn STM32H7DMA::~STM32H7DMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~STM32H7DMA();

    /**
     * @fn touchgfx::BlitOperations STM32H7DMA::getBlitCaps();
     *
     * @brief Gets the blit capabilities.
     *
     *        Gets the blit capabilities.
     *
     *        This DMA supports a range of blit caps: BLIT_OP_COPY, BLIT_OP_COPY_ARGB8888,
     *        BLIT_OP_COPY_ARGB8888_WITH_ALPHA, BLIT_OP_COPY_A4, BLIT_OP_COPY_A8.
     *
     * @note For demonstration purposes, the blitcaps can be overridden at runtime to return zero
     *       by setting the overrideBlitCaps boolean to true.
     *
     * @return Currently supported blitcaps.
     */
    virtual touchgfx::BlitOperations getBlitCaps();

    /**
     * @fn void STM32H7DMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    virtual void initialize();

    /**
     * @fn void STM32H7DMA::signalDMAInterrupt()
     *
     * @brief Raises a DMA interrupt signal.
     *
     *        Raises a DMA interrupt signal.
     */
    virtual void signalDMAInterrupt()
    {
        executeCompleted();
    }

    /**
     * @fn void STM32H7DMA::start();
     *
     * @brief Signals that DMA transfers can start.
     *
     *        Signals that DMA transfers can start. If any elements are in the queue, start it.
     */
    virtual void start();

protected:
    /**
     * @fn virtual void STM32H7DMA::setupDataCopy(const touchgfx::BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame buffer.
     *
     *        Configures the DMA for copying data to the frame buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const touchgfx::BlitOp& blitOp);

    /**
     * @fn virtual void STM32H7DMA::setupDataFill(const touchgfx::BlitOp& blitOp);
     *
     * @brief Configures the DMA for "filling" the frame-buffer with a single color.
     *
     *        Configures the DMA for "filling" the frame-buffer with a single color.
     *
     * @param blitOp Details on the "fill" to perform.
     */
    virtual void setupDataFill(const touchgfx::BlitOp& blitOp);

private:
    touchgfx::LockFreeDMA_Queue dma_queue;
    touchgfx::BlitOp            queue_storage[96];
};

#endif // STM32H7DMA_HPP
