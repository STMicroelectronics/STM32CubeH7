#include <gui/home_automation_screen/StatisticsContainer.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <math.h>
#include <gui/common/Utils.hpp>
#include <touchgfx/EasingEquations.hpp>

int StatisticsContainer::yScale = 5;

StatisticsContainer::StatisticsContainer()
    : numValues(0), statState(STAT_STATE_NONE)
{
    setTouchable(true);

    setWidth(HAL::DISPLAY_WIDTH);
    setHeight(HAL::DISPLAY_HEIGHT - 80);

    upper.setBitmap(Bitmap(BITMAP_STATISTICS_UPPER_ID));
    upper.setXY(0, 0);
    lower.setBitmap(Bitmap(BITMAP_STATISTICS_LOWER_ID));
    lower.setXY(0, upper.getRect().bottom());
    lower.setHeight(Bitmap(lower.getBitmap()).getHeight());
    lower.setWidth(getWidth());

    add(upper);
    add(lower);

    verticalBar.setPosition(80, 1, 2, upper.getHeight() - 2);
    verticalBar.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    verticalBar.setAlpha(51); // 20%
    add(verticalBar);

    painter[0].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_GREEN_LINE_ID));
    painter[1].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_BLUE_LINE_ID));
    painter[2].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_RED_LINE_ID));
    bgPainter.setColor(Color::getColorFrom24BitRGB(7, 35, 45));
    circlePainter[0].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_GREEN_ID));
    circlePainter[1].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_BLUE_ID));
    circlePainter[2].setBitmap(Bitmap(BITMAP_STATISTICS_GRADIENT_RED_ID));
    icon[0].setBitmap(Bitmap(BITMAP_LIGHTS_ICON_32_ID));
    icon[1].setBitmap(Bitmap(BITMAP_BLINDS_ICON_32_ID));
    icon[2].setBitmap(Bitmap(BITMAP_LOCK_ICON_32_ID));
    for (int i = 0; i < 3; i++)
    {
        graph[i].setPosition(80, 23, 690 - 160, 194);
        graph[i].setup(painter[i], bgPainter);
        graph[i].setRange(-10 * yScale, 110 * yScale);
        add(graph[i]);

        circle[i].setPosition(52 + 214 * i, 24 + upper.getHeight(), 110, 110);
        circle[i].setPainter(circlePainter[i]);
        circle[i].setLineWidth(10);
        circle[i].setCenter(55, 55);
        circle[i].setRadius(50);
        circle[i].setCapPrecision(10);
        circle[i].setArc(6, 6);
        add(circle[i]);

        icon[i].setXY(12 + 214 * i, 109 + upper.getHeight());
        add(icon[i]);

        buffer[i][3] = 0;
        Unicode::snprintf(buffer[i], 4, "%d", 123);
        pctValue[i].setTypedText(TypedText(T_PERCENT));
        pctValue[i].setWildcard(buffer[i]);
        pctValue[i].setPosition(76 + 214 * i, 51 + upper.getHeight(), 60, pctValue[i].getTextHeight());
        pctValue[i].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        add(pctValue[i]);
        pctSign[i].setTypedText(TypedText(T_PERCENT_SIGN));
        pctSign[i].setPosition(96 + 214 * i, 86 + upper.getHeight(), 20, pctSign[i].getTextHeight());
        pctSign[i].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        add(pctSign[i]);
    }

    update(graph[0].getX());

    Application::getInstance()->registerTimerWidget(this);
}

StatisticsContainer::~StatisticsContainer()
{
}

void StatisticsContainer::setup()
{
    statState = STAT_STATE_INIT;
    numValues = 0;
    for (int i = 0; i < 3; i++)
    {
        graph[i].clear();
        for (int j = 0; j < 5; j++)
        {
            randFreq[i][j] = Utils::randomNumberBetween(100, 500) / 10.f;
            randAdd[i][j] = Utils::randomNumberBetween(0, 1000) / 10.f;
        }
    }
}

void StatisticsContainer::handleTickEvent()
{
#ifdef SIMULATOR
    (void)Utils::randomNumberBetween(0, 1);
#endif
    switch (statState)
    {
    case STAT_STATE_INIT:
        if (numValues < 265)
        {
            // Add "random" data
            int newValues = 4;
            for (int v = 0; v < newValues; v++)
            {
                for (int i = 0; i < 3; i++)
                {
                    int x = numValues + v;
                    float y = 0.0f;
                    for (int j = 0; j < 5; j++)
                    {
                        y += sinf(x / randFreq[i][j] + randAdd[i][j]);
                    }
                    y = (y + 5) / 10 * 100;
                    graph[i].addValue(x, (int)(y * yScale + 0.5f));
                }
            }
            Rect updatedRect(graph[0].getX() + numValues * 2 - 2, 0, 3 + numValues * 2, graph[0].getHeight());
            invalidateRect(updatedRect);
            update(graph[0].getX() + numValues * 2);
            numValues += newValues;
        }
        else
        {
            statState = STAT_STATE_SHOW_OFF;
            tick = 0;
        }
        break;
    case STAT_STATE_SHOW_OFF:
        {
            tick++;
            int steps = 80;
            int delta = EasingEquations::backEaseOut(tick, 0, (graph[0].getWidth() * 2) / 3, steps);
            update(graph[0].getRect().right() - delta);
            if (tick > steps)
            {
                statState = STAT_STATE_READY;
            }
        }
        break;
    default:
        break;
    }
}

void StatisticsContainer::handleClickEvent(const ClickEvent& evt)
{
    if (statState == STAT_STATE_READY && evt.getY() <= upper.getRect().bottom())
    {
        update(evt.getX());
    }
}

void StatisticsContainer::handleDragEvent(const DragEvent& evt)
{
    if (statState == STAT_STATE_READY && evt.getNewY() <= upper.getRect().bottom())
    {
        update(evt.getNewX());
    }
}

void StatisticsContainer::update(int x)
{
    x &= ~1;

    x = MAX(x, graph[0].getX());
    x = MIN(x, graph[0].getRect().right());
    verticalBar.invalidate();
    verticalBar.setX(x - 1);
    verticalBar.invalidate();

    x -= graph[0].getX();
    for (int i = 0; i < 3; i++)
    {
        graph[i].setSelectedX(x);

        int y = graph[i].getValue(x) / yScale;
        if (y >= 0 && y <= 100)
        {
            circle[i].updateArcEnd(348 * y / 100 + 6);
            Unicode::snprintf(buffer[i], 4, "%d", y);
            pctValue[i].invalidate();
        }
    }
}
