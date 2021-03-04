/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef ZOOMANIMATIONIMAGE_HPP
#define ZOOMANIMATIONIMAGE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * @class ZoomAnimationImage ZoomAnimationImage.hpp touchgfx/containers/ZoomAnimationImage.hpp
 *
 * @brief Class for optimizing and wrapping move and zoom operations on ScalableImages.
 *
 *        Class for optimizing and wrapping move and zoom operations on ScalableImages. The
 *        ZoomAnimationImage takes two bitmaps representing the same image but at a small and a
 *        large resolution. These bitmaps should be the sizes that are used when not animating
 *        the image. The ZoomAnimationImage will use an Image for displaying the bitmap when
 *        its width and height matches one of them. When it does not it will use a
 *        ScalableImage instead. The main idea is that the supplied bitmaps should be the end
 *        points of the zoom animation so that it ends up using an Image when not animating.
 *        This is, however, not a required. You can animate from and to sizes that are not
 *        equal the sizes of the bitmaps. The result is a container that has the high
 *        performance of an ordinary image when the size matches the prerendered bitmaps.
 *        Moreover it supplies easy to use animation functions that lets you zoom and move the
 *        image.
 *
 * @note Note that since this container uses the ScalableImage it has the same restrictions. That
 *       means no 1 bit per pixel mode.
 *
 * @see ScalableImage
 */
class ZoomAnimationImage : public touchgfx::Container
{
public:

    /**
     * @enum ZoomMode
     *
     * @brief A ZoomMode describes in which direction the image will grow/shrink when do a zoom
     *        animation. A FIXED direction means that the image will not grow/shrink in that
     *        direction.
     */
    enum ZoomMode
    {
        FIXED_CENTER = 0,
        FIXED_LEFT,
        FIXED_RIGHT,
        FIXED_TOP,
        FIXED_BOTTOM,
        FIXED_LEFT_AND_TOP,
        FIXED_RIGHT_AND_TOP,
        FIXED_LEFT_AND_BOTTOM,
        FIXED_RIGHT_AND_BOTTOM
    };

    /**
     * @fn ZoomAnimationImage::ZoomAnimationImage();
     *
     * @brief Default constructor.
     *
     *        Creates and initialize the ZoomAnimationImage.
     */
    ZoomAnimationImage();

    /**
     * @fn virtual ZoomAnimationImage::~ZoomAnimationImage();
     *
     * @brief Destructor.
     *
     *        Destroys the ZoomAnimationImage.
     */
    virtual ~ZoomAnimationImage();

    /**
     * @fn void ZoomAnimationImage::startZoomAnimation(int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode = FIXED_LEFT_AND_TOP, touchgfx::EasingEquation widthProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation heightProgressionEquation = &touchgfx::EasingEquations::linearEaseNone);
     *
     * @brief Setup and starts the zoom animation.
     *
     *        Setup and starts the zoom animation. At end of the animation the image will have
     *        been resized to the endWidth and endHeight. The development of the width and
     *        height during the animation is described by the supplied EasingEquations. The
     *        container is registered as a TimerWidget. Unregistering is handled automatically
     *        when the animation has finished.
     *
     *        Note that the animation follows the specified ZoomMode so the X and Y of the
     *        image might change during animation.
     *
     * @param endWidth                  The width of the image at animation end.
     * @param endHeight                 The height of the image at animation end.
     * @param duration                  The duration of the animation measured in ticks.
     * @param zoomMode                  The zoom mode that will be used during the animation.
     *                                  Default = FIXED_LEFT_AND_TOP.
     * @param widthProgressionEquation  The equation that describes the development of the width
     *                                  during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     * @param heightProgressionEquation The equation that describes the development of the height
     *                                  during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     */
    void startZoomAnimation(int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode = FIXED_LEFT_AND_TOP, touchgfx::EasingEquation widthProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation heightProgressionEquation = &touchgfx::EasingEquations::linearEaseNone);

