


#ifndef FRONTENDHEAP_HPP
#define FRONTENDHEAP_HPP

#include <common/Meta.hpp>
#include <common/Partition.hpp>
#include <mvp/MVPHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>
#include <gui/main_menu_screen/MainMenuCarouselView.hpp>
#include <gui/main_menu_screen/MainMenuAnimatingButtonsView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/game2048_screen/Game2048View.hpp>
#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <gui/game2D_screen/Game2DView.hpp>
#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/controls_screen/ControlsView.hpp>
#include <gui/controls_screen/ControlsPresenter.hpp>
#include <gui/audio_player_screen/AudioPlayerPresenter.hpp>
#include <gui/audio_player_screen/AudioPlayerView.hpp>
#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>
#include <gui/settings_screen/SettingsView.hpp>
#include <gui/video_player_screen/VideoPlayerPresenter.hpp>
#include <gui/video_player_screen/VideoPlayerView.hpp>

/**
 * This class provides the memory that shall be used for memory allocations
 * in the frontend. A single instance of the FrontendHeap is allocated once (in heap
 * memory), and all other frontend objects such as views, presenters and data model are
 * allocated within the scope of this FrontendHeap. As such, the RAM usage of the entire
 * user interface is sizeof(FrontendHeap).
 *
 * @note The FrontendHeap reserves memory for the most memory-consuming presenter and
 * view only. The largest of these classes are determined at compile-time using template
 * magic. As such, it is important to add all presenters, views and transitions to the
 * type lists in this class.
 *
 */
class FrontendHeap : public MVPHeap
{
public:
    /**
     * A list of all view types. Must end with meta::Nil.
     * @note All view types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< MainMenuCarouselView,
            meta::TypeList< MainMenuAnimatingButtonsView,
            meta::TypeList< Game2048View,
            meta::TypeList< Game2DView,
            meta::TypeList< ControlsView,
            meta::TypeList< AudioPlayerView,
            meta::TypeList< HomeAutomationView,
            meta::TypeList< SettingsView,
            meta::TypeList< VideoPlayerView,
            meta::Nil
            > > > > > > > > > ViewTypes;

    /**
     * Determine (compile time) the View type of largest size.
     */
    typedef meta::select_type_maxsize< ViewTypes >::type MaxViewType;

    /**
     * A list of all presenter types. Must end with meta::Nil.
     * @note All presenter types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< MainMenuPresenter,
            meta::TypeList< Game2048Presenter,
            meta::TypeList< Game2DPresenter,
            meta::TypeList< ControlsPresenter,
            meta::TypeList< AudioPlayerPresenter,
            meta::TypeList< HomeAutomationPresenter,
            meta::TypeList< SettingsPresenter,
            meta::TypeList< VideoPlayerPresenter,
            meta::Nil
            > > > > > > > > PresenterTypes;

    /**
     * Determine (compile time) the Presenter type of largest size.
     */
    typedef meta::select_type_maxsize< PresenterTypes >::type MaxPresenterType;

    /**
     * A list of all transition types. Must end with meta::Nil.
     * @note All transition types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< NoTransition,
            meta::Nil
            > TransitionTypes;

    /**
     * Determine (compile time) the Transition type of largest size.
     */
    typedef meta::select_type_maxsize< TransitionTypes >::type MaxTransitionType;

    static FrontendHeap& getInstance()
    {
        static FrontendHeap instance;
        return instance;
    }

    Partition< PresenterTypes, 1 > presenters;
    Partition< ViewTypes, 1 > views;
    Partition< TransitionTypes, 1 > transitions;
    FrontendApplication app;
    Model model;
private:
    FrontendHeap()
        : MVPHeap(presenters, views, transitions, app),
          presenters(),
          views(),
          transitions(),
          app(model, *this)
    {
        // Goto start screen
        //app.gotoControlsScreen();
        //app.gotoSettingsScreen();
        //app.gotoHomeAutomationScreen();
        app.gotoMainMenuCarouselScreen();
        //app.gotoAudioPlayerScreen();
        //app.gotoVideoPlayerScreen();
    };
};
#endif
