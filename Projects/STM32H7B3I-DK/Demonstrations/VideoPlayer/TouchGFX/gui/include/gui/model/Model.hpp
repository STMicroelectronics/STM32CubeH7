#ifndef MODEL_HPP
#define MODEL_HPP

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

    void setVolume(int volume);
    int getVolume();

    void setVideoLength(int length);
    int getVideoLength();

    void setVideoDuration(int duration);
    int getVideoDuration();

    void setPlayVideo(bool play);
    bool getPlayVideo();

    void setSelected(int id);
    int getSelected();

    void setRepeat(bool set);
    bool getRepeat();

    void setShuffle(bool set);
    bool getShuffle();

    void setNumberOfVideos(int);
    int  getNumberOfVideos();

    bool hasVideoEnded();
    int getElapsedTime();

    void updateVideoList(const char *videoFile);

    void refreshVideoList();

    void returnToMenuLauncher();

protected:
    ModelListener* modelListener;

    static const int MAX_NUMBER_OF_VIDEOS = 30;

    int numberOfVideos;
    int volume;
    int videoLength;
    int videoDuration;
    bool playVideo;
    int selected;

    bool repeat;
    bool shuffle;

    int tickCnt;
};

#endif // MODEL_HPP