    /**
     * @fn void ZoomAnimationImage::startZoomAndMoveAnimation(int16_t endX, int16_t endY, int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode = FIXED_LEFT_AND_TOP, EasingEquation xProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation yProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation widthProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation heightProgressionEquation = &touchgfx::EasingEquations::linearEaseNone);
     *
     * @brief Setup and starts the zoom and move animation.
     *
     *        Setup and starts the zoom and move animation. At end of the animation the image
     *        will have been resized to the endWidth and endHeight and have moved from its
     *        original position to the endX and endY. Please note that the ZoomMode might
     *        influence the actual end position since the zoom transformation might change the
     *        X and Y of the image. The ZoomMode FIXED_LEFT_AND_TOP ensures that the endX and
     *        endY will be the actual end position.
     *
     *        The development of the width, height, X and Y during the animation is described
     *        by the supplied EasingEquations. The container is registered as a TimerWidget.
     *        Unregistering is handled automatically when the animation has finished.
     *
     * @param endX                      The X position of the image at animation end. Relative to
     *                                  the container or view that holds the ZoomAnimationImage.
     * @param endY                      The Y position of the image at animation end. Relative to
     *                                  the container or view that holds the ZoomAnimationImage.
     * @param endWidth                  The width of the image at animation end.
     * @param endHeight                 The height of the image at animation end.
     * @param duration                  The duration of the animation measured in ticks.
     * @param zoomMode                  The zoom mode that will be used during the animation.
     *                                  Default = FIXED_LEFT_AND_TOP.
     * @param xProgressionEquation      The equation that describes the development of the X
     *                                  position during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     * @param yProgressionEquation      The equation that describes the development of the Y
     *                                  position during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     * @param widthProgressionEquation  The equation that describes the development of the width
     *                                  during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     * @param heightProgressionEquation The equation that describes the development of the height
     *                                  during the animation. Default =
     *                                  EasingEquations::linearEaseNone.
     */
    void startZoomAndMoveAnimation(int16_t endX, int16_t endY, int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode = FIXED_LEFT_AND_TOP, EasingEquation xProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation yProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation widthProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation heightProgressionEquation = &touchgfx::EasingEquations::linearEaseNone);

    /**
     * @fn void ZoomAnimationImage::cancelZoomAnimation();
     *
     * @brief Cancel zoom animation.
     */
    void cancelZoomAnimation();

    /**
     * @fn virtual void ZoomAnimationImage::handleTickEvent();
     *
     * @brief The tick handler.
     *
     *        The tick handler.
     */
    virtual void handleTickEvent();

    /**
     * @fn void ZoomAnimationImage::setBitmaps(const Bitmap& smallBitmap, const Bitmap& largeBitmap);
     *
     * @brief Initializes the bitmap of the image to be used.
     *
     *        Initializes the bitmap of the image to be used. The bitmaps should represent the
     *        same image in the two needed static resolutions. Note that it is possible to
     *        scale the image beyond the sizes of these bitmaps.
     *
     * @param smallBitmap The image in the smallest resolution.
     * @param largeBitmap The image in the largest resolution.
     */
    void setBitmaps(const Bitmap& smallBitmap, const Bitmap& largeBitmap);

    /**
     * @fn Bitmap ZoomAnimationImage::getSmallBitmap() const
     *
     * @brief Gets the small bitmap.
     *
     *        Gets the small bitmap.
     *
     * @return the small bitmap.
     */
    Bitmap getSmallBitmap() const
    {
        return smallBmp;
    }

    /**
     * @fn Bitmap ZoomAnimationImage::getLargeBitmap() const
     *
     * @brief Gets the large bitmap.
     *
     *        Gets the large bitmap.
     *
     * @return the large bitmap.
     */
    Bitmap getLargeBitmap() const
    {
        return largeBmp;
    }

