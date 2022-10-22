
#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader()
{ }

DirectShader::DirectShader(Vector3D bgColor_) :
	Shader(bgColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	//Check the closest intersection with all the object of the scene
	// Get the closest Intersection along the ray
	Intersection its; // Auxiliar structure to store information about the intersection, in case there is one
	Vector3D color;
	if (Utils::getClosestIntersection(r, objList, its))
	{
		Vector3D Lo(0.0);
		//Check if Phong Material
		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			for (auto const& light : lsList) {
				Vector3D P_L = light.getPosition() - its.itsPoint; //Vector from intersection point to lightsource
				Vector3D wi = P_L.normalized(); //Normalized Vector wi
				Ray ray_visibility(its.itsPoint, wi, 0, Epsilon, P_L.length());
				if (Utils::hasIntersection(ray_visibility, objList))
					continue;
				Lo += light.getIntensity(its.itsPoint) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wi) * dot(its.normal, wi);
			}
			
			color = Lo;
		}
		


		if (its.shape->getMaterial().hasSpecular()) {

			Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
			Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
			color = computeColor(reflectionRay, objList, lsList);
		}

		if (its.shape->getMaterial().hasTransmission()) {

			double nt = its.shape->getMaterial().getIndexOfRefraction();

			if (dot(r.d, its.normal) > 0) { //We check if we are inside the material, if so, we invert things
				its.normal = -its.normal;
				nt = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}


			double cos_alpha = dot(its.normal, -r.d);
			double sin2_alpha = 1 - std::pow(cos_alpha, 2);
			double radical = 1 - std::pow(nt, 2) * sin2_alpha; //Get indexOfRefraction ja agafa el ratio

			if (radical >= 0) {

				double first = -std::sqrt(radical);
				double second = nt * dot(-r.d, its.normal);
				Vector3D third = its.normal * (first + second);

				Vector3D wt = third - (-r.d) * nt;

				Ray refracRay = Ray(its.itsPoint, wt, r.depth + 1);
				color = computeColor(refracRay, objList, lsList);

			}
			else {
				Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
			}

		}


		return color;
	}
	else {

		return bgColor;
	}
}
