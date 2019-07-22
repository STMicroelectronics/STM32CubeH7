

#include <gui/video_player_screen/VideoPlayerPresenter.hpp>
#include <gui/video_player_screen/VideoPlayerView.hpp>
#include <gui/common/Defines.hpp>

VideoPlayerPresenter::VideoPlayerPresenter(VideoPlayerView& v) :
    DemoPresenter(v),
    view(v),
    counter(0)
{
}

void VideoPlayerPresenter::activate()
{
	model->setCurrentScreen(Defines::VIDEO_PLAYER_SCREEN);
}

void VideoPlayerPresenter::deactivate()
{

}

void VideoPlayerPresenter::notifyMediaDisconnected()
{
    application()->gotoVideoPlayerScreen();
}

void VideoPlayerPresenter::notifyMediaConnected()
{
    application()->gotoVideoPlayerScreen();
}

void VideoPlayerPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 2:
        view.screenClick(ClickEvent::PRESSED, 57, 442);
        break;
    case 3:
        view.screenClick(ClickEvent::RELEASED, 57, 442);
        break;
    case 6:
        view.screenClick(ClickEvent::PRESSED, 339, 261);
        break;
    case 8:
        view.screenClick(ClickEvent::RELEASED, 339, 261);
        break;
    case 12:
        view.screenClick(ClickEvent::PRESSED, 178, 150);
        break;
    case 13:
        view.screenClick(ClickEvent::RELEASED, 178, 150);
        break;
    case 25:
        view.screenClick(ClickEvent::PRESSED, 200, 100);
        break;
    case 28:
        view.screenClick(ClickEvent::RELEASED, 200, 100);
        break;
    case 31:
        view.screenClick(ClickEvent::PRESSED, 57, 442);
        break;
    case 33:
        view.screenClick(ClickEvent::RELEASED, 57, 442);
        break;
    case 36:
        view.screenClick(ClickEvent::PRESSED, 57, 390);
        break;
    case 38:
        view.screenClick(ClickEvent::RELEASED, 57, 390);
        break;
    case 40:
        view.screenClick(ClickEvent::PRESSED, 410, 280);
        break;
    case 42:
        view.screenClick(ClickEvent::RELEASED, 410, 280);
        break;
    case 54:
        view.screenClick(ClickEvent::PRESSED, 410, 280);
        break;
    case 55:
        view.screenClick(ClickEvent::RELEASED, 410, 280);
        break;
    case 57:
        view.screenClick(ClickEvent::PRESSED, 739, 443);
        break;
    case 59:
        view.screenClick(ClickEvent::RELEASED, 739, 443);
        break;
    case 66:
        view.screenClick(ClickEvent::PRESSED, 42, 31);
        break;
    case 68:
        view.screenClick(ClickEvent::RELEASED, 42, 31);
        break;
    }
}

void VideoPlayerPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
