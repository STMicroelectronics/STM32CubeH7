

#ifndef VIDEO_PLAYER_FOLDERWIDGET_HPP
#define VIDEO_PLAYER_FOLDERWIDGET_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

#include <gui/common/Playlist.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/video_player_screen/FileInput.hpp>

using namespace touchgfx;

class PlaylistManager
{
public:
    virtual bool addFile(uint32_t index) = 0;
    virtual void removeFile(uint32_t index) = 0;
    virtual void folderSelected(uint32_t index) = 0;
    virtual void upPressed() = 0;
    virtual void donePressed() = 0;
    virtual ~PlaylistManager() {};
};

class FolderListElement : public Container
{
public:
    FolderListElement();
    void configure(bool isDirectory);
    void setSelected(bool selected);
    virtual void handleClickEvent(const ClickEvent& event);

    TextAreaWithOneWildcard name;
    Unicode::UnicodeChar nameBuf[FILENAME_LENGTH];
    Image background; // visible when selected
    Image icon;
    Image button;
    uint32_t index;
    bool isDirectory;
    bool isIncluded;
    bool isSelected;
    bool clickCancelled;

    PlaylistManager* playlistManager;
    static GenericCallback< const uint32_t >* itemPressedAction;
};

class FolderWidget : public Container
{
public:
    FolderWidget(PlaylistManager&);
    void setPlaylist(PlaylistElement* playlist, uint32_t length);
    void setupDirlist(const char* const foldername, fileinput::DirEntry* dirlist, uint32_t length);
private:
    Image background;
    Image shadow;
    Image titlebar;
    TextAreaWithOneWildcard titleText;
    Unicode::UnicodeChar folderNameBuf[FOLDERNAME_LENGTH];    

    ScrollableContainer scrollCnt;
    ListLayout list;
    FolderListElement listElements[DIRLIST_LENGTH];

    Image bottombar;
    Button backButton;
    Button okButton;
    uint32_t selected;
    
    PlaylistManager& playlistManager;

    PlaylistElement* playlist;
    uint32_t playlistLength;
    bool isOnPlaylist(const char* const foldername, fileinput::DirEntry& direntry);

    Callback<FolderWidget, const AbstractButton&> buttonCallback;
    void onButtonPressed(const AbstractButton& button);
    Callback<FolderWidget, const uint32_t> itemCallback;
    void onItemPressed(const uint32_t index);
};

#endif

