#include <gui/homeautomation_screen/Graph.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <math.h>

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::setup(touchgfx::AbstractPainter& painter, touchgfx::AbstractPainter& bgPainter)
{
    graphLine.setPosition(0, 0, getWidth(), getHeight());
    graphLine.setBuffer(graphBuffer, NUMBER_OF_POINTS);
    graphLine.setLineWidth(4);
    graphLine.setXFactor(2);
    graphLine.setup(painter, bgPainter);

    add(graphLine);
    add(graphLine.outer);
    add(graphLine.inner);
}
