
#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"



GlobalShader::GlobalShader()
{ }

GlobalShader::GlobalShader(Vector3D bgColor_) :
	Shader(bgColor_)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	//Check the closest intersection with all the object of the scene
	// Get the closest Intersection along the ray
	Intersection its; // Auxiliar structure to store information about the intersection, in case there is one
	Vector3D color;
	Vector3D a_t = Vector3D(0.1);

	if (Utils::getClosestIntersection(r, objList, its))
	{
		Vector3D Lo(0.0);
		Vector3D Lo_ind;
		//Check if Phong Material
		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			
			if (r.depth == 0) {

				int nSamples = 10; //How to define the nSamples value?
				HemisphericalSampler sampler = HemisphericalSampler();
				
				for (int j = 0; j < nSamples; j++) {
					
					//Check this method (Eq.4)
					
					Vector3D wj = sampler.getSample(its.normal);
					Ray incidentRay = Ray(its.itsPoint, wj, r.depth+1); //r.depth +1 or not?
					Lo_ind += computeColor(incidentRay, objList, lsList) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wj) * dot(its.normal, wj) * (1 / (2 * M_PI * nSamples));
				}
				
				color = Lo + Lo_ind;
			}
			
			else if (r.depth > 0) {
				
				for (auto const& light : lsList) {
					Vector3D P_L = light.getPosition() - its.itsPoint; //Vector from intersection point to lightsource
					Vector3D wi = P_L.normalized(); //Normalized Vector wi
					Ray ray_visibility(its.itsPoint, wi, 0, Epsilon, P_L.length());
					if (Utils::hasIntersection(ray_visibility, objList))
						continue;
					Lo += light.getIntensity(its.itsPoint) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wi) * dot(its.normal, wi);
				}
				
				Lo_ind = its.shape->getMaterial().getDiffuseCoefficient() * a_t;
				color = Lo_ind + Lo; //Check if this line goes here
			}

		}
		
		
		if (its.shape->getMaterial().hasSpecular()) {

			Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
			Ray reflectionRay = Ray(its.itsPoint, wr, r.depth+1); //r.depth +1 or not?
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

				Ray refracRay = Ray(its.itsPoint, wt, r.depth+1); //r.depth +1 or not?
				color = computeColor(refracRay, objList, lsList);
				
			}
			else {
				Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray reflectionRay = Ray(its.itsPoint, wr, r.depth+1); //r.depth +1 or not?
				color = computeColor(reflectionRay, objList, lsList);
			}

		}

		return color;
	
	}else{

		return bgColor;
	}
	
}
