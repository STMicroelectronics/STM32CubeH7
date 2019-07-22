

#include <string.h>
#include <touchgfx/Color.hpp>

#include <gui/video_player_screen/FolderWidget.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

FolderWidget::FolderWidget(PlaylistManager& manager)
    : selected(DIRLIST_LENGTH),
      playlistManager(manager),
      playlist(0),
      playlistLength(0),
      buttonCallback(this, &FolderWidget::onButtonPressed),
      itemCallback(this, &FolderWidget::onItemPressed)
{
    folderNameBuf[0] = 0;

    FolderListElement::itemPressedAction = &itemCallback;

    for (int i = 0; i < DIRLIST_LENGTH; i++)
    {
        listElements[i].playlistManager = &playlistManager;
    }

    setWidth(400 + 30);
    setHeight(480 - 70);

    background.setBitmap(Bitmap(BITMAP_GRID_BACKGROUND_ID));
    background.setWidth(400);
    background.setHeight(getHeight());

    shadow.setBitmap(Bitmap(BITMAP_BROWSE_FOLDER_SHADOW_RIGHT_ID));
    shadow.setX(400);

    titlebar.setBitmap(Bitmap(BITMAP_BROWSE_FOLDER_TITLE_BAR_ID));
    titlebar.setXY(0, 0);
    titleText.setPosition(20, 10, titlebar.getWidth() - 20, titlebar.getHeight());
    titleText.setTypedText(TypedText(T_MOVIE_BROWSE_FOLDER));
    titleText.setWildcard(folderNameBuf);
    titleText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    bottombar.setBitmap(Bitmap(BITMAP_BROWSE_FOLDER_NAVI_BAR_ID));
    bottombar.setXY(0, 330);

    backButton.setBitmaps(Bitmap(BITMAP_UP_BUTTON_ID), Bitmap(BITMAP_UP_BUTTON_PRESSED_ID));
    backButton.setXY(53, 350);
    backButton.setAction(buttonCallback);
    okButton.setBitmaps(Bitmap(BITMAP_DONE_BUTTON_ID), Bitmap(BITMAP_DONE_BUTTON_PRESSED_ID));
    okButton.setXY(53 + 120 + 53, 350);
    okButton.setAction(buttonCallback);

    add(background);
    add(shadow);
    add(titlebar);
    add(titleText);

    scrollCnt.setPosition(0, 50, 400, 280);
    scrollCnt.add(list);
    add(scrollCnt);

    add(bottombar);
    add(backButton);
    add(okButton);
}

void FolderWidget::onButtonPressed(const AbstractButton& button)
{
    if (&button == &okButton)
    {
        playlistManager.donePressed();
    }
    else if (&button == &backButton)
    {
        playlistManager.upPressed();
    }
}

void FolderWidget::onItemPressed(const uint32_t index)
{
    //deselect
    if (selected < DIRLIST_LENGTH)
    {
        listElements[selected].setSelected(false);
    }

    if (listElements[index].isDirectory)
    {
        //pass on to manager
        playlistManager.folderSelected(index);
    }
    else
    {
        //handle selection UI here
        //select new index
        selected = index;
        listElements[selected].setSelected(true);
    }
}

void FolderWidget::setPlaylist(PlaylistElement* playlist, uint32_t length)
{
    this->playlist = playlist;
    this->playlistLength = length;
}

bool FolderWidget::isOnPlaylist(const char* const foldername, fileinput::DirEntry& direntry)
{
    char fullname[FOLDERNAME_LENGTH + FILENAME_LENGTH];

    strncpy(fullname, foldername, FOLDERNAME_LENGTH);
    strcat(fullname, fileinput::DirectorySeparator);
    strcat(fullname, direntry.name);

    for (unsigned int i = 0; i < playlistLength; i++)
    {
        if (playlist[i].used && !strcmp(fullname, playlist[i].filename))
        {
            return true;
        }
    }

    return false;
}

