#include "transmissive.h"

#include <iostream>

Transmissive::Transmissive()
{ }

Transmissive::Transmissive(double index1):
    ref_index(index1)
{}

double Transmissive::getIndexOfRefraction() const
{
    return ref_index;
}

bool Transmissive::hasSpecular() const
{
    return false;
}

bool Transmissive::hasTransmission() const
{
    return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
    return false;
}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {

    return Vector3D();
}
