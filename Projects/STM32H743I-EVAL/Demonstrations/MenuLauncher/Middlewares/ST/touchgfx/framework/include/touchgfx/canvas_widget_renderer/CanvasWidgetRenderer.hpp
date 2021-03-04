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

#ifndef CANVASWIDGETRENDERER_HPP
#define CANVASWIDGETRENDERER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/canvas_widget_renderer/Cell.hpp>
#ifdef SIMULATOR
#include <touchgfx/canvas_widget_renderer/Cell.hpp>
#include <stdio.h> // For optional memory footprint reporting
#endif

namespace touchgfx
{
/**
 * @class CanvasWidgetRenderer CanvasWidgetRenderer.hpp touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp
 *
 * @brief Class for supporting drawing of figures.
 *
 *        Class for supporting drawing of figures. This class holds the memory which is used by
 *        the underlying algorithms. CanvasWidget will not allocate memory dynamically, but
 *        will use memory from the buffer kept in CanvasWidgetRenderer. When using the TouchGFX
 *        simulator, it is also possible to get a report on the actual amount of memory used
 *        for the drawings to help adjusting the buffer size.
 *
 * @see Widget
 */
class CanvasWidgetRenderer : public Widget
{
public:

    /**
     * @fn static void CanvasWidgetRenderer::setupBuffer(uint8_t* buffer, unsigned bufsize)
     *
     * @brief Setup the buffers used by CanvasWidget.
     *
     *        Setup the buffers used by CanvasWidget.
     *
     * @param [in] buffer Buffer reserved for CanvasWidget.
     * @param bufsize     The size of the buffer.
     */
    static void setupBuffer(uint8_t* buffer, unsigned bufsize);

    /**
     * @fn static bool CanvasWidgetRenderer::setScanlineWidth(unsigned width)
     *
     * @brief Sets scanline width.
     *
     *        Sets scanline width. Setting the scanline width will initialize the buffers for
     *        scanline and outline. If the width set is too large to fit the scanline buffers
     *        in the allocated memory buffer, false will be returned and all buffer pointers
     *        will be cleared.
     *
     * @param width The width of the scanline on screen.
     *
     * @return true if it succeeds, false if it fails.
     */
    static bool setScanlineWidth(unsigned width);

    /**
     * @fn static bool CanvasWidgetRenderer::hasBuffer()
     *
     * @brief Query if CanvasWidgetRenderer has been initialized with a buffer.
     *
     *        Query if CanvasWidgetRenderer has been initialized with a buffer.
     *
     * @return True if a buffer has been setup.
     */
    static bool hasBuffer();

    /**
     * @fn static unsigned CanvasWidgetRenderer::getScanlineWidth()
     *
     * @brief The width of a scanline.
     *
     *        The width of a scanline. This is the same as the width of the invalidated area.
     *        Used to optimize the memory layout of the buffer.
     *
     * @return Scanline width (HAL::FRAME_BUFFER_WIDTH).
     */
    static unsigned getScanlineWidth();

    /**
     * @fn static void* CanvasWidgetRenderer::getScanlineCovers()
     *
     * @brief Gets pointer to memory used for covers in Scanline.
     *
     *        Gets pointer to memory used for covers in Scanline.
     *
     * @return Pointer to memory used internally by Scanline.
     */
    static void* getScanlineCovers();

    /**
     * @fn static void* CanvasWidgetRenderer::getScanlineStartIndices()
     *
     * @brief Gets pointer to memory used for indices in Scanline.
     *
     *        Gets pointer to memory used for indices in Scanline.
     *
     * @return Pointer to memory used internally by Scanline.
     */
    static void* getScanlineStartIndices();

    /**
     * @fn static void* CanvasWidgetRenderer::getScanlineCounts()
     *
     * @brief Gets pointer to memory used for counts in Scanline.
     *
     *        Gets pointer to memory used for counts in Scanline.
     *
     * @return Pointer to memory used internally by Scanline.
     */
    static void* getScanlineCounts();

    /**
     * @fn static Cell* CanvasWidgetRenderer::getOutlineBuffer()
     *
     * @brief Gets pointer to memory used for Cell objects in Outline.
     *
     *        Gets pointer to memory used for Cell objects in Outline.
     *
     * @return Pointer to memory used internally by Outline.
     */
    static Cell* getOutlineBuffer();

    /**
     * @fn static unsigned int CanvasWidgetRenderer::getOutlineBufferSize()
     *
     * @brief Gets size of memory area used for Cell objects in Outline.
     *
     *        Gets size of memory area used for Cell objects in Outline.
     *
     * @return Size of memory area used internally by Outline.
     */
    static unsigned int getOutlineBufferSize();

#ifdef SIMULATOR

