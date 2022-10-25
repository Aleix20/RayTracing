
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
		
		//Check if Specular material (i.e, mirror-like)
		if (its.shape->getMaterial().hasSpecular()) {
			//We compute the perfect reflection ray
			Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
			Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);

			//Compute the color for the mirror material adding 1 to the depth
			color = computeColor(reflectionRay, objList, lsList);
		}

		//Check if Transmissive material
		if (its.shape->getMaterial().hasTransmission()) {

			//Index of the scene mediums
			double nt = its.shape->getMaterial().getIndexOfRefraction();

			if (dot(r.d, its.normal) > 0) { //We check if we are inside the material, if so, we invert the normal and the index of the scene mediums
				its.normal = -its.normal;
				nt = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}

			//Compute the parts of the transmissive vector
			double cos_alpha = dot(its.normal, -r.d);
			double sin2_alpha = 1 - std::pow(cos_alpha, 2);
			double radical = 1 - std::pow(nt, 2) * sin2_alpha; //Get indexOfRefraction ja agafa el ratio

			//If the sqrt is possitive then we can treat the material as transmissive
			if (radical >= 0) {

				double first = -std::sqrt(radical);
				double second = nt * dot(-r.d, its.normal);
				Vector3D third = its.normal * (first + second);

				Vector3D wt = third - (-r.d) * nt;

				Ray refracRay = Ray(its.itsPoint, wt, r.depth + 1);
				color = computeColor(refracRay, objList, lsList);

			}
			//If not, then the material will be like a perfect specular material
			else {
				//Same computations as specular material
				Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
			}
		}
		
		//Check if Phong Material
		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			//For each one of the lights in the scene we compute the perfect reflection ray
			for (auto const& light : lsList) {
				Vector3D P_L = light.getPosition() - its.itsPoint; //Vector from intersection point to lightsource
				Vector3D wi = P_L.normalized(); //Normalized Vector wi
				Ray ray_visibility(its.itsPoint, wi, 0, Epsilon, P_L.length());
				
				if (Utils::hasIntersection(ray_visibility, objList))
					continue;
				//If there is not object between the point and the light, we can compute the refleted light in that point
				Lo += light.getIntensity(its.itsPoint) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wi) * dot(its.normal, wi);
			}

			color = Lo;
		}

		
		return color;
	}
	else {

		return bgColor;
	}
}
