#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"


GlobalShader::GlobalShader()
{ }

GlobalShader::GlobalShader(Vector3D bgColor_) :
	Shader(bgColor_)
{}

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	Vector3D lo_dir = Vector3D(0.0);
	Vector3D lo_ind = Vector3D(0.0);
	Vector3D color = Vector3D(0.0);


	//Check the closest intersection with all the object of the scene
	if (Utils::getClosestIntersection(r, objList, its)) {


		//Check if the material is a Specular material (i.e, mirror-like)

		if (its.shape->getMaterial().hasSpecular()) {

			Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
			Ray reflectionRay = Ray(its.itsPoint, wr, r.depth); 

			color = computeColor(reflectionRay, objList, lsList); 
		}


		//Check if the material is transmissive material

		if (its.shape->getMaterial().hasTransmission()) {

			double nt = its.shape->getMaterial().getIndexOfRefraction(); //Get indexOfRefraction gets the nt (i.e, the ratio between mediums)

			if (dot(r.d, its.normal) > 0) { //We check if we are inside the material, if so, we invert the normal and the Refraction index
				its.normal = -its.normal;
				nt = 1 / nt;
			}

			//Compute the radical
			//Compute the parts of the transmissive vector

			double cos_alpha = dot(its.normal, -r.d);
			double sin2_alpha = 1 - std::pow(cos_alpha, 2);
			double radical = 1 - std::pow(nt, 2) * sin2_alpha;
			//If the sqrt is possitive then we can treat the material as transmissive
			if (radical >= 0) {

				double first = -std::sqrt(radical);
				double second = nt * dot(-r.d, its.normal);
				Vector3D third = its.normal * (first + second);

				Vector3D wt = third - (-r.d) * nt;

				Ray refracRay = Ray(its.itsPoint, wt, r.depth); 
				color = computeColor(refracRay, objList, lsList);

			}
			else { //Compute like a specular material (mirror-like)

				Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray reflectionRay = Ray(its.itsPoint, wr, r.depth); 
				color = computeColor(reflectionRay, objList, lsList); 
			}
		}


		//Check if the material is a Phong material

		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {

			if (r.depth == 0) {	//First intersection with a phong material
				//Throw nRays from the point to random directions(like a hemisphere)
				for (int j = 0; j < nSamples; ++j) { //Indirect light at output

					HemisphericalSampler sample;
					Vector3D wj = sample.getSample(its.normal);
					Ray secondaryRay = Ray(its.itsPoint, wj, r.depth + 1); //random ray adding one to the depth

					//Compute the indirect light in a recursive way
					lo_ind += computeColor(secondaryRay, objList, lsList) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wj) ;
				}
				//Aproximation/average of all the indirect lights
				lo_ind = lo_ind * (1.0 / (2.0 * M_PI * nSamples));
			}

			else if (r.depth == 2) { //Case in which r.depth == maxDepth
				//Aproximation of indirect light using ambient term
				lo_ind = ambientTerm * its.shape->getMaterial().getDiffuseCoefficient();
			}

			else {
				//If we have n_bounces the we do another aproximation with the normal and the reflected ray
				Vector3D w_r = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray wn = Ray(its.itsPoint, its.normal, r.depth + 1);
				Ray wr = Ray(its.itsPoint, w_r, r.depth + 1);

				//Aproximation using the normal and the perfect reflected ray
				lo_ind = (computeColor(wn, objList, lsList) * its.shape->getMaterial().getReflectance(its.normal, -r.d, its.normal)
					+
					computeColor(wr, objList, lsList) * its.shape->getMaterial().getReflectance(its.normal, -r.d, w_r))
					*
					(1.0 / (4.0 * M_PI));
			}



			//Check of the lightsources of the scene
			//For each one of the lights in the scene we compute the perfect reflection ray
			for (auto const& light : lsList) {
				Vector3D P_L = light.getPosition() - its.itsPoint; //Vector from intersection point to lightsource
				Vector3D wi = P_L.normalized(); //Normalized Vector wi
				Ray ray_visibility(its.itsPoint, wi, 0, Epsilon, P_L.length());

				if (Utils::hasIntersection(ray_visibility, objList))
					continue;
				//If there is not object between the point and the light, we can compute the refleted light in that point
				lo_dir += light.getIntensity(its.itsPoint) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wi) * dot(its.normal, wi);
			}
			//Total light will be the addition of direct light and indirect
			//We multiply the indirect light by 5.0 to get a more visible result
			color = lo_ind*5.0 + lo_dir;
		}


		return color;


	}
	else {

		return bgColor;
	}

}
