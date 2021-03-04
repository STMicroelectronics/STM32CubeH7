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

#ifndef MATH3D_HPP
#define MATH3D_HPP

namespace touchgfx
{
static const float PI = 3.14159265358979323846f;    ///< PI

class Point4;
class Vector4;
class Matrix4x4;

/**
 * @fn Matrix4x4 operator*(const Matrix4x4& multiplicand, const Matrix4x4& multiplier);
 *
 * @brief Multiplication operator.
 *
 *        Multiplication operator.
 *
 * @param multiplicand The first value to multiply.
 * @param multiplier   The second value to multiply.
 *
 * @return The result of the operation.
 */
Matrix4x4 operator*(const Matrix4x4& multiplicand, const Matrix4x4& multiplier);

/**
 * @fn Point4 operator*(const Matrix4x4& multiplicand, const Point4& multiplier);
 *
 * @brief Multiplication operator.
 *
 *        Multiplication operator.
 *
 * @param multiplicand The first value to multiply.
 * @param multiplier   The second value to multiply.
 *
 * @return The result of the operation.
 */
Point4 operator*(const Matrix4x4& multiplicand, const Point4& multiplier);

/**
 * @class Quadruple Math3D.hpp touchgfx/Math3D.hpp
 *
 * @brief Base class for homogeneous vectors and points.
 *
 *        Base class for homogeneous vectors and points.
 */
class Quadruple
{
public:

    /**
     * @fn inline float Quadruple::getElement(int row) const
     *
     * @brief Gets an element.
     *
     *        Gets an element.
     *
     * @param row The row.
     *
     * @return The element.
     */
    inline float getElement(int row) const
    {
        return elements[row];
    }

    /**
     * @fn inline float Quadruple::getX() const
     *
     * @brief Get x coordinate.
     *
     *        Get x coordinate.
     *
     * @return The x coordinate.
     */
    inline float getX() const
    {
        return elements[0];
    }

    /**
     * @fn inline float Quadruple::getY() const
     *
     * @brief Get y coordinate.
     *
     *        Get y coordinate.
     *
     * @return The y coordinate.
     */
    inline float getY() const
    {
        return elements[1];
    }

    /**
     * @fn inline float Quadruple::getZ() const
     *
     * @brief Get z coordinate.
     *
     *        Get z coordinate.
     *
     * @return The z coordinate.
     */
    inline float getZ() const
    {
        return elements[2];
    }

    /**
     * @fn inline float Quadruple::getW() const
     *
     * @brief Gets the w.
     *
     *        Gets the w.
     *
     * @return The w.
     */
    inline float getW() const
    {
        return elements[3];
    }

    /**
     * @fn inline void Quadruple::setElement(int row, float value)
     *
     * @brief Sets an element.
     *
     *        Sets an element.
     *
     * @param row   The row.
     * @param value The value.
     */
    inline void setElement(int row, float value)
    {
        elements[row] = value;
    }

    /**
     * @fn inline void Quadruple::setX(float value)
     *
     * @brief Sets an x coordinate.
     *
     *        Sets an x coordinate.
     *
     * @param value The value.
     */
    inline void setX(float value)
    {
        elements[0] = value;
    }

    /**
     * @fn inline void Quadruple::setY(float value)
     *
     * @brief Sets a y coordinate.
     *
     *        Sets a y coordinate.
     *
     * @param value The value.
     */
    inline void setY(float value)
    {
        elements[1] = value;
    }

    /**
     * @fn inline void Quadruple::setZ(float value)
     *
     * @brief Sets a z coordinate.
     *
     *        Sets a z coordinate.
     *
     * @param value The value.
     */
    inline void setZ(float value)
    {
        elements[2] = value;
    }

    /**
     * @fn inline void Quadruple::setW(float value)
     *
     * @brief Sets a w.
     *
     *        Sets a w.
     *
     * @param value The value.
     */
    inline void setW(float value)
    {
        elements[3] = value;
    }

protected:

    /**
     * @fn inline Quadruple::Quadruple()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    inline Quadruple()
    {
        elements[0] = elements[1] = elements[2] = elements[3] = 0;  ///< The elements[ 3]
    };

    /**
     * @fn inline Quadruple::Quadruple(float x, float y, float z, float w)
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     * @param w The w value.
     */
    inline Quadruple(float x, float y, float z, float w)
    {
        elements[0] = x;
        elements[1] = y;
        elements[2] = z;
        elements[3] = w;
    }

    float elements[4];  ///< The elements[4]
};

/**
 * @class Point4 Math3D.hpp touchgfx/Math3D.hpp
 *
 * @brief This class represents a homogeneous 3D point.
 *
 *        This class represents a homogeneous 3D point.
 *
 * @see quadruple
 */
class Point4 : public Quadruple
{
public:

