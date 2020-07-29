#ifndef SHADOWCONTAINER_HPP
#define SHADOWCONTAINER_HPP

#include <gui_generated/containers/shadowContainerBase.hpp>

class shadowContainer : public shadowContainerBase
{
public:
    shadowContainer();
    virtual ~shadowContainer() {}

    virtual void initialize();

    void setScale(float scale);
    void setAlpha(uint8_t alpha);
protected:
};

#endif // SHADOWCONTAINER_HPP
