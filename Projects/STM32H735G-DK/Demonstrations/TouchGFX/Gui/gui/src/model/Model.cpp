#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifndef SIMULATOR
#include "bsp.h"
#endif /* SIMULATOR */

Model::Model() :  
    autoDemoState(HallwayKitchen),
    modelListener(0),   
    showIntro(true),
    showLight(false),
    showPot(false),
    showOven(false),
    hoodUIEntered(false),
    ovenUIEntered(false),
    kitchenUIEntered(false),
    fanState(false),
    fanSpeed(0),
    selectedOvenMode(0),
    ovenTimerHour(1),
    ovenTimerMinute(30),
    ovenTemperature(150),    
    washerUIEntered(false),
    bathtubUIEntered(false),
    bathroomUIEntered(false),
    bathtubOn(false),
    tempCelcius(true),
    jetOn(false),
    lightOn(false),
    tempValue(10),
    jetLevel(1),
    colorCoordinateX(75),
    colorCoordinateY(81), 
    currentProgram(0),
    washingTime(160),
    bathroomHomeUI(false),
    fromRecipeScreen(false),
    mcuLoadLast(0),
    autoDemoCnt(0),
    autoDemoTimeOutCnt(0),
    autoDemoOn(false)
{
#ifndef SIMULATOR
    HAL_PWR_EnableBkUpAccess();
    uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
    uint32_t autoDemoId = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
    uint32_t autoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
    if(autoDemoTimeOutMs == 0)
    {
      autoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
    }
    if(autoDemoId)
    {
      /* Set SW Reset value to 2 : Pressing SW Reset button will do SW reset with this value */
      WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x2));
    }
    else
    {
      WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
    }
    HAL_PWR_DisableBkUpAccess();
#endif // SIMULATOR
}

void Model::tick()
{
    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);               
    }
    if (autoDemoOn)
    {
        if (autoDemoCnt == AUTO_DEMO_SCALER)
        {
            autoDemoCnt = 0;
            modelListener->autoDemoTick();
        }
        else
        {
            autoDemoCnt++;
        }
    }
    else if (autoDemoTimeOutCnt >= AUTO_DEMO_TIMEOUT && startAutoDemo())
    {
        resetDemo();
        autoDemoOn = true;
        modelListener->autoDemoStart();
#ifndef SIMULATOR
        /* Set SW Reset to 2 */
        HAL_PWR_EnableBkUpAccess();
        uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
        WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x2));
        HAL_PWR_DisableBkUpAccess();
#endif // SIMULATOR
    }
    else if(autoDemoTimeOutCnt <= AUTO_DEMO_TIMEOUT)
    {
        //autoDemoTimeOutCnt++; // Commented out to disable auto demo mode
    }
}

void Model::resetAutoDemo()
{
    autoDemoOn = false;
    autoDemoCnt = 0;
    autoDemoTimeOutCnt = 0;
#ifndef SIMULATOR
    if ( getAutoDemoState() == Model::DemoDone )
    {
        /* Set SW Reset to 1 */
        HAL_PWR_EnableBkUpAccess();
        uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
        WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
        HAL_PWR_DisableBkUpAccess();
        /* Return back to ML */
        NVIC_SystemReset();
    }
    else
    {
        /* Set SW Reset to 2 */
        HAL_PWR_EnableBkUpAccess();
        uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
        WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x2));
        HAL_PWR_DisableBkUpAccess();
    }
#endif // SIMULATOR
}

void Model::setAutoDemoState(AutoDemoStates state)
{
    autoDemoState = state;
}

Model::AutoDemoStates Model::getAutoDemoState()
{
    return autoDemoState;
}

void Model::setCurrentView(Views view)
{
    currentView = view;
}

bool Model::startAutoDemo()
{
    return (currentView == Hallway || currentView == Kitchen || currentView == Bathroom);
}

void Model::setShowIntro(bool show)
{
    showIntro = show;
}

bool Model::getShowIntro()
{
    return showIntro;
}

void Model::setShowLight(bool show)
{
    showLight = show;
}

bool Model::getShowLight()
{
    return showLight;
}

void Model::setShowPot(bool show)
{
    showPot = show;
}

bool Model::getShowPot()
{
    return showPot;
}

void Model::setShowOven(bool show)
{
    showOven = show;
}

bool Model::getShowOven()
{
    return showOven;
}

void Model::setFanState(bool state)
{
    fanState = state;
}

bool Model::getFanState()
{
    return fanState;
}

void Model::setFanSpeed(uint16_t speed)
{
    fanSpeed = speed;
}

uint16_t Model::getFanSpeed()
{
    return fanSpeed;
}

