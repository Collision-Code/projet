/*
 * Collision-Code
 * Free software to calculate collision cross-section with Helium.
 * Université de Rouen
 * 2016
 *
 * Anthony BREANT
 * Clement POINSOT
 * Jeremie PANTIN
 * Mohamed TAKHTOUKH
 * Thomas CAPET
 */

/**
 * \file Vector3D.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way to save a vector of three variables of type "double".
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>

class Vector3D {
  public:

    /**
     * Create a coordinate with three points specified in input.
     * \param x the value on the X axis.
     * \param y the value on the Y axis.
     * \param z the value on the Z axis.
     */
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);
    /**
     * Copy constructor.
     */
    Vector3D(const Vector3D& vec);

    /**
     * Release allocates resources.
     */
    ~Vector3D();

    // Operators
    /**
     * Overload of the << operator.
     */
    friend std::ostream& operator << (std::ostream& out, const Vector3D& vec);
    /**
     * Overloading of the assignment operator.
     */
    inline void operator = (const Vector3D& vec)
    {
      x = vec.x;
      y = vec.y;
      z = vec.z;
    }
    /**
     * Overloading of the equality operator.
     */
    inline bool operator == (const Vector3D& vec) const
    {
      return x == vec.x
        && y == vec.y
        && z == vec.z;
    }

  public:
    /**
     * The value on the X axis.
     */
    double x;
    /**
     * The value on the Y axis.
     */
    double y;
    /**
     * The value on the Z axis.
     */
    double z;
};

inline std::ostream& operator << (std::ostream& out, const Vector3D& vec)
{
    out << "(" << vec.x << ", " <<
        vec.y << ", " <<
        vec.z << ")";
    return out;
}

#endif
