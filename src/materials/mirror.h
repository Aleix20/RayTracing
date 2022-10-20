#ifndef MIRROR
#define MIRROR

#include "../core/vector3d.h"
#include "material.h"

class Mirror : public Material
{
public:
    Mirror();

    Vector3D getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
};


#endif // MIRROR
