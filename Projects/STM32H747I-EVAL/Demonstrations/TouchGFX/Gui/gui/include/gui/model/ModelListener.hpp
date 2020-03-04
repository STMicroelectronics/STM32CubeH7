


#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <touchgfx/hal/Types.hpp>

class Model;

/**
 * ModelListener is the interface through which the Model can inform the currently
 * active presenter of events. All presenters should derive from this class.
 * It also provides a model pointer for the presenter to interact with the Model.
 *
 * The bind function is called automatically.
 *
 * Add the virtual functions Model should be able to call on the active presenter to this class.
 */
class ModelListener
{
public:
    ModelListener() : model(0) {}
    virtual ~ModelListener() {}

    /**
     * Sets the model pointer to point to the Model object. Called automatically
     * when switching screen.
     */
    void bind(Model* m) { model = m; }

    virtual void mcuLoadUpdated(uint8_t mcuLoad) {}
    
    virtual void screenSaverMinorTick() {}
    virtual void screenSaverMajorTick() {}

    virtual void newJunctionTempValue(int value) {}
    virtual void newPotentiometerValue(int value) {}
    virtual void updateTime(uint8_t hour, uint8_t minute, uint8_t second) {}
    virtual void notifyAudioEndOfFile() {}
    virtual void notifyMediaDisconnected() {}
    virtual void notifyMediaConnected() {}
    virtual void oneSecond() {}
    virtual void presenterTick() {}

protected:
    Model* model;
};

#endif /* MODELLISTENER_HPP */
