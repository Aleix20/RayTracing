#ifndef PHONG
#define PHONG

#include "../core/vector3d.h"
#include "material.h"

class Phong: public Material
{
public:
    Phong(Vector3D kd, Vector3D ks, int shine);

	bool Phong::hasSpecular() const;
	bool Phong::hasTransmission() const;
	bool Phong::hasScatter() const;
	bool Phong::hasDiffuseOrGlossy() const;
	virtual Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;
	virtual Vector3D getDiffuseCoefficient() const;

private:
	Vector3D kd;
	Vector3D ks;
	int shine;
};


#endif // PHONG
