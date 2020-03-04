

#include <gui/controls_screen/CircularValueIndicator.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

CircularValueIndicator::CircularValueIndicator() 
{
    background.setBitmap(Bitmap(BITMAP_VALUE_INDICATOR_WHEEL_ID));
    background.setXY(21, 21);
    add(background);

    setWidth(Bitmap(BITMAP_VALUE_INDICATOR_CIRCLE_ID).getWidth());
    setHeight(Bitmap(BITMAP_VALUE_INDICATOR_CIRCLE_ID).getHeight());


    arrow.setBitmap(Bitmap(BITMAP_VALUE_INDICATOR_ARROW_ID));

    arrow.setXY(background.getX() + 36, background.getY() + 36);
    arrow.setWidth(background.getWidth());
    arrow.setHeight(background.getHeight());
    arrow.setBitmapPosition(84, 8);
    arrow.setCameraDistance(300.0f);
    arrow.setOrigo(100, 102, arrow.getCameraDistance());
    arrow.setCamera(arrow.getOrigoX(), arrow.getOrigoY());
    arrow.setRenderingAlgorithm(TextureMapper::BILINEAR_INTERPOLATION);
    add(arrow);

    myBitmapPainterCircle.setBitmap(Bitmap(BITMAP_VALUE_INDICATOR_CIRCLE_ID));
    circle.setPosition(0, 0, getWidth(), getHeight());
    circle.setCenter(background.getX() + 138, background.getY() + 136);
    circle.setRadius(132);
    circle.setLineWidth(20);
    circle.setPainter(myBitmapPainterCircle);
    circle.setArc(MIN_DEGREE-1, MIN_DEGREE);

    JunctionTempText.setWildcard(JunctionTempTextBuffer);
    JunctionTempText.setTypedText(TypedText(T_CONTROLS_POT_READOUT));
    JunctionTempText.setPosition((getWidth()/2) - 80, 122, 100, 60);
    JunctionTempText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    updateJunctionTempText(0);
    add(JunctionTempText);

    JunctionTempPercentageText.setTypedText(TypedText(T_CONTROLS_POT_PERCENTAGE));
    JunctionTempPercentageText.setXY(JunctionTempText.getRect().right() + 3, JunctionTempText.getY() + 22);
    JunctionTempPercentageText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(JunctionTempPercentageText);

    add(circle);

    arrow.updateAngles(0.f, 0.f, 0.0f);

    value = 0;
}

CircularValueIndicator::~CircularValueIndicator()
{
}

void CircularValueIndicator::setValue(int temp)
{
    if (temp > 100)
    {
        temp = 100;
    }

    updateJunctionTempText(temp);

    int arcEnd = (((MAX_DEGREE - MIN_DEGREE) * temp) / 100) + MIN_DEGREE;
    circle.updateArcEnd(arcEnd); 
    circle.invalidate();

    float a = arcEnd * (3.1415f/180);

    arrow.updateAngles(arrow.getXAngle(), arrow.getYAngle(), a);
    arrow.invalidate();

    value = temp;
}

void CircularValueIndicator::updateJunctionTempText(int value)
{
    Unicode::snprintf(JunctionTempTextBuffer, 10, "%d", value);
    JunctionTempText.invalidate();
}
