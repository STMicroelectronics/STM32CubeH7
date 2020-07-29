#ifndef MODEL_HPP
#define MODEL_HPP

#include "gui/common/enums.hpp"

class ModelListener;

class Model
{
public:
    static const int TICK_CNT = 16;      // ticks per second

    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void startDemo(DEMOS demoId);

    unsigned int GetautoDemoTicks() { return ((autoDemoTimeOutMs * 1000) / TICK_CNT); }

    void SetautoAnimate( bool value );
    bool GetautoAnimate();

protected:
    ModelListener* modelListener;

private:
	unsigned int autoDemoTimeOutMs;
};

#endif // MODEL_HPP
