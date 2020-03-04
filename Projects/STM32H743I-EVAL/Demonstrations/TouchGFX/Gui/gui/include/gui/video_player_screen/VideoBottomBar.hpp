#ifndef VIDEO_BOTTOM_BAR_HPP
#define VIDEO_BOTTOM_BAR_HPP

#include <gui/common/BottomBar.hpp>

class VideoBottomBar : public BottomBar
{
public:
    VideoBottomBar();
    virtual ~VideoBottomBar() {}
    
private:
    virtual void buttonClicked(const AbstractButton& btn);
    static const int ON_OFF_LENGTH = 4;
    ToggleButton repeatButton;
    ToggleButton hwJPEGButton;
    TextAreaWithOneWildcard hwJPEGText;
    Unicode::UnicodeChar onOffBuf[ON_OFF_LENGTH];
};

#endif /* VIDEO_BOTTOM_BAR_HPP */