    /**
     * @fn inline Point4::Point4()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    inline Point4() : Quadruple(0, 0, 0, 1) {}

    /**
     * @fn inline Point4::Point4(float x, float y, float z)
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    inline Point4(float x, float y, float z) : Quadruple(x, y, z, 1) {}
};

/**
 * @class Vector4 Math3D.hpp touchgfx/Math3D.hpp
 *
 * @brief This class represents a homogeneous 3D vector.
 *
 *        This class represents a homogeneous 3D vector.
 *
 * @see quadruple
 */
class Vector4 : public Quadruple
{
public:

    /**
     * @fn inline Vector4::Vector4()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    inline Vector4() : Quadruple(0, 0, 0, 0) { }

    /**
     * @fn inline Vector4::Vector4(float x, float y, float z)
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    inline Vector4(float x, float y, float z) : Quadruple(x, y, z, 0) {}

    /**
     * @fn inline Vector4 Vector4::crossProduct(const Vector4& operand)
     *
     * @brief Cross product.
     *
     *        Cross product.
     *
     * @param operand The second operand.
     *
     * @return The result of the operation.
     */
    inline Vector4 crossProduct(const Vector4& operand)
    {
        float X = getY() * operand.getZ() - getZ() * operand.getY();
        float Y = getZ() * operand.getX() - getX() * operand.getZ();
        float Z = getX() * operand.getY() - getY() * operand.getX();

        return Vector4(X, Y, Z);
    }
};

/**
 * @class Matrix4x4 Math3D.hpp touchgfx/Math3D.hpp
 *
 * @brief This class represents row major 4x4 homogeneous matrices.
 *
 *        This class represents row major 4x4 homogeneous matrices.
 */
class Matrix4x4
{
public:

    /**
     * @fn Matrix4x4::Matrix4x4();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Matrix4x4();

    /**
     * @fn inline float Matrix4x4::getElement(int row, int column) const
     *
     * @brief Gets an element.
     *
     *        Gets an element.
     *
     * @param row    The row.
     * @param column The column.
     *
     * @return The element.
     */
    inline float getElement(int row, int column) const
    {
        return elements[row][column];
    }

    /**
     * @fn void Matrix4x4::setViewDistance(float distance);
     *
     * @brief Sets view distance.
     *
     *        Sets view distance.
     *
     * @param distance The distance.
     */
    void setViewDistance(float distance);

    /**
     * @fn inline Matrix4x4 Matrix4x4::setElement(int row, int column, float value)
     *
     * @brief Sets an element.
     *
     *        Sets an element.
     *
     * @param row    The row.
     * @param column The column.
     * @param value  The value.
     *
     * @return A matrix_4x4&amp;
     */
    inline Matrix4x4 setElement(int row, int column, float value)
    {
        elements[row][column] = value;

        return *this;
    }

    /**
     * @fn static Matrix4x4 Matrix4x4::identity()
     *
     * @brief Gets the identity.
     *
     *        Gets the identity. Instead of using "Matrix4x4::identity()"
     *        consider using "Matrix4x4()" instead.
     *
     * @return A matrix_4x4.
     */
    static Matrix4x4 identity()
    {
        return Matrix4x4();
    }

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateXRotation(float radians);
     *
     * @brief Concatenate x coordinate rotation.
     *
     *        Concatenate x coordinate rotation.
     *
     * @param radians The radians.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateXRotation(float radians);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateYRotation(float radians);
     *
     * @brief Concatenate y coordinate rotation.
     *
     *        Concatenate y coordinate rotation.
     *
     * @param radians The radians.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateYRotation(float radians);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateZRotation(float radians);
     *
     * @brief Concatenate z coordinate rotation.
     *
     *        Concatenate z coordinate rotation.
     *
     * @param radians The radians.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateZRotation(float radians);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateXTranslation(float distance);
     *
     * @brief Concatenate x coordinate translation.
     *
     *        Concatenate x coordinate translation.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateXTranslation(float distance);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateYTranslation(float distance);
     *
     * @brief Concatenate y coordinate translation.
     *
     *        Concatenate y coordinate translation.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateYTranslation(float distance);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateZTranslation(float distance);
     *
     * @brief Concatenate z coordinate translation.
     *
     *        Concatenate z coordinate translation.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateZTranslation(float distance);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateXScale(float distance);
     *
     * @brief Concatenate x coordinate scale.
     *
     *        Concatenate x coordinate scale.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateXScale(float distance);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateYScale(float distance);
     *
     * @brief Concatenate y coordinate scale.
     *
     *        Concatenate y coordinate scale.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateYScale(float distance);

    /**
     * @fn Matrix4x4& Matrix4x4::concatenateZScale(float distance);
     *
     * @brief Concatenate z coordinate scale.
     *
     *        Concatenate z coordinate scale.
     *
     * @param distance The distance.
     *
     * @return A matrix_4x4&amp;
     */
    Matrix4x4& concatenateZScale(float distance);

protected:

    float elements[4][4];   ///< The elements[4][4]
};
}

#endif // MATH3D_HPP
