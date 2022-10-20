#ifndef TRANSMISSIVE
#define TRANSMISSIVE

#include "../core/vector3d.h"
#include "material.h"

class Transmissive : public Material
{
public:
    
    Transmissive();
    Transmissive(double index);

    Vector3D getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
    double getIndexOfRefraction() const;
    
    private:
        double ref_index;
};


#endif // TRANSMISSIVE
