#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{

}

void MainPresenter::activate()
{

}

void MainPresenter::deactivate()
{

}

void MainPresenter::startDemo(DEMOS demoId)
{
    model->startDemo(demoId);
}

void MainPresenter::SetautoAnimate( bool value )
{
  view.SetautoAnimate( value );
}


bool MainPresenter::GetautoAnimate( )
{
  return view.GetautoAnimate( );
}
