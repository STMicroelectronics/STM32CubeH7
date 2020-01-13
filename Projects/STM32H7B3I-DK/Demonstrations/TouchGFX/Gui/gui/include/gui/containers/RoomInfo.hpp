#ifndef ROOMINFO_HPP
#define ROOMINFO_HPP

#include <gui_generated/containers/RoomInfoBase.hpp>

class RoomInfo : public RoomInfoBase
{
public:
    RoomInfo();
    virtual ~RoomInfo() {}

    virtual void initialize();

    void updateValues(int16_t item);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:

};

#endif // ROOMINFO_HPP
