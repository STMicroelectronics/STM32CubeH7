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
    circle.setArc(MIN_DEGREE - 1, MIN_DEGREE);

    potentiometerText.setWildcard(potentiometerTextBuffer);
    potentiometerText.setTypedText(TypedText(T_CONTROLS_POT_READOUT));
    potentiometerText.setPosition((getWidth() / 2) - 80, 122, 100, 60);
    potentiometerText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    updatePotentiometerText(0);
    add(potentiometerText);

    potentiometerPercentageText.setTypedText(TypedText(T_CONTROLS_POT_PERCENTAGE));
    potentiometerPercentageText.setXY(potentiometerText.getRect().right() + 3, potentiometerText.getY() + 22);
    potentiometerPercentageText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(potentiometerPercentageText);

    add(circle);

    arrow.updateAngles(0.f, 0.f, 0.0f);

    value = 0;
}

CircularValueIndicator::~CircularValueIndicator()
{
}

void CircularValueIndicator::setValue(int percentage)
{
    if (percentage > 100)
    {
        percentage = 100;
    }

    updatePotentiometerText(percentage);

    int arcEnd = (((MAX_DEGREE - MIN_DEGREE) * percentage) / 100) + MIN_DEGREE;
    circle.updateArcEnd(arcEnd);
    circle.invalidate();

    float a = arcEnd * (3.1415f / 180);

    arrow.updateAngles(arrow.getXAngle(), arrow.getYAngle(), a);
    arrow.invalidate();

    value = percentage;
}

void CircularValueIndicator::updatePotentiometerText(int value)
{
    Unicode::snprintf(potentiometerTextBuffer, 10, "%d", value);
    potentiometerText.invalidate();
}