void Model::setBathtubOn(bool on)
{
    bathtubOn = on;
}

bool Model::getBathtubOn()
{
    return bathtubOn;
}

void Model::setTempCelcius(bool temp)
{
    tempCelcius = temp;
}

bool Model::getTempCelcius()
{
    return tempCelcius;
}

void Model::setJetOn(bool on)
{
    jetOn = on;
}

bool Model::getJetOn()
{
    return jetOn;
}

void Model::setLightOn(bool on)
{
    lightOn = on;
}

bool Model::getLightOn()
{
    return lightOn;
}

void Model::setTempValue(uint16_t temp)
{
    tempValue = temp;
}

uint16_t Model::getTempValue()
{
    return tempValue;
}

void Model::setJetLevel(uint16_t level)
{
    jetLevel = level;
}


uint16_t Model::getJetLevel()
{
    return jetLevel;
}


void Model::setColorCoordinates(uint32_t x, uint32_t y)
{
    colorCoordinateX = x;
    colorCoordinateY = y;
}


void Model::getColorCoordinates(uint32_t& x, uint32_t& y)
{
    x = colorCoordinateX;
    y = colorCoordinateY;
}

void Model::setSelectedOvenMode(uint16_t mode)
{
    selectedOvenMode = mode;
}

uint16_t Model::getSelectedOvenMode()
{
    return selectedOvenMode;
}

void Model::setOvenTimerHour(int16_t hour)
{
    ovenTimerHour = hour;
}

int16_t Model::getOvenTimerHour()
{
    return ovenTimerHour;
}

void Model::setOvenTimerMinute(int16_t minute)
{
    ovenTimerMinute = minute;
}

int16_t Model::getOvenTimerMinute()
{
    return ovenTimerMinute;
}

void Model::setOvenTemperature(uint16_t temp)
{
    ovenTemperature = temp;
}

uint16_t Model::getOvenTemperature()
{
    return ovenTemperature;
}

void Model::setWashingProgram(uint16_t program)
{
    currentProgram = program;
}

uint16_t Model::getWashingProgram()
{
    return currentProgram;
}

void Model::setWashingTime(uint16_t time)
{
    washingTime = time;
}

uint16_t Model::getWashingTime()
{
    return washingTime;
}

void Model::setBathroomHomeUI(bool bathroom)
{
    bathroomHomeUI = bathroom;
}

bool Model::getBathroomHomeUI()
{
    return bathroomHomeUI;
}

void Model::setHoodEntered(bool entered)
{
    hoodUIEntered = entered;
}

bool Model::getHoodEntered()
{
    return hoodUIEntered;
}

void Model::setOvenEntered(bool entered)
{
    ovenUIEntered = entered;
}

bool Model::getOvenEntered()
{
    return ovenUIEntered;
}

void Model::setKitchenUIEntered(bool entered)
{
    kitchenUIEntered = entered;
}

bool Model::getKitchenUIEntered()
{
    return kitchenUIEntered;
}

void Model::setBathtubUIEntered(bool entered)
{
    bathtubUIEntered = entered;
}

bool Model::getBathtubUIEntered()
{
    return bathtubUIEntered;
}

void Model::setBathroomUIEntered(bool entered)
{
    bathroomUIEntered = entered;
}

bool Model::getBathroomUIEntered()
{
    return bathroomUIEntered;
}

void Model::setWasherUIEntered(bool entered)
{
    washerUIEntered = entered;
}

bool Model::getWasherUIEntered()
{
    return washerUIEntered;
}

void Model::setFromRecipeScreen(bool recipeScreen)
{
    fromRecipeScreen = recipeScreen;
}

bool Model::getFromRecipeScreen()
{
    return fromRecipeScreen;
}

void Model::resetDemo()
{
    autoDemoState = HallwayKitchen;
    showIntro = true;
    showLight = false;
    showPot = false;
    showOven = false;
    hoodUIEntered = false;
    ovenUIEntered = false;
    kitchenUIEntered = false;
    washerUIEntered = false;
    bathtubUIEntered = false;
    bathroomUIEntered = false;
    fanState = false;
    fanSpeed = 0;
    bathtubOn = false;
    tempCelcius = true;
    jetOn = false;
    lightOn = false;
    tempValue = 10;
    jetLevel = 1;
    colorCoordinateX = 75;
    colorCoordinateY = 81;
    selectedOvenMode = 0;
    ovenTimerHour = 1;
    ovenTimerMinute = 30;
    ovenTemperature = 150; 
    currentProgram = 0;
    washingTime = 160;
    bathroomHomeUI = false;
    fromRecipeScreen = false;
}
