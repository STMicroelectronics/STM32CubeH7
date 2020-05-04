#include <gui/gameKnight_screen/GameStatusPopup.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

GameStatusPopup::GameStatusPopup() :
    buttonClickedCallback(this, &GameStatusPopup::buttonClicked),
    popupButtonCallback(0)
{
    setBackground(BITMAP_KNIGHT_MENU_BACKGROUND_ID);
    setShadeColor(Color::getColorFrom24BitRGB(0x0, 0x31, 0x3D));
    setShadeAlpha(230);

    add(ribbon);
    
    backButton.setBitmaps(Bitmap(BITMAP_KNIGHT_BACK_BUTTON_ID), Bitmap(BITMAP_KNIGHT_BACK_BUTTON_PRESSED_ID));
    backButton.setXY(147, 366);
    backButton.setAction(buttonClickedCallback);
    add(backButton);

    playButton.setBitmaps(Bitmap(BITMAP_KNIGHT_PLAY_BUTTON_ID), Bitmap(BITMAP_KNIGHT_PLAY_BUTTON_PRESSED_ID));
    playButton.setXY(319, 366);
    playButton.setAction(buttonClickedCallback);
    add(playButton);

    headline.setColor(Color::getColorFrom24BitRGB(0x64, 0x40, 0x1D));
    add(headline);

    Unicode::snprintf(percentageTextBuffer, BUFFER_SIZE, "%d", 0);
    percentageText.setWildcard(percentageTextBuffer);
    percentageText.setTypedText(TypedText(T_KNIGHT_POPUP_PERCENTAGE));
    percentageText.setColor(Color::getColorFrom24BitRGB(0x64, 0x40, 0x1D));
    add(percentageText);

    progressBar.setRange(0, 100);
    progressBar.setDirection(AbstractDirectionProgress::RIGHT);
    progressBar.setAnchorAtZero(true);
    add(progressBar);

    add(progressBarIcon);
}

GameStatusPopup::~GameStatusPopup()
{

}

void GameStatusPopup::setupWelcome()
{
    progressBar.setVisible(false);
    progressBarIcon.setVisible(false);
    percentageText.setVisible(false);

    headline.setTypedText(TypedText(T_KNIGHT_POPUP_WELCOME));
    headline.setPosition(windowBackground.getX() + ((getBackgroundWidth() - 400) / 2), 0, 400, 100);
    headline.setBaselineY(259);

    ribbon.setBitmap(Bitmap(BITMAP_RIBBON_TITLE_ID));
    ribbon.setXY(25, 6);

    invalidate();
}

void GameStatusPopup::setupGameWon()
{
    progressBar.setVisible(true);
    progressBarIcon.setVisible(true);
    percentageText.setVisible(true);

    headline.setTypedText(TypedText(T_KNIGHT_POPUP_WON));
    headline.setPosition(windowBackground.getX() + ((getBackgroundWidth() - 200) / 2), 0, 200, 30);
    headline.setBaselineY(209);

    ribbon.setBitmap(Bitmap(BITMAP_RIBBON_VICTORY_ID));
    ribbon.setXY(0, 0);

    progressBar.setBackground(Bitmap(BITMAP_POPUP_PROGRESS_BAR_ID));
    progressBar.setXY(157, 274);
    progressBar.setProgressIndicatorPosition(73, 17, Bitmap(BITMAP_POPUP_PROGRESS_BAR_WON_ID).getWidth(), Bitmap(BITMAP_POPUP_PROGRESS_BAR_WON_ID).getHeight());
    progressBar.setBitmap(BITMAP_POPUP_PROGRESS_BAR_WON_ID);
    progressBar.setValue(100);

    progressBarIcon.setBitmap(Bitmap(BITMAP_POPUP_PROGRESS_BAR_VICTORY_ICON_ID));
    progressBarIcon.setXY(progressBar.getX() + 2, progressBar.getY() + 9);

    Unicode::snprintf(percentageTextBuffer, BUFFER_SIZE, "%d", 100);
    percentageText.setPosition(windowBackground.getX() + (getBackgroundWidth() - 300) / 2, 0, 300, 30);
    percentageText.setBaselineY(250);
    invalidate();
}

void GameStatusPopup::setupGameLost(int percentage)
{
    progressBar.setVisible(true);
    progressBarIcon.setVisible(true);
    percentageText.setVisible(true);

    headline.setTypedText(TypedText(T_KNIGHT_POPUP_LOST));
    headline.setPosition(windowBackground.getX() + ((getBackgroundWidth() - 200) / 2), 0, 200, 30);
    headline.setBaselineY(209);

    ribbon.setBitmap(Bitmap(BITMAP_RIBBON_GAME_OVER_ID));
    ribbon.setXY(0, 16);

    progressBar.setBackground(Bitmap(BITMAP_POPUP_PROGRESS_BAR_ID));
    progressBar.setXY(157, 274);
    progressBar.setProgressIndicatorPosition(73, 17, Bitmap(BITMAP_POPUP_PROGRESS_BAR_LOST_ID).getWidth(), Bitmap(BITMAP_POPUP_PROGRESS_BAR_LOST_ID).getHeight());
    progressBar.setBitmap(BITMAP_POPUP_PROGRESS_BAR_LOST_ID);
    progressBar.setValue(percentage);

    progressBarIcon.setBitmap(Bitmap(BITMAP_POPUP_PROGRESS_BAR_HEAD_ICON_ID));
    progressBarIcon.setXY(progressBar.getX() + 8, progressBar.getY() + 2);

    Unicode::snprintf(percentageTextBuffer, BUFFER_SIZE, "%d", percentage);
    percentageText.setPosition(windowBackground.getX() + (getBackgroundWidth() - 300) / 2, 0, 300, 30);
    percentageText.setBaselineY(250);
    invalidate();
}

void GameStatusPopup::setButtonCallback(GenericCallback<const ReturnState >& callback)
{
    popupButtonCallback = &callback;
}

void GameStatusPopup::buttonClicked(const AbstractButton& source)
{
    ReturnState returnState = (&source == &backButton) ? GOTO_MENU : GOTO_PLAY;

    if (popupButtonCallback && popupButtonCallback->isValid())
    {
        popupButtonCallback->execute(returnState);
    }
}

