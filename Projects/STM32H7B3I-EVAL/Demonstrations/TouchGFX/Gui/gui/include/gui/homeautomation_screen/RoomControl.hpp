#ifndef ROOM_CONTROL_HPP
#define ROOM_CONTROL_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/Container.hpp>

using namespace touchgfx;

class RoomControl : public Container
{
public:
    static const int NUMBER_OF_ROOMS = 10;

    RoomControl();
    virtual ~RoomControl();

protected:
    Button rooms[NUMBER_OF_ROOMS];

    Callback<RoomControl, const AbstractButton&> buttonClickedCallback;
    virtual void buttonClicked(const AbstractButton& source) = 0;
};

#endif /* ROOM_CONTROL_HPP */
