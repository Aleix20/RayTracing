#include "phong.h"

#include <iostream>

// Implement the Constructors here
Phong::Phong(Vector3D kd, Vector3D ks, int shine)
{
	this->kd = kd;
	this->ks = ks;
	this->shine = shine;
}

bool Phong::hasSpecular() const
{
	// This material does not have specular component
	return false;
}

bool Phong::hasTransmission() const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}

bool Phong::hasScatter() const
{
	return false;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{
	//Due to the pre - established operands in the vector class, we had to modify the order of the formula	
	//Compute ideal reflection direction
	Vector3D wr = n*2*dot(n,wi) - wi;
	//Compute the reflectance of a phong material
	Vector3D r = kd * std::max(dot(wi, n),0.0) + ks * pow(dot(wo, wr), shine);
	return r;
}

Vector3D Phong::getDiffuseCoefficient() const
{
	return kd;
}

