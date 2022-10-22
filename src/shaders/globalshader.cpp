
#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"

GlobalShader::GlobalShader()
{ }

GlobalShader::GlobalShader(Vector3D bgColor_, Vector3D ambientTerm_) :
	Shader(bgColor_)
{ 
	this->ambientTerm = ambientTerm;
}

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection its;
	
	Vector3D l_o_ind;
	Vector3D l_i_ind;
	//Check the closest intersection with all the object of the scene
	if (Utils::getClosestIntersection(r, objList, its)) {
	
		Vector3D color;
		if (its.shape->getMaterial().hasSpecular()) {
			Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
			Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
			if (r.depth == 0)
				color += computeColor(reflectionRay, objList, lsList);
			if (r.depth == n_bounces)
				l_o_ind += computeColor(reflectionRay, objList, lsList);

	
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
				color += computeColor(refracRay, objList, lsList);
				//color = Vector3D(1, 0, 0);
			}
			else {
				Vector3D wr = its.normal * 2 * (dot(-r.d, its.normal)) + r.d;
				Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
				color += computeColor(reflectionRay, objList, lsList);
			}

		}

		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			if (r.depth == 0)	//First Bounce
			{

				for (int j = 0; j < n_directions; ++j) {		//Indirect light at output

					Vector3D origen = its.itsPoint;

					HemisphericalSampler sample;
					Vector3D wj = sample.getSample(its.normal);
					Ray r_bounce = Ray(origen, wj, r.depth + 1, 0.01);

					if (Utils::hasIntersection(r_bounce, objList))	// NEXT BOUNCE
					{
						Vector3D n = its.normal;
						Vector3D wo = (r.o - its.itsPoint).normalized();
						Vector3D l_i_ind = computeColor(r_bounce, objList, lsList);
						Vector3D r = its.shape->getMaterial().getReflectance(n, wo, wj);
						l_o_ind += Utils::multiplyPerCanal(l_i_ind, r);
					}
					
				}
				l_o_ind = l_o_ind * 1 / (2 * 3.1417 * n_directions);

				
			}
			else {
				l_o_ind = ambientTerm * its.shape->getMaterial().getDiffuseCoefficient();
			}

			
			
			Vector3D interesectionPoint = its.itsPoint;
			Vector3D wo = (r.o - interesectionPoint).normalized();
			

			//Check of the lightsources of the scene
			for (int s = 0; s < lsList.size(); s++)
			{
				PointLightSource actualSource_s = lsList.at(s);

				Vector3D actualSourcePos = actualSource_s.getPosition();
				//Vector to the lightsource
				Vector3D wi = (actualSourcePos - interesectionPoint);
				double maxT = std::sqrt(std::pow(wi.x, 2) + std::pow(wi.y, 2) + std::pow(wi.z, 2));
				//Ray to check if there are any object between the light and the hit point
				Ray wiTest = Ray(interesectionPoint, wi.normalized(), 0, Epsilon, maxT);
				//Check if there is any object between the light and the intersection point
				if (!Utils::hasIntersection(wiTest, objList)) {
					//Compute the phong color
					
					Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, wo, wi.normalized());
					color += actualSource_s.getIntensity(interesectionPoint) * reflectance;
				}
			}
			color += l_o_ind;
		}

		return color;


	}
	else {

		return bgColor;
	}
	
}
