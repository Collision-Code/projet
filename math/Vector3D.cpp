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
 * @file Vector3D.cpp
 *
 * Implements a Vector3D.
 */

#include "Vector3D.h"


Vector3D::Vector3D(double xx, double yy, double zz)
  : x(xx), y(yy), z(zz)
{
}

Vector3D::Vector3D(const Vector3D& vec)
  : x(vec.x), y(vec.y), z(vec.z)
{

}

Vector3D::~Vector3D()
{
}