    /**
     * @fn static void CanvasWidgetRenderer::setWriteMemoryUsageReport(bool writeUsageReport);
     *
     * @brief Memory reporting.
     *
     *        Memory reporting can be turned on (and off) using this method. CWR will try to
     *        work with the given amount of memory passed when calling setupBuffer(). If the
     *        outline of the figure is too complex, this will be reported.
     *
     *        "CWR requires X bytes" means that X bytes is the highest number of bytesd required
     *        by CWR so far, but since the size of the invalidated area and the shape of things
     *        draw can influence this, this may be reported several times with a higher and
     *        higher number. Leave your app running for a long time to find out what the memory
     *        requirements are.
     *
     *        "CWR requires X bytes (Y bytes missing)" means the same as the report above, but
     *        there as was not enough memory to render the entire shape. To get around this,
     *        CWR will split the shape into two separate drawings of half size. This means that
     *        less memory is required, but drawing will be (somewhat)
     *        slower. After you see this message all future draws will be split into smaller
     *        chunks, so memory requirements might not get as high. This is followed by:
     *
     *        "CWR will split draw into multiple draws due to limited memory." actually just means
     *        that CWR will try to work with a smaller amount of memory.
     *
     *        In general, if there is enough memory available to run the simulation and never
     *        see the message "CWR will split draw ...", this is preferred. The size of the
     *        buffer required will be the highest number X reported as "CWR requires X bytes".
     *        Good numbers can also be around half of X.
     *
     * @param writeUsageReport true to write report.
     *
     * @see setupBuffer
     */
    static void setWriteMemoryUsageReport(bool writeUsageReport);

    /**
     * @fn static bool CanvasWidgetRenderer::getWriteMemoryUsageReport();
     *
     * @brief Gets write memory usage report flag.
     *
     *        Gets write memory usage report flag.
     *
     * @return true if it CWR writes memory reports, false if not.
     */
    static bool getWriteMemoryUsageReport();

    /**
     * @fn static void CanvasWidgetRenderer::numCellsUsed(unsigned used)
     *
     * @brief Called by the destructor in Outline to help keep track of the memory requirements of
     *        CanvasWidgets.
     *
     *        Called by the destructor in Outline to help keep track of the memory requirements
     *        of CanvasWidgets.
     *
     * @param used Number of Cell objects used from the dedicated buffer.
     */
    static void numCellsUsed(unsigned used);

    /**
     * @fn static void CanvasWidgetRenderer::numCellsMissing(unsigned missing)
     *
     * @brief Called by the destructor in Outline to help keep track of the memory requirements of
     *        CanvasWidgets.
     *
     *        Called by the destructor in Outline to help keep track of the memory requirements
     *        of CanvasWidgets.
     *
     * @param missing Number of Cell objects required, but not available, to Outline.
     */
    static void numCellsMissing(unsigned missing);

    /**
     * @fn static unsigned CanvasWidgetRenderer::getUsedBufferSize()
     *
     * @brief Calculate how much memory has been required by CanvasWidgets.
     *
     *        Calculate how much memory has been required by CanvasWidgets. This can be used to
     *        fine tune the size of the buffer passed to CanvasWidgetRenderer upon
     *        initialization.
     *
     * @return The number of bytes required.
     */
    static unsigned getUsedBufferSize();

    /**
     * @fn static unsigned CanvasWidgetRenderer::getMissingBufferSize()
     *
     * @brief Calculate how much memory was required by CanvasWidgets, but was unavailable.
     *
     *        Calculate how much memory was required by CanvasWidgets, but was unavailable. If
     *        the value returned is greater than 0 it means the This can be used to fine tune
     *        the size of the buffer passed to CanvasWidgetRenderer upon initialization.
     *
     * @return The number of bytes required.
     */
    static unsigned getMissingBufferSize();
#endif

private:
    static uint8_t* memoryBuffer;
    static unsigned int memoryBufferSize;
    static unsigned int scanlineWidth;
    static void* scanlineCovers;
    static void* scanlineStartIndices;
    static void* scanlineCounts;
    static Cell* outlineBuffer;
    static unsigned int outlineBufferSize;
#ifdef SIMULATOR
    static unsigned int scanlineSize;
    static unsigned int maxCellsUsed;
    static unsigned int maxCellsMissing;
    static bool writeReport;
#endif
}; // class CanvasWidgetRenderer
} // namespace touchgfx

#endif // CANVASWIDGETRENDERER_HPP
