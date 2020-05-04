#include <gui/home_automation_screen/GraphLine.hpp>

using namespace touchgfx;

GraphLine::GraphLine() :
    CanvasWidget(), top(-1), bottom(-1), xFactor(1), yAt(0), maxPoints(0), lineWidth(0)
{
    clearGraph();
}

GraphLine::~GraphLine()
{
}

void GraphLine::setBuffer(int16_t* pointsBuffer, int numPointsInBuffer)
{
    yAt = pointsBuffer;
    maxPoints = numPointsInBuffer;
    numPoints = 0;
}

void GraphLine::setRange(int newBottom, int newTop)
{
    if (top != newTop || bottom != newBottom)
    {
        top = newTop;
        bottom = newBottom;

        graphHeight = CWRUtil::toQ5(newBottom - newTop);
    }
}

void GraphLine::setHeight(int16_t height)
{
    CanvasWidget::setHeight(height);
    widgetHeight = CWRUtil::toQ5(getHeight());
}

void GraphLine::clearGraph()
{
    numPoints = 0;
}

bool GraphLine::addValue(int x, int y)
{
    if (x < 0 || x > numPoints || x >= maxPoints)
    {
        return false;
    }
    if (numPoints == 0 || x == numPoints)
    {
        // Point added at end of graph line
        yAt[numPoints] = y;
        numPoints++;
    }
    else
    {
        yAt[x] = y;
    }
    return true;
}

bool GraphLine::drawCanvasWidget(const Rect& invalidatedArea) const
{
    if (numPoints < 2)
    {
        // A graph line with a single (or not even a single) point is invisible
        return true;
    }

    int extra = (lineWidth.to<int>() + 1) / 2;
    int firstIndex = ((invalidatedArea.x - extra) / xFactor);
    firstIndex = MAX(0, firstIndex);
    int lastIndex = (((invalidatedArea.right() + extra) + (xFactor - 1)) / xFactor);
    lastIndex = MIN(numPoints - 1, lastIndex);

    Canvas canvas(this, invalidatedArea);
    CWRUtil::Q5 dx, dy, d;

    dy = (widgetY(yAt[firstIndex + 1]) - widgetY(yAt[firstIndex]));
    dx = (widgetX(xAt(firstIndex + 1)) - widgetX(xAt(firstIndex)));
    d = CWRUtil::sqrtQ10(dy * dy + dx * dx);
    dy = (lineWidth * dy / d) / 2;
    dx = (lineWidth * dx / d) / 2;

    canvas.moveTo(widgetX(xAt(firstIndex)) + dy, widgetY(yAt[firstIndex]) - dx);
    canvas.lineTo(widgetX(xAt(firstIndex + 1)) + dy, widgetY(yAt[firstIndex + 1]) - dx);
    for (int index = firstIndex + 1; index < lastIndex; index++)
    {
        dy = (widgetY(yAt[index + 1]) - widgetY(yAt[index]));
        dx = (widgetX(xAt(index + 1)) - widgetX(xAt(index)));
        d = CWRUtil::sqrtQ10(dy * dy + dx * dx);
        dy = (lineWidth * dy / d) / 2;
        dx = (lineWidth * dx / d) / 2;
        canvas.lineTo(widgetX(xAt(index)) + dy, widgetY(yAt[index]) - dx);
        canvas.lineTo(widgetX(xAt(index + 1)) + dy, widgetY(yAt[index + 1]) - dx);
    }
    for (int index = lastIndex - 1; index >= firstIndex; index--)
    {
        dy = (widgetY(yAt[index + 1]) - widgetY(yAt[index]));
        dx = (widgetX(xAt(index + 1)) - widgetX(xAt(index)));
        d = CWRUtil::sqrtQ10(dy * dy + dx * dx);
        dy = (lineWidth * dy / d) / 2;
        dx = (lineWidth * dx / d) / 2;
        canvas.lineTo(widgetX(xAt(index + 1)) - dy, widgetY(yAt[index + 1]) + dx);
        canvas.lineTo(widgetX(xAt(index)) - dy, widgetY(yAt[index]) + dx);
    }
    return canvas.render();
}

int GraphLine::getValue(int x)
{
    x /= xFactor;
    if (x < 0 || x > numPoints)
    {
        return -1;
    }
    return yAt[x];
}
