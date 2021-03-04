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

#ifndef ABSTRACTPARTITION_HPP
#define ABSTRACTPARTITION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class AbstractPartition AbstractPartition.hpp touchgfx/common/AbstractPartition.hpp
 *
 * @brief This type defines an abstract interface to a storage partition for allocating memory
 *        slots of equal size.
 *
 *        This type defines an abstract interface to a storage partition for allocating memory
 *        slots of equal size. The "partition" is not aware of the actual types stored in the
 *        partition memory, hence it provides no mechanism for deleting C++ objects when
 *        clear()'ed.
 */
class AbstractPartition
{
public:

    /**
     * @fn virtual AbstractPartition::~AbstractPartition();
     *
     * @brief Virtual destructor.
     *
     *        Virtual destructor.
     */
    virtual ~AbstractPartition();

    /**
     * @fn virtual void* AbstractPartition::allocate(uint16_t size);
     *
     * @brief Gets the address of the next available storage slot.
     *
     *        Gets the address of the next available storage slot. The slot size is compared
     *        with the specified size.
     *
     * @note Asserts if 'size' is too large, or the storage is depleted.
     *
     * @param size The size.
     *
     * @return The address of an empty storage slot which contains minimum 'size' bytes.
     */
    virtual void* allocate(uint16_t size);

    /**
     * @fn virtual void* AbstractPartition::allocateAt(uint16_t index, uint16_t size);
     *
     * @brief Gets the address of the specified index.
     *
     *        Gets the address of the specified index.
     *
     * @note Asserts if 'size' is too large.
     *
     * @param index Zero-based index of the.
     * @param size  The size.
     *
     * @return The address of the appropriate storage slot which contains minimum 'size' bytes.
     */
    virtual void* allocateAt(uint16_t index, uint16_t size);

    /**
     * @fn virtual uint16_t AbstractPartition::getAllocationCount() const;
     *
     * @brief Gets allocation count.
     *
     *        Gets allocation count.
     *
     * @return The currently allocated storage slots.
     */
    virtual uint16_t getAllocationCount() const;

    /**
     * @fn virtual uint16_t AbstractPartition::indexOf(const void* address);
     *
     * @brief Determines index of previously allocated location.
     *
     *        Determines index of previously allocated location. Since the Partition concept is
     *        loosely typed this method shall be used with care. The method does not guarantee
     *        that the found object at the returned index is a valid object. It only tests
     *        whether or not the object is within the bounds of the current partition
     *        allocations.
     *
     * @param address The location address to lookup.
     *
     * @return An uint16_t.
     */
    virtual uint16_t indexOf(const void* address);

    /**
     * @fn virtual void AbstractPartition::clear();
     *
     * @brief Prepares the Partition for new allocations.
     *
     *        Prepares the Partition for new allocations. Any objects present in the Partition
     *        shall not be used after invoking this method.
     */
    virtual void clear();

    /**
     * @fn virtual uint16_t AbstractPartition::capacity() const = 0;
     *
     * @brief Gets the capacity, i.e. the maximum allocation count.
     *
     *        Gets the capacity, i.e. the maximum allocation count.
     *
     * @return The maximum allocation count.
     */
    virtual uint16_t capacity() const = 0;

    /**
     * @fn template< typename T > void* AbstractPartition::allocate()
     *
     * @brief Gets the address of the next available storage slot.
     *
     *        Gets the address of the next available storage slot. The slot size is determined
     *        from the size of type T.
     *
     * @note Asserts if T is too large, or the storage is depleted.
     *
     * @tparam T Generic type parameter.
     *
     * @return The address of an empty storage slot.
     */
    template< typename T >
    void* allocate()
    {
        return allocate(static_cast<uint16_t>(sizeof(T)));
    }

