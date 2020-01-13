#ifndef THUMBNAILCONTAINER_HPP
#define THUMBNAILCONTAINER_HPP

#include <gui_generated/containers/thumbnailContainerBase.hpp>

class thumbnailContainer : public thumbnailContainerBase
{
public:
    thumbnailContainer();
    virtual ~thumbnailContainer() {}

    virtual void initialize();

    void setSelected(bool selected);
    void setBitmap(Bitmap image);
    void setId(int id);
    void setPlayPause(bool play);

    virtual void itemSelectedPressed(const touchgfx::AbstractButton& value);

protected:

    int id;
};

#endif // THUMBNAILCONTAINER_HPP
