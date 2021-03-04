#ifndef MODEL_HPP
#define MODEL_HPP

#include <gui/common/enums.hpp>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void moduleSelected(MODULES value);

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP