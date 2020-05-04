#ifndef GRAPHLINE_HPP
#define GRAPHLINE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

/**
 * @class GraphLine GraphLine.hpp gui/common/GraphLine.hpp
 *
 * @brief Simple widget capable of drawing a graph line. The graph line consists of a number of
 *        points with different x values. Points on the graph can be added and removed and the
 *        coordinate system of the graph can easily be setup. The graph is limited to integer
 *        values for x and y and the values should not exceed 1000.
 *
 * @see CanvasWidget
 */
class GraphLine : public CanvasWidget
{
public:
    /**
     * @fn GraphLine::GraphLine()
     *
     * @brief Constructs a new NewGraph.
     *
     *        Constructs a new NewGraph.
     */
    GraphLine();

    /**
     * @fn virtual GraphLine::~GraphLine()
     *
     * @brief Virtual Destructor.
     *
     *        Virtual Destructor. Not used.
     */
    virtual ~GraphLine();

    /**
     * @fn CWRUtil::Q5 GraphLine::widgetX(int x) const;
     *
     * @brief Calculate Widget x coordinate.
     *
     *        Calculate widget x coordinate from a graph x coordinate.
     *
     * @param x The x coordinate.
     *
     * @return The x coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetX(int x) const
    {
        return CWRUtil::toQ5(x);
    }

    /**
     * @fn CWRUtil::Q5 GraphLine::widgetY(int y) const;
     *
     * @brief Calculate Widget y coordinate.
     *
     *        Calculate widget y coordinate from a graph y coordinate.
     *
     * @param y The y coordinate.
     *
     * @return The y coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetY(int y) const
    {
        return CWRUtil::toQ5(y - top) * widgetHeight / graphHeight;
    }

    /**
     * @fn void GraphLine::setBuffer(GraphPoint *pointsBuffer, int numPointsInBuffer)
     *
     * @brief Sets a buffer to be used to store points on the graph line.
     *
     *        Sets a buffer to be used to store points on the graph line.
     *
     * @param [in] pointsBuffer If non-null, buffer for points data.
     * @param numPointsInBuffer Number of points in buffers.
     */
    void setBuffer(int16_t* pointsBuffer, int numPointsInBuffer);

    /**
     * @fn void GraphLine::setRange(int newBottom, int newTop);
     *
     * @brief Sets the range of the graph.
     *
     *        Sets the range of the graph.
     *
     * @param newBottom The y value at the bottom of the graph area.
     * @param newTop    The y value at the top of the graph area.
     */
    void setRange(int newBottom, int newTop);

    /**
     * @fn int GraphLine::getRangeBottom()
     *
     * @brief Gets the y value at the bottom of the graph area.
     *
     * @return The y value at the bottom of the graph area.
     */
    int getRangeBottom()
    {
        return bottom;
    }

    /**
     * @fn int GraphLine::getRangeTop()
     *
     * @brief Gets the y value at the top of the graph area.
     *
     * @return The y value at the top of the graph area.
     */
    int getRangeTop()
    {
        return top;
    }

    /**
     * @fn virtual void GraphLine::setHeight(int16_t height)
     *
     * @brief Sets the height of this drawable.
     *
     *        Sets the height of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param height The new height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn template <class T> void GraphLine::setLineWidth(T width)
     *
     * @brief Sets the width for this graph line in pixels.
     *
     *        Sets the width for this graph line in pixels.
     *
     * @note The graph line is invalidated.
     *
     * @tparam T Generic type parameter.
     * @param width The width of the line measured in pixels.
     */
    template <class T>
    void setLineWidth(T width)
    {
        CWRUtil::Q5 newLineWidth = CWRUtil::toQ5(width);
        if (lineWidth == newLineWidth)
        {
            return;
        }

        lineWidth = newLineWidth;
    }

    /**
     * @fn template <class T> T GraphLine::getLineWidth() const
     *
     * @brief Gets line width in pixels.
     *
     *        Gets line width in pixels.
     *
     * @tparam T Generic type parameter.
     *
     * @return The line width in pixels.
     */
    template <class T>
    T getLineWidth() const
    {
        return int(lineWidth) / T(Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn virtual void NewGraph::clearGraph()
     *
     * @brief Clears the graph.
     *
     * @note The graph line is not invalidated.
     */
    void clearGraph();

    /**
     * @fn virtual bool GraphLine::addValue(int x, int y)
     *
     * @brief Adds a value to the graph.
     *
     *        Adds a value to the graph.
     *
     * @note The updated graph line is invalidated.
     *
     * @param x The value.
     * @param y The y coordinate.
     *
     * @return true if it succeeds, false if it fails.
     */
    bool addValue(int x, int y);

    int getValue(int x);

    /**
     * @fn virtual bool GraphLine::drawCanvasWidget(const Rect& invalidatedArea) const
     *
     * @brief Draw the graph line.
     *
     *        Draw the graph line. Care is taken not to spend time drawing graphs lines
     *        that are outside the invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if there was insufficient memory to draw the line.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    void setXFactor(int16_t factor)
    {
        xFactor = MAX(1, factor);
    }

    /**
     * @fn void GraphLine::setup(AbstractPainter& painter, AbstractPainter& background)
     *
     * @brief Setup.
     *
     *        Setup. Could have been put in overloaded function setPainter, but in TouchGFX
     *        4.5.0 setPainter is not virtual in CanvasWidget.
     *
     * @param [in,out] painter    The painter.
     * @param [in,out] background The background painter.
     */
    void setup(AbstractPainter& painter, AbstractPainter& background)
    {
        outer.setPainter(background);
        inner.setPainter(painter);
        setPainter(painter);

        outer.setPosition(0, 0, getWidth(), getHeight());
        outer.setRadius(6);
        outer.setLineWidth(0);
        outer.setArc(0, 360);
        outer.setCenter(-100, -100);
        inner.setPosition(0, 0, getWidth(), getHeight());
        inner.setRadius(4);
        inner.setLineWidth(0);
        inner.setArc(0, 360);
        inner.setCenter(-100, -100);
    }

    void setSelectedX(int16_t x)
    {
        x /= xFactor;
        inner.invalidate();
        outer.invalidate();
        if (x >= 0 && x < numPoints)
        {
            inner.setVisible(true);
            outer.setVisible(true);
            inner.setCenter(widgetX(xAt(x)).to<float>(), widgetY(yAt[x]).to<float>());
            outer.setCenter(widgetX(xAt(x)).to<float>(), widgetY(yAt[x]).to<float>());
            inner.invalidate();
            outer.invalidate();
        }
        else
        {
            inner.setVisible(false);
            outer.setVisible(false);
        }
    }

protected:
    int xAt(int index) const
    {
        return index * xFactor;
    }
    int         top, bottom;  ///< Limits of the values of the graph line area
    int16_t     xFactor;      ///< The x scaling factor
    int16_t*    yAt;          ///< Pointer to buffer where the coordinates are stored
    int         maxPoints;    ///< Maximum number of points possible on graph line
    int         numPoints;    ///< Number of         points currently in use
    CWRUtil::Q5 lineWidth;    ///< Line with (0=fill below graph line)
    CWRUtil::Q5 widgetHeight; ///< Height of the widget as Q5
    CWRUtil::Q5 graphHeight;  ///< Height of the graph range (top to bottom) as Q5
    int16_t     selectedY;
public:
    Circle      outer;
    Circle      inner;
};

#endif // GRAPHLINE_HPP
