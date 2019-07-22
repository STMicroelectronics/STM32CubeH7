#ifndef WASHERINSTRUCTIONCONTAINER_HPP
#define WASHERINSTRUCTIONCONTAINER_HPP

#include <widgets/MJPEGReader.hpp>
#include <gui_generated/containers/WasherInstructionContainerBase.hpp>
#include <touchgfx/containers/Container.hpp>
#include <gui/containers/FpsMcuLoad.hpp>

class WasherInstructionContainer : public WasherInstructionContainerBase
{
public:
    WasherInstructionContainer();
    virtual ~WasherInstructionContainer();

    virtual void initialize();

    void tickEvent();

    bool getVideoTexts(int frameno, TypedText& headline, TypedText& body);
    void resetContainer();
    void setMcuLoad(uint8_t load);

protected:
    Callback<WasherInstructionContainer, const Box&, const ClickEvent&> boxClickedCallback;
    void boxClickedHandler(const Box& b, const ClickEvent& evt);

    Callback<WasherInstructionContainer> movieEndedCallback;
    void movieEndedHandler();

    void updateFPSAndMCU();
    uint32_t tickCounter;
    bool playing;
    MJPEGReader mjpgWidget;
    BitmapId movieBufferBitmapId;
    BitmapId temporaryBufferBitmapId;
    Container mjpgContainer;
    FpsMcuLoad fpsMcu;
};

#endif // WASHERINSTRUCTIONCONTAINER_HPP
