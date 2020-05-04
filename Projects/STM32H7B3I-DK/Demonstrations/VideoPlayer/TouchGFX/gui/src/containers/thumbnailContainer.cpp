#include <gui/containers/thumbnailContainer.hpp>
#include <BitmapDatabase.hpp>

thumbnailContainer::thumbnailContainer()
{

}

void thumbnailContainer::initialize()
{
    thumbnailContainerBase::initialize();
}

void thumbnailContainer::setSelected(bool selected)
{
    playPauseOverlay.setVisible(selected);
    selectItemOverlay.setVisible(!selected);
}

void thumbnailContainer::setBitmap(Bitmap image)
{
    thumbnailBitmap.setBitmap(image);
}

void thumbnailContainer::setId(int id)
{
    this->id = id;
}

void thumbnailContainer::itemSelectedPressed(const touchgfx::AbstractButton& value)
{
    emitItemSelectedCallback(this->id);
}

void thumbnailContainer::setPlayPause(bool play)
{
    playPauseOverlay.forceState(play);
}
