#ifndef GAMESTATUSPOPUP_HPP
#define GAMESTATUSPOPUP_HPP

#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>

using namespace touchgfx;

class GameStatusPopup : public ModalWindow
{
public:
    enum ReturnState
    {
        GOTO_MENU,
        GOTO_PLAY
    };

    GameStatusPopup();
    virtual ~GameStatusPopup();

    void setupWelcome();
    void setupGameWon();
    void setupGameLost(int percentage);

    void setButtonCallback(GenericCallback<const ReturnState >& callback);

private:
    static const int BUFFER_SIZE = 6;

    Button backButton;
    Button playButton;

    TextArea headline;
    TextAreaWithOneWildcard percentageText;
    Unicode::UnicodeChar percentageTextBuffer[BUFFER_SIZE];

    Image ribbon;
    ImageProgress progressBar;
    Image progressBarIcon;

    Callback<GameStatusPopup, const AbstractButton&> buttonClickedCallback;
    void buttonClicked(const AbstractButton& source);

    GenericCallback<const ReturnState >* popupButtonCallback;
};

#endif
