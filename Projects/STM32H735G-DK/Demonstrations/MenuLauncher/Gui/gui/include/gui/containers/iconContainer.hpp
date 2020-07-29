#ifndef ICONCONTAINER_HPP
#define ICONCONTAINER_HPP

#include <gui_generated/containers/iconContainerBase.hpp>

class iconContainer : public iconContainerBase
{
public:
    iconContainer();
    virtual ~iconContainer() {}

    virtual void initialize();

    void setScale(float scale);
    float getScale();

    void setSideAlpha(uint8_t alpha);
    void setBitmap(int index);
    int getIndex();
protected:
    int index;
};

#endif // ICONCONTAINER_HPP