    /**
     * @fn template< typename T > void* AbstractPartition::allocateAt(uint16_t index)
     *
     * @brief Gets the address of the specified storage slot.
     *
     *        Gets the address of the specified storage slot. The slot size is determined from
     *        the size of type T.
     *
     * @note Asserts if T is too large.
     *
     * @tparam T Generic type parameter.
     * @param index Zero-based index of the.
     *
     * @return The address of the appropriate storage slot.
     */
    template< typename T >
    void* allocateAt(uint16_t index)
    {
        return allocateAt(index, static_cast<uint16_t>(sizeof(T)));
    }

    /**
     * @fn template< typename T > T& AbstractPartition::at(const uint16_t index)
     *
     * @brief Gets the object at the specified index.
     *
     *        Gets the object at the specified index.
     *
     * @tparam T Generic type parameter.
     * @param index The index into the Partition storage where the returned object is located.
     *
     * @return A typed reference to the object at the specified index.
     */
    template< typename T >
    T& at(const uint16_t index)
    {
        return *static_cast<T*>(element(index));
    }

    /**
     * @fn template< typename T > const T& AbstractPartition::at(const uint16_t index) const
     *
     * @brief const version of at().
     *
     *        const version of at().
     *
     * @tparam T Generic type parameter.
     * @param index Zero-based index of the.
     *
     * @return A T&amp;
     */
    template< typename T >
    const T& at(const uint16_t index) const
    {
        return *static_cast<const T*>(element(index));
    }

    /**
     * @fn template< class T > Pair< T*, uint16_t > AbstractPartition::find(const void* pT)
     *
     * @brief Determines if the specified object could have been previously allocated in the
     *        partition.
     *
     *        Determines if the specified object could have been previously allocated in the
     *        partition. Since the Partition concept is loosely typed this method shall be used
     *        with care. The method does not guarantee that the found object at the returned
     *        index is a valid object. It only tests whether or not the object is within the
     *        bounds of the current partition allocations.
     *
     * @tparam T Generic type parameter.
     * @param pT Pointer to the object to lookup.
     *
     * @return If the object seems to be allocated in the Partition, a Pair object containing a
     *         typed pointer to the object and an index into the Partition storage is returned.
     *         Otherwise, a Pair&lt; 0, 0 &gt; is returned.
     */
    template< class T >
    Pair< T*, uint16_t > find(const void* pT)
    {
        uint16_t index = indexOf(pT);
        if (0 < getAllocationCount() && index < getAllocationCount())
        {
            return Pair<T*, uint16_t>(&at<T>(index), index);
        }

        return Pair<T*, uint16_t>(0, (uint16_t) -1);
    }

    /**
     * @fn void AbstractPartition::dec()
     *
     * @brief Decreases number of allocations.
     *
     *        Decreases number of allocations.
     */
    void dec()
    {
        if (allocations)
        {
            allocations--;
        }
    }

    /**
     * @fn virtual uint32_t AbstractPartition::element_size() = 0;
     *
     * @brief Access to concrete element-size. Used internally.
     *
     *        Access to concrete element-size. Used internally.
     *
     * @return An uint32_t.
     */
    virtual uint32_t element_size() = 0;

protected:

    /**
     * @fn virtual void* AbstractPartition::element(uint16_t index) = 0;
     *
     * @brief Access to stored element. Used internally.
     *
     *        Access to stored element. Used internally.
     *
     * @param index Zero-based index of the.
     *
     * @return null if it fails, else a void*.
     */
    virtual void* element(uint16_t index) = 0;

    /**
     * @fn virtual const void* AbstractPartition::element(uint16_t index) const = 0;
     *
     * @brief Access to stored element, const version.
     *
     *        Access to stored element, const version.
     *
     * @param index Zero-based index of the.
     *
     * @return null if it fails, else a void*.
     */
    virtual const void* element(uint16_t index) const = 0;

    /**
     * @fn AbstractPartition::AbstractPartition();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AbstractPartition();

private:
    uint16_t allocations;
};
} // namespace touchgfx
#endif // ABSTRACTPARTITION_HPP
