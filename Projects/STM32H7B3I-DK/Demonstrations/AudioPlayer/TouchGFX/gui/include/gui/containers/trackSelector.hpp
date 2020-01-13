#ifndef TRACKSELECTOR_HPP
#define TRACKSELECTOR_HPP

#include <gui_generated/containers/trackSelectorBase.hpp>

class trackSelector : public trackSelectorBase
{
public:
    trackSelector();
    virtual ~trackSelector() {}

    virtual void initialize();

    virtual void trackListUpdateItem(track& item, int16_t itemIndex);

    void setUpdatePlaylist(GenericCallback< track&, int >& callback);
    void setSelectedItemCallback(GenericCallback<int>& callback);

    void setTrackListSize(int size);
    void refreshTrackList();

    void setRepeatButtonState(bool on);
    void setShuffleButtonState(bool on);

protected:

    GenericCallback< track&, int >* action;
    GenericCallback<int> *itemSelectedAction;

    void trackListItemSelectedHandler(int16_t selectedItem);
    Callback<trackSelector, int16_t> trackListItemSelectedCallback;
    
    void trackListPLayPausePressed(bool play);
    Callback<trackSelector, bool>trackListPLayPausePressedCallback;
     
    
};

#endif // TRACKSELECTOR_HPP