    /**
     * @fn virtual void ZoomAnimationImage::setPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the size and position of the image, relative to its parent.
     *
     *        Sets the size and position of the image, relative to its parent. Chooses the
     *        optimal rendering method afterwards The image is automatically invalidated.
     *
     * @param x      The x coordinate of this Drawable.
     * @param y      The y coordinate of this Drawable.
     * @param width  The width of this Drawable.
     * @param height The height of this Drawable.
     */
    virtual void setPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void ZoomAnimationImage::setWidth(int16_t width);
     *
     * @brief Sets the width of the image.
     *
     *        Sets the width of the image. Chooses the optimal rendering method afterwards The
     *        image is automatically invalidated.
     *
     * @param width The new width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void ZoomAnimationImage::setHeight(int16_t height);
     *
     * @brief Sets the height of the image.
     *
     *        Sets the height of the image. Chooses the optimal rendering method afterwards.
     *        The image is automatically invalidated.
     *
     * @param height The new height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn virtual void ZoomAnimationImage::setDimension(int16_t width, int16_t height);
     *
     * @brief Sets the width and height of the image.
     *
     *        Sets the width and height of the image. Chooses the optimal rendering method
     *        afterwards The image is automatically invalidated.
     *
     * @param width  The new width.
     * @param height The new height.
     */
    virtual void setDimension(int16_t width, int16_t height);

    /**
     * @fn virtual void ZoomAnimationImage::setScalingMode(ScalableImage::ScalingAlgorithm mode);
     *
     * @brief Sets the scaling algorithm of the ScalableImage.
     *
     *        Sets the scaling algorithm of the ScalableImage.
     *
     * @param mode The new mode.
     */
    virtual void setScalingMode(ScalableImage::ScalingAlgorithm mode);

    /**
     * @fn virtual ScalableImage::ScalingAlgorithm ZoomAnimationImage::getScalingMode();
     *
     * @brief Gets the scaling algorithm of the ScalableImage.
     *
     *        Gets the scaling algorithm of the ScalableImage.
     *
     * @return the scaling algorithm used.
     */
    virtual ScalableImage::ScalingAlgorithm getScalingMode();

    /**
     * @fn virtual void ZoomAnimationImage::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t ZoomAnimationImage::getAlpha() const;
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    virtual uint8_t getAlpha() const;

    /**
     * @fn virtual void ZoomAnimationImage::setAnimationDelay(uint16_t delay);
     *
     * @brief Sets a delay on animations done by the ZoomAnimationImage.
     *
     *        Sets a delay on animations done by the ZoomAnimationImage. Defaults to 0.
     *
     * @param delay The delay in ticks.
     */
    virtual void setAnimationDelay(uint16_t delay);

    /**
     * @fn virtual uint16_t ZoomAnimationImage::getAnimationDelay() const;
     *
     * @brief Gets the current animation delay.
     *
     *        Gets the current animation delay.
     *
     * @return The current animation delay. Expressed in ticks.
     */
    virtual uint16_t getAnimationDelay() const;

    /**
     * @fn void ZoomAnimationImage::setAnimationEndedCallback(touchgfx::GenericCallback< const ZoomAnimationImage& >& callback)
     *
     * @brief Associates an action to be performed when the animation ends.
     *
     *        Associates an action to be performed when the animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the ZoomAnimationImage.
     *
     * @see GenericCallback
     */
    void setAnimationEndedCallback(touchgfx::GenericCallback< const ZoomAnimationImage& >& callback)
    {
        animationEndedAction = &callback;
    }

    /**
     * @fn virtual bool ZoomAnimationImage::isRunning() const;
     *
     * @brief Is there currently an animation running.
     *
     *        Is there currently an animation running.
     *
     * @return true if there is an animation running.
     * @deprecated use isZoomAnimationRunning()
     */
    virtual bool isRunning() const;

    /**
     * @fn virtual bool ZoomAnimationImage::isZoomAnimationRunning() const;
     *
     * @brief Is there currently an animation running.
     *
     *        Is there currently an animation running.
     *
     * @return  true if there is an animation running.
     */
    virtual bool isZoomAnimationRunning() const;

    /**
     * @fn virtual uint16_t ZoomAnimationImage::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_ZOOMANIMATIONIMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_ZOOMANIMATIONIMAGE;
    }

protected:

    /**
     * @enum States
     *
     * @brief Animation states.
     */
    enum States
    {
        ANIMATE_ZOOM,          ///< Zoom animation state
        ANIMATE_ZOOM_AND_MOVE, ///< Zoom and move animation state
        NO_ANIMATION           ///< No animation state
    };