void FolderWidget::setupDirlist(const char* const foldername, fileinput::DirEntry* dirlist, uint32_t length)
{
    Unicode::strncpy(folderNameBuf, foldername, FOLDERNAME_LENGTH);
    titleText.invalidate();

    selected = DIRLIST_LENGTH;
    list.invalidate();
    list.removeAll();
    for (uint32_t i = 0; i < length; i++)
    {
        Unicode::strncpy(listElements[i].nameBuf, dirlist[i].name, FILENAME_LENGTH);
        listElements[i].index = i;
        listElements[i].setSelected(false);
        if (dirlist[i].isDirectory)
        {
            listElements[i].isIncluded = false;
            listElements[i].configure(true);
        }
        else
        {
            listElements[i].isIncluded = isOnPlaylist(foldername, dirlist[i]);
            listElements[i].configure(false);
        }
        list.add(listElements[i]);
    }
    list.invalidate();
}

GenericCallback< const uint32_t >* FolderListElement::itemPressedAction = 0;

FolderListElement::FolderListElement()
{
    nameBuf[0] = 0;
    isSelected = false;

    setTouchable(true);

    setWidth(380);
    setHeight(46);

    background.setBitmap(Bitmap(BITMAP_BROWSE_FOLDER_LISTITEM_BAR_SELECTED_ID));
    background.setVisible(false);

    name.setPosition(85, 8, 250, 42);
    name.setTypedText(TypedText(T_MOVIE_BROWSE_NAME));
    name.setWildcard(nameBuf);
    name.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    button.setXY(310, 0);

    add(background);
    add(icon);
    add(name);
    add(button);
}

void FolderListElement::setSelected(bool selected)
{
    isSelected = selected;
    if (selected)
    {
        background.setVisible(true);
        invalidate();
    }
    else
    {
        if (isIncluded)
        {
            button.setBitmap(Bitmap(BITMAP_SELECTED_ID));
        }
        else
        {
            button.setBitmap(Bitmap());
        }
        invalidate();
        background.setVisible(false);
    }
}

void FolderListElement::handleClickEvent(const ClickEvent& event)
{
    //do not select movie on release if click was cancelled due to drag.
    switch (event.getType())
    {
    case ClickEvent::PRESSED:
        break;
    case ClickEvent::RELEASED:
        if (!clickCancelled)
        {
            if (itemPressedAction && itemPressedAction->isValid())
            {

                if (isSelected)
                {
                    if (isIncluded)
                    {
                        playlistManager->removeFile(index);
                        isIncluded = false;
                        button.setBitmap(Bitmap());
                    }
                    else
                    {
                        isIncluded = playlistManager->addFile(index);
                        if (isIncluded)
                        {
                            button.setBitmap(Bitmap(BITMAP_SELECTED_ID));
                        }
                    }
                }
                else
                {
                    if (isIncluded)
                    {
                        button.setBitmap(Bitmap(BITMAP_MINUS_ID));
                    }
                    else
                    {
                        button.setBitmap(Bitmap(BITMAP_PLUS_ID));
                    }
                }

                itemPressedAction->execute(index);
            }
        }
        clickCancelled = false;
        break;
    case ClickEvent::CANCEL:
        clickCancelled = true;
        break;
    }
}

void FolderListElement::configure(bool isDirectory)
{
    this->isDirectory = isDirectory;
    if (isDirectory)
    {
        icon.setXY(30, 8);
        icon.setBitmap(Bitmap(BITMAP_DIRECTORY_ID));
    }
    else
    {
        icon.setXY(30, 8);
        icon.setBitmap(Bitmap(BITMAP_DOCUMENT_ID));
    }
    if (isIncluded)
    {
        button.setBitmap(Bitmap(BITMAP_SELECTED_ID));
    }
    else
    {
        button.setBitmap(Bitmap());
    }
    invalidate();
}
