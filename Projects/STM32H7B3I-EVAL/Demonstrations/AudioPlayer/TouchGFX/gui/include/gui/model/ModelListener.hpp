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

    virtual void updateMusicDuration(int duration) {}
    virtual void setMusicDuration(int length) {}
    virtual void psCountUpdate(int cnt) {}
    virtual void setPlayingGraphics(bool playing) {}
    virtual void noMediaFilesDetected(bool filesDetected) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
