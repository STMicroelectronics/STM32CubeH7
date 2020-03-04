

#include <gui/settings_screen/DatePicker.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

DatePicker::DatePicker() :
    onSelectedElementChanged(this, &DatePicker::selectedElementChangedHandler)
{
    background.setBitmap(Bitmap(BITMAP_DATE_PICKER_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    days.setXY(248, 23);
    add(days);

    months.setXY(22, 23);
    months.setElementSelectedCallback(onSelectedElementChanged);
    add(months);

    years.setXY(374, 23);
    years.setElementSelectedCallback(onSelectedElementChanged);
    add(years);

    shadowTop.setBitmap(Bitmap(BITMAP_DATE_PICKER_SHADOW_TOP_ID));
    shadowTop.setXY(8, 15);
    add(shadowTop);

    shadowBottom.setBitmap(Bitmap(BITMAP_DATE_PICKER_SHADOW_BOTTOM_ID));
    shadowBottom.setXY(8, 166);
    add(shadowBottom);

    setWidth(background.getWidth());
    setHeight(background.getHeight());

    // Setup the year wheel. The overlay image used covers all three wheels, so
    // a negative x value is used to place the overlay correctly inside the wheel
    years.setup(BITMAP_DATE_PICKER_BAR_ID, 204, 172, 56, 2, -366, 59);
    for (int i = START_YEAR; i < START_YEAR + 12; i++)
    {
        Unicode::UnicodeChar temp[6];
        Unicode::snprintf(temp, 6, "%d", i);
        years.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);
    }
    years.setSelectedItem(1, 20);

    // Setup the day wheel. The overlay image used covers all three wheels, so
    // a negative x value is used to place the overlay correctly inside the wheel
    days.setup(BITMAP_DATE_PICKER_BAR_ID, 103, 172, 56, 2, -240, 59);
    for (int i = 1; i < 32; i++)
    {
        Unicode::UnicodeChar temp[3];
        Unicode::snprintf(temp, 3, "%d", i);
        days.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);
    }
    days.setSelectedItem(1, 20);

    // Setup the month wheel. The overlay image used covers all three wheels, so
    // a negative x value is used to place the overlay correctly inside the wheel
    months.setup(BITMAP_DATE_PICKER_BAR_ID, 203, 172, 56, 2, -14, 59);
    Unicode::UnicodeChar temp[5];

    Unicode::snprintf(temp, 5, "JAN");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "FEB");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "MAR");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "APR");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "MAY");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "JUN");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "JUL");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "AUG");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "SEP");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "OCT");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "NOV");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    Unicode::snprintf(temp, 5, "DEC");
    months.addText(T_SETTINGS_DATE_PICKER_TEXT, T_SETTINGS_DATE_PICKER_TEXT_SELECTED, temp);

    months.setSelectedItem(1, 20);
}

DatePicker::~DatePicker()
{

}

void DatePicker::setDay(int index, int duration, EasingEquation equation)
{
    days.setSelectedItem(index, duration, equation);
}

void DatePicker::setMonth(int index, int duration, EasingEquation equation)
{
    months.setSelectedItem(index, duration, equation);
}

void DatePicker::setYear(int index, int duration, EasingEquation equation)
{
    years.setSelectedItem(index, duration, equation);
}

int DatePicker::getDay()
{
    return days.getSelectedItem();
}

int DatePicker::getMonth()
{
    return months.getSelectedItem();
}

int DatePicker::getYear()
{
    return years.getSelectedItem();
}

void DatePicker::reset()
{
    days.reset();
    months.reset();
    years.reset();
}

void DatePicker::selectedElementChangedHandler(const WheelSelector& wheel, const int& index)
{
    // Adjust the number of days in the selected month/year
    int numberOfDaysInMonth = getNumberOfDays(months.getSelectedItem() + 1, START_YEAR + years.getSelectedItem());

    if (numberOfDaysInMonth == 28)
    {
        days.hideBottomElements(3);
        if (days.getSelectedItem() > 27)
        {
            days.setSelectedItem(27, 20);
        }
    }
    else if (numberOfDaysInMonth == 29)
    {
        days.hideBottomElements(2);
        if (days.getSelectedItem() > 28)
        {
            days.setSelectedItem(28, 20);
        }
    }
    else if (numberOfDaysInMonth == 30)
    {
        days.hideBottomElements(1);
        if (days.getSelectedItem() > 29)
        {
            days.setSelectedItem(29, 20);
        }
    }
    else
    {
        days.hideBottomElements(0);
    }
}

uint16_t DatePicker::getNumberOfDays(uint16_t month, uint16_t year)
{
    uint16_t result = 0;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        result = 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        result = 30;
    }
    else if (month == 2)
    {
        result = 28;
        if (year % 400 == 0)
        {
            result++;
        }
        else if (year % 100 == 0)
        {
        }
        else if (year % 4 == 0)
        {
            result++;
        }
    }
    return result;
}

