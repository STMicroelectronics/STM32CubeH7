#include <gui/containers/infoBox.hpp>
#include <gui/common/enums.hpp>
#include <texts/TextKeysAndLanguages.hpp>

infoBox::infoBox()
{

}

void infoBox::initialize()
{
    infoBoxBase::initialize();
}

void infoBox::setUpContent(int module)
{
    switch (module)
    {
    case BOARD_INFO:
        headline.setTypedText(TypedText(T_HEADLINEBOARDINFO));
        infoText.setTypedText(TypedText(T_INFOBOARDINFO));
        startButton.setVisible(false);
        break;
    case MUSIC_PLAYER:
        headline.setTypedText(TypedText(T_HEADLINEMUSICPLAYER));
        infoText.setTypedText(TypedText(T_INFOMUSICPLAYER));
        startButton.setVisible(true);
        break;
    case TOUCHGFX:
        headline.setTypedText(TypedText(T_HEADLINETOUCHGFX));
        infoText.setTypedText(TypedText(T_INFOTOUCHGFX));
        startButton.setVisible(true);
        break;
    case VIDEO_PLAYER:
        headline.setTypedText(TypedText(T_HEADLINEVIDEOPLAYER));
        infoText.setTypedText(TypedText(T_INFOVIDEOPLAYER));
        startButton.setVisible(true);
        break;
    case WEATHER:
        headline.setTypedText(TypedText(T_HEADLINECLOCKANDWEATHER));
        infoText.setTypedText(TypedText(T_INFOCLOCKANDWEATHER));
        startButton.setVisible(true);
        break;
    case ST_EMWIN:
        headline.setTypedText(TypedText(T_HEADLINESTEMWIN));
        infoText.setTypedText(TypedText(T_INFOSTEMWIN));
        startButton.setVisible(true);
        break;
    case TARA:
        headline.setTypedText(TypedText(T_HEADLINETARA));
        infoText.setTypedText(TypedText(T_INFOTARA));
        startButton.setVisible(true);
        break;
    }

    headline.invalidate();
    infoText.invalidate();
    startButton.invalidate();
}

void infoBox::setFirmwareVersion(const char* fwVersion)
{
    Unicode::strncpy(infoTextBuffer, fwVersion, INFOTEXT_SIZE);
}
