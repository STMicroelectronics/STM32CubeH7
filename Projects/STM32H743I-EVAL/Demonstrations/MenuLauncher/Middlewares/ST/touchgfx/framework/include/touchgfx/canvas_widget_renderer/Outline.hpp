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

#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include <touchgfx/canvas_widget_renderer/Cell.hpp>

namespace touchgfx
{
/**
 * @class Outline Outline.hpp touchgfx/canvas_widget_renderer/Outline.hpp
 *
 * @brief An internal class that implements the main rasterization algorithm.
 *
 *        An internal class that implements the main rasterization algorithm. Used in the
 *        Rasterizer. Should not be used directly.
 */
class Outline
{
public:
    /**
     * @typedef unsigned int OutlineFlags_t
     *
     * @brief Defines an alias representing the outline flags.
     *
     *        Defines an alias representing the outline flags.
     */
    typedef unsigned int OutlineFlags_t;

    static const OutlineFlags_t OUTLINE_NOT_CLOSED = 1U;    ///< If this bit is set in flags, the current Outline has not yet been closed. Used for automatic closing an Outline before rendering the Outline.
    static const OutlineFlags_t OUTLINE_SORT_REQUIRED = 2U; ///< If this bit is set in flags, Cell objects have been added to the Outline requiring the Cell list needs to be sorted.

    /**
     * @fn Outline::Outline();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Outline();

    /**
     * @fn Outline::~Outline();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Outline();

    /**
     * @fn void Outline::reset();
     *
     * @brief Resets this object.
     *
     *        Resets this object. This implies removing the current Cell objects and preparing
     *        for a new Outline.
     */
    void reset();

    /**
     * @fn void Outline::moveTo(int x, int y);
     *
     * @brief Move a virtual pen to the specified coordinate.
     *
     *        Move a virtual pen to the specified coordinate.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void moveTo(int x, int y);

    /**
     * @fn void Outline::lineTo(int x, int y);
     *
     * @brief Create a line from the current virtual pen coordinate to the given coordinate
     *        creating an Outline.
     *
     *        Create a line from the current virtual pen coordinate to the given coordinate
     *        creating an Outline.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void lineTo(int x, int y);

    /**
     * @fn unsigned Outline::getNumCells() const
     *
     * @brief Gets number cells registered in the current drawn path for the Outline.
     *
     *        Gets number cells registered in the current drawn path for the Outline.
     *
     * @return The number of cells.
     */
    unsigned getNumCells() const
    {
        return numCells;
    }

    /**
     * @fn const Cell* Outline::getCells();
     *
     * @brief Gets a pointer to the the Cell objects in the Outline.
     *
     *        Gets a pointer to the the Cell objects in the Outline. If the Outline is not
     *        closed, it is closed. If the Outline is unsorted, it will be quick sorted first.
     *
     * @return A pointer to the sorted list of Cell objects in the Outline.
     */
    const Cell* getCells();

    /**
     * @fn void Outline::setMaxRenderY(int y)
     *
     * @brief Sets maximum render y coordinate.
     *
     *        Sets maximum render y coordinate. This is used to avoid registering any Cell that
     *        has a y coordinate less than zero of higher than the given y.
     *
     * @param y The max y coordinate to render for the Outline.
     */
    void setMaxRenderY(int y)
    {
        maxRenderY = y;
    }

    /**
     * @fn bool Outline::wasOutlineTooComplex()
     *
     * @brief Determines if there was enough memory to register the entire outline.
     *
     *        Determines if there was enough memory to register the entire outline, of if the
     *        outline was too complex.
     *
     * @return false if the buffer for Outline Cell objects was too small.
     */
    bool wasOutlineTooComplex()
    {
        return outlineTooComplex;
    }

private:
    /**
     * @fn void Outline::setCurCell(int x, int y);
     *
     * @brief Sets coordinate of the current Cell.
     *
     *        Sets coordinate of the current Cell.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setCurCell(int x, int y);

    /**
     * @fn void Outline::addCurCell();
     *
     * @brief Adds current cell to the Outline.
     *
     *        Adds current cell to the Outline.
     */
    void addCurCell();

    /**
     * @fn void Outline::sortCells();
     *
     * @brief Sort cells in the Outline.
     *
     *        Sort cells in the Outline.
     */
    void sortCells();

    /**
     * @fn void Outline::renderScanline(int ey, int x1, int y1, int x2, int y2);
     *
     * @brief Render the scanline in the special case where the line is on the same scanline.
     *
     *        Render the scanline in the special case where the line is on the same scanline,
     *        though it might not be 100% horizontal as the fraction of the y endpoints might
     *        differ making the line tilt ever so slightly.
     *
     * @param ey The entire part of the from/to y coordinate (same for both from y and to y as it
     *           is a horizontal scanline).
     * @param x1 The from x coordinate in poly format.
     * @param y1 The from y coordinate fraction part in poly format.
     * @param x2 The to x coordinate in poly format.
     * @param y2 The to y coordinate fraction part in poly format.
     */
    void renderScanline(int ey, int x1, int y1, int x2, int y2);

    /**
     * @fn void Outline::renderLine(int x1, int y1, int x2, int y2);
     *
     * @brief Render the line.
     *
     *        Render the line. This is the general case which handles all cases regardless of
     *        the position of the from coordinate and the to coordinate.
     *
     * @param x1 The from x coordinate in poly format.
     * @param y1 The from y coordinate in poly format.
     * @param x2 The to x coordinate in poly format.
     * @param y2 The to y coordinate in poly format.
     */
    void renderLine(int x1, int y1, int x2, int y2);

    /**
     * @fn static void Outline::qsortCells(Cell* const start, unsigned num);
     *
     * @brief Quick sort Outline cells.
     *
     *        Quick sort Outline cells.
     *
     * @param [in] start The first Cell object in the Cell array to sort.
     * @param num        Number of Cell objects to sort.
     */
    static void qsortCells(Cell* const start, unsigned num);

private:
    unsigned     maxCells;
    unsigned     numCells;
    Cell*        cells;
    Cell*        curCellPtr;
    Cell         curCell;
    int          curX;
    int          curY;
    int          closeX;
    int          closeY;
    int          minX;
    int          minY;
    int          maxX;
    int          maxY;
    unsigned int flags;
    int          maxRenderY;
    bool         outlineTooComplex;
#ifdef SIMULATOR
    unsigned     numCellsMissing;
    unsigned     numCellsUsed;
#endif
};
} // namespace touchgfx

#endif // OUTLINE_HPP
