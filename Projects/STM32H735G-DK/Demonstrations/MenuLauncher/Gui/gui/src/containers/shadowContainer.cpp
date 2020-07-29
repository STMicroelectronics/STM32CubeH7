#include <gui/containers/shadowContainer.hpp>

shadowContainer::shadowContainer()
{

}

void shadowContainer::initialize()
{
    shadowContainerBase::initialize();
}

void shadowContainer::setScale(float scale)
{
    //shadowBig.setScale(scale);
}

void shadowContainer::setAlpha(uint8_t alpha)
{
    shadowBig.setAlpha(alpha);
}
