#include <gui/graphics_effect_screen/graphics_effectView.hpp>
#include <gui/graphics_effect_screen/graphics_effectPresenter.hpp>

graphics_effectPresenter::graphics_effectPresenter(graphics_effectView& v)
    : view(v)
{
}

void graphics_effectPresenter::activate()
{
    HAL::getInstance()->setFrameRateCompensation(true);
}

void graphics_effectPresenter::deactivate()
{
    HAL::getInstance()->setFrameRateCompensation(false);
}

void graphics_effectPresenter::gotoMenuSelected()
{
    if (model->getPreviousSelectedMainMenuType() == Defines::CAROUSEL_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreenNoTransition();
    }
    else if (model->getPreviousSelectedMainMenuType() == Defines::ANIMATING_BUTTONS_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreenNoTransition();
    }
}