    States           currentState;                ///< The current animation state
    uint32_t         animationCounter;            ///< The progress counter for the animation
    uint16_t         zoomAnimationDelay;          ///< A delay that is applied before animation start. Expressed in ticks.
    touchgfx::Bitmap smallBmp;                    ///< The bitmap representing the small image
    touchgfx::Bitmap largeBmp;                    ///< The bitmap representing the large image
    touchgfx::Image  image;                       ///< The image for displaying the bitmap when the width/height is equal one of the bitmaps
    ScalableImage    scalableImage;               ///< The scalable image for displaying the bitmap when the width/height is not equal one of the bitmaps
    ZoomMode         currentZoomMode;             ///< The ZoomMode to use by the animation
    int16_t          zoomAnimationStartWidth;     ///< Width of the zoom animation start
    int16_t          zoomAnimationStartHeight;    ///< Height of the zoom animation start
    int16_t          zoomAnimationEndWidth;       ///< Width of the zoom animation end
    int16_t          zoomAnimationEndHeight;      ///< Height of the zoom animation end
    int16_t          zoomAnimationStartX;         ///< The zoom animation start x coordinate
    int16_t          zoomAnimationStartY;         ///< The zoom animation start y coordinate
    int16_t          zoomAnimationDeltaX;         ///< The zoom animation delta x
    int16_t          zoomAnimationDeltaY;         ///< The zoom animation delta y
    int16_t          moveAnimationEndX;           ///< The move animation end x coordinate
    int16_t          moveAnimationEndY;           ///< The move animation end y coordinate
    uint16_t         animationDuration;           ///< Duration of the animation
    EasingEquation   zoomAnimationWidthEquation;  ///< The zoom animation width equation
    EasingEquation   zoomAnimationHeightEquation; ///< The zoom animation height equation
    EasingEquation   moveAnimationXEquation;      ///< The move animation x coordinate equation
    EasingEquation   moveAnimationYEquation;      ///< The move animation y coordinate equation

    touchgfx::GenericCallback<const ZoomAnimationImage&>* animationEndedAction; ///< The animation ended action

    /**
     * @fn virtual void ZoomAnimationImage::updateRenderingMethod();
     *
     * @brief Chooses the optimal rendering of the image given the current width and height.
     *
     *        Chooses the optimal rendering of the image given the current width and height.
     */
    virtual void updateRenderingMethod();

    /**
     * @fn virtual void ZoomAnimationImage::setCurrentState(States state);
     *
     * @brief Sets the current animation state.
     *
     *        Sets the current animation state.
     *
     * @param state The new state.
     */
    virtual void setCurrentState(States state);

    /**
     * @fn void ZoomAnimationImage::startTimerAndSetParameters(int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode, EasingEquation widthProgressionEquation, EasingEquation heightProgressionEquation)
     *
     * @brief Starts timer and set parameters.
     *
     *        Starts timer and set parameters. Contains code shared between
     *        startZoomAnimation() and startZoomAndMoveAnimation(). If both delay and duration
     *        is zero, the end position and size is applied and the animation is ended.
     *
     * @param endWidth                  The end width.
     * @param endHeight                 The end height.
     * @param duration                  The duration.
     * @param zoomMode                  The zoom mode.
     * @param widthProgressionEquation  The width progression equation.
     * @param heightProgressionEquation The height progression equation.
     */
    void startTimerAndSetParameters(int16_t endWidth, int16_t endHeight, uint16_t duration, ZoomMode zoomMode, EasingEquation widthProgressionEquation, EasingEquation heightProgressionEquation);

    /**
     * @fn virtual void ZoomAnimationImage::updateZoomAnimationDeltaXY();
     *
     * @brief Calculates the change in X and Y caused by the zoom animation given the current
     *        ZoomMode.
     *
     *        Calculates the change in X and Y caused by the zoom animation given the current
     *        ZoomMode.
     */
    virtual void updateZoomAnimationDeltaXY();
};
} // namespace touchgfx
#endif // ZOOMANIMATIONIMAGE_HPP
