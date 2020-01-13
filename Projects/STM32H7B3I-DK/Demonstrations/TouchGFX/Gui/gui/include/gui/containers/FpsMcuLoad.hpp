#ifndef FPSMCULOAD_HPP
#define FPSMCULOAD_HPP

#include <gui_generated/containers/FpsMcuLoadBase.hpp>
#include <touchgfx/Color.hpp>

class FpsMcuLoad : public FpsMcuLoadBase
{
public:
    FpsMcuLoad();
    virtual ~FpsMcuLoad() {}

    virtual void initialize();

    void update();
    void setMcuLoad(uint8_t load);

    void setTextColor(colortype textColor);
    void setMCUOnly(bool setMcuOnly);

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
    bool mcuOnly;

    void updateFPSAndMCU();
    int frameSkippedCounter;
    int frames;
    int fps;
    int mcu;

};

#endif // FPSMCULOAD_HPP
