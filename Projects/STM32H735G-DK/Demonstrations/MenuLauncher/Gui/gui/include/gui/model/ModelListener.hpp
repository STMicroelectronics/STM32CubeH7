#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void SetautoAnimate ( bool value ) { }
    virtual bool GetautoAnimate () { return false; }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
