#ifndef TRACKSELECTOR_HPP
#define TRACKSELECTOR_HPP

#include <gui_generated/containers/trackSelectorBase.hpp>
#include <gui/containers/thumbnailContainer.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/Box.hpp>

class trackSelector : public trackSelectorBase
{
public:
    trackSelector();
    virtual ~trackSelector() {}

    virtual void initialize();

    void setTrackListSize(int size);
    void refreshTrackList();

    void setRepeatButtonState(bool on);
    void setShuffleButtonState(bool on);
    
    void setupThumbnail(int id, bool used, Bitmap image = Bitmap(BITMAP_VIDEO_CLIP_OVERLAY_ID));
    void setSelectedThumbnail(int selected);

    void playPauseSelectedVideo(bool play);
    //virtual void thumbnailPlayPausePressed(bool value);

protected:

    void videoListItemSelectedHandler(int selectedItem);
    Callback<trackSelector, int> videoListItemSelectedCallback;
    
    void videoListPLayPausePressedHandler(bool play);
    Callback<trackSelector, bool>videoListPLayPausePressedCallback;
     
    thumbnailContainer thumbNails[20];
    int currentSelectedVideo;
};

#endif // TRACKSELECTOR_HPP
