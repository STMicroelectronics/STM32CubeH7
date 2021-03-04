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

#ifndef CACHEABLECONTAINER_HPP
#define CACHEABLECONTAINER_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>

namespace touchgfx
{
/**
 * @class CacheableContainer CacheableContainer.hpp touchgfx/containers/CacheableContainer.hpp
 *
 * @brief A CacheableContainer is a Container that can have its drawing redirected into a Bitmap.
 *
 *        A CacheableContainer is a Drawable that can have child nodes. The z-order of children is
 *        determined by the order in which Drawables are added to the container - the Drawable
 *        added last will be front-most on the screen. A CacheableContainer can also render its
 *        content to a dynamic bitmap that could be used as a texture in subsequent drawing operations.
 *
 * @see Container
 * @see Bitmap
 */
class CacheableContainer : public Container
{
public:

    /**
     * @fn CacheableContainer::CacheableContainer()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    CacheableContainer() : Container()
    {
    }

    /**
     * @fn virtual CacheableContainer::~CacheableContainer()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~CacheableContainer() { }

    /**
     * @fn void CacheableContainer::setCacheBitmap(BitmapId bitmapId);
     *
     * @brief Set the dynamic bitmap into which the container content will be rendered.
     *
     *        Set the dynamic bitmap into which the container content will be rendered.
     *
     * @param bitmapId Id of the dynamic bitmap to serve as a render target.
     */
    void setCacheBitmap(BitmapId bitmapId);

    /**
     * @fn void CacheableContainer::updateCache();
     *
     * @brief Render the container into the attached dynamic bitmap.
     *
     *        Render the container into the attached dynamic bitmap.
     *
     */
    void updateCache();

    /**
     * @fn void CacheableContainer::updateCache(const Rect& rect);
     *
     * @brief Render the container into the attached dynamic bitmap.
     *
     *        Render the container into the attached dynamic bitmap. Only the specified Rect region is updated.
     *
     * @param rect Region to update.
     */
    void updateCache(const Rect& rect);

    /**
     * @fn void CacheableContainer::enableCachedMode(bool enable);
     *
     * @brief Toggle cached mode on and off.
     *
     *        Toggle cached mode on and off. The cacheable container behaves as a regular container when cached mode is off.
     *
     * @param enable Enable or disable cached mode.
     */
    void enableCachedMode(bool enable);

    /**
     * @fn virtual void CacheableContainer::invalidateRect(Rect& invalidatedArea) const;
     *
     * @brief Request that a subregion of this drawable is redrawn.
     *
     *        Request that a subregion of this drawable is redrawn. Will recursively traverse
     *        the tree towards the root, and once reached, issue a draw operation. When this
     *        function returns, the specified invalidated area has been redrawn for all
     *        appropriate Drawables covering the region.
     *
     * @param [in] invalidatedArea The area of this drawable to redraw expressed in coordinates
     *                             relative to its parent (e.g. to request a complete redraw,
     *                             invalidatedArea will be (0, 0, width, height).
     */
    virtual void invalidateRect(Rect& invalidatedArea) const;

    /**
     * @fn void CacheableContainer::isChildInvalidated() const;
     *
     * @brief Queries the CacheableContainer whether any child widget has been invalidated.
     *
     *        Queries the CacheableContainer whether any child widget has been invalidated.
     *
     * @return True if a child widget has been invalidated and false otherwise.
     */
    bool isChildInvalidated() const;

protected:
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);

    /**
     * @class CachedImage CacheableContainer.hpp touchgfx/containers/CacheableContainer.hpp
     *
     * @brief A CachedImage is a specialized Image object that exposes the setupDrawChain() method.
     *
     *        A CachedImage is a specialized Image object that exposes the setupDrawChain() method.
     *
     * @see CacheableContainer
     * @see Image
     */
    class CachedImage : public Image
    {
    public:
        /**
         * @fn CachedImage::CachedImage()
         *
         * @brief Default constructor.
         *
         *        Default constructor.
         */
        CachedImage() { }

        /**
         * @fn virtual CachedImage::~CachedImage()
         *
         * @brief Destructor.
         *
         *        Destructor.
         */
        virtual ~CachedImage() { }

        void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
        {
            Drawable::setupDrawChain(invalidatedArea, nextPreviousElement);
        }
    };

private:
    BitmapId cachedBitmapId;  ///< The BitmapId of the dynamic bitmap attached to the CacheableContainer
    CachedImage cachedImage;  ///< The CachedImage object used as a wrapper widget around the attached dynamic bitmap
    bool isCachedMode;        ///< Cached mode whether enabled or disabled
    bool childWasInvalidated; ///< A child widget has been invalidated. The flag is meaningful when isCachedMode is true.
};
} // namespace touchgfx
#endif // CACHEABLECONTAINER_HPP
