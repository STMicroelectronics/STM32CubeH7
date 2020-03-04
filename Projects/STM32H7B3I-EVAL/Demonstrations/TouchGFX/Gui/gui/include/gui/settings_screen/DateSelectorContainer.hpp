#ifndef DATE_SELECTOR_CONTAINER
#define DATE_SELECTOR_CONTAINER

#include <touchgfx/containers/Container.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/settings_screen/DatePicker.hpp>

using namespace touchgfx;

class DateSelectorContainer : public Container
{
public:
    DateSelectorContainer();
    virtual ~DateSelectorContainer();

    void setDate(uint16_t year, uint8_t month, uint8_t day);

    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();

    int getYearOffset()
    {
        return DatePicker::START_YEAR;
    }
private:
    DatePicker datePicker;
};

#endif /* DATE_SELECTOR_CONTAINER */
