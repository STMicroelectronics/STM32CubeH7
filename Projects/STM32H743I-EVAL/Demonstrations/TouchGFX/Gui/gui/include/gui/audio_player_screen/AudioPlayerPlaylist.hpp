#ifndef AUDIO_PLAYER_PLAYLIST_HPP
#define AUDIO_PLAYER_PLAYLIST_HPP

#include <gui/common/CommonDefinitions.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Button.hpp>

using namespace touchgfx;

class AudioPlayerPlaylistElement : public Container
{
public:
    AudioPlayerPlaylistElement();
    virtual ~AudioPlayerPlaylistElement();
    void setSelectedCallback(GenericCallback<int>& callback);
    void setRemoveCallback(GenericCallback<AudioPlayerPlaylistElement&>& callback);
    void setPlaying(bool play);
    void setLineVisibility(bool visible);
    void setCover(Bitmap& bitmapId);
    void set(int index, BitmapId cover, char* artist, char* song);

    virtual void handleClickEvent(const ClickEvent& evt);
    
private:
    GenericCallback<int>* selectedCallback;
    GenericCallback<AudioPlayerPlaylistElement&>* removeCallback;

    Image cover;
    Image coverShadow;
    Image coverOverlay;
    Button remove;
    Image line;
    TextAreaWithOneWildcard numberText;
    Unicode::UnicodeChar numberTextBuffer[3];
    TextAreaWithOneWildcard songText;
    Unicode::UnicodeChar songTextBuffer[50];
    TextAreaWithOneWildcard artistText;
    Unicode::UnicodeChar artistTextBuffer[50];

    int playlistIndex;
    bool pressed;
};

class AudioPlayerPlaylist : public Container
{
public:
    AudioPlayerPlaylist();

    void setSelectedCallback(GenericCallback<int>& callback);
    void clear();
    void addSong(BitmapId cover, char* artist, char* song);
    void setPlaying(int index);
    
private:
    Box background;
    Image shadow;
    ScrollableContainer scroll;

    int numberOfElements;
    const static int maxElements = 20;
    AudioPlayerPlaylistElement elements[maxElements];

    Callback<AudioPlayerPlaylist, AudioPlayerPlaylistElement&> onRemove;
    void removeElement(AudioPlayerPlaylistElement& element);
};

#endif
