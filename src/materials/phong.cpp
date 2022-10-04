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
	//Degut als operand pre establerts a la classe vector, hem hagut de modificar l'ordre de la formula
	Vector3D wr = n*2*dot(n,wi) - wi;
	//Vector3D r = kd*std::max(dot(wi,n),0.001) + ks * pow(std::max(dot(wo, wr), 0.0001), shine);
	Vector3D r = kd * dot(wi, n) + ks * pow(dot(wo, wr), shine);
	//Vector3D r = kd*dot(wi,n);
	return r;
}

Vector3D Phong::getDiffuseCoefficient() const
{
	return kd;
}

