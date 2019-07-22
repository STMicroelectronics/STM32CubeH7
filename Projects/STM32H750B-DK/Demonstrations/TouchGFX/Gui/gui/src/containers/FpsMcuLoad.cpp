#include <gui/containers/FpsMcuLoad.hpp>
#include <texts/TextKeysAndLanguages.hpp>

FpsMcuLoad::FpsMcuLoad() :
    mcuOnly(false),
    frameSkippedCounter(0),
    frames(0),
    fps(0),
    mcu(0)
{

}

void FpsMcuLoad::initialize()
{
    FpsMcuLoadBase::initialize();
}

void FpsMcuLoad::updateFPSAndMCU()
{
    if (mcuOnly)
    {
        Unicode::snprintf(mcuWildcardBuffer, MCUWILDCARD_SIZE, "%d", mcu);
        mcuWildcard.invalidate();
    }
    else
    {
        Unicode::snprintf(fps_mps_wildcardBuffer, FPS_MPS_WILDCARD_SIZE, "%d / %d", fps, mcu);
        fps_mps_wildcard.invalidate();
    }

}

void FpsMcuLoad::update()
{
    if (HAL::getInstance()->getLCDRefreshCount() > 1)
    {
        frameSkippedCounter++;
    }
    else
    {
        frames++;
    }

    if (frames + frameSkippedCounter >= 60)
    {
        if (fps != frames)
        {
            fps = frames;
            updateFPSAndMCU();
        }
        frameSkippedCounter = 0;
        frames = 0;
    }
}

void FpsMcuLoad::setMcuLoad(uint8_t load)
{
    mcu = load;
    updateFPSAndMCU();
}

void FpsMcuLoad::setTextColor(colortype textColor)
{
    fps_mps_wildcard.setColor(textColor);
    MCU_FPS_text.setColor(textColor);

    mcuWildcard.setColor(textColor);
    mcuText.setColor(textColor);
}

void FpsMcuLoad::setMCUOnly(bool setMcuOnly)
{
    mcuOnly = setMcuOnly;

    fps_mps_wildcard.setVisible(!setMcuOnly);
    MCU_FPS_text.setVisible(!setMcuOnly);
    mcuWildcard.setVisible(setMcuOnly);
    mcuText.setVisible(setMcuOnly);

    fps_mps_wildcard.invalidate();
    MCU_FPS_text.invalidate();
    mcuWildcard.invalidate();
    mcuText.invalidate();
}
