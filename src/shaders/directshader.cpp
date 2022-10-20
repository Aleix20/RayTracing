
#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader()
{ }

DirectShader::DirectShader(Vector3D bgColor_) :
	Shader(bgColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection* its = new Intersection();
	//Check the closest intersection with all the object of the scene
	if (Utils::getClosestIntersection(r, objList, *its)) {
	
		Vector3D color;
		
		if (its->shape->getMaterial().hasDiffuseOrGlossy()) {
			
			Vector3D interesectionPoint = its->itsPoint;
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
					Vector3D reflectance = its->shape->getMaterial().getReflectance(its->normal, wo, wi.normalized());
					color += actualSource_s.getIntensity(interesectionPoint) * reflectance* dot(its->normal, -r.d);
				}
			}
			Vector3D ambientTerm = Vector3D(0.2, 0.2, 0.2);
			color+= its->shape->getMaterial().getDiffuseCoefficient() * ambientTerm;
		}

		if (its->shape->getMaterial().hasSpecular()) {
			
			Vector3D wr = its->normal * 2 * (dot(-r.d, its->normal)) + r.d;
			Ray reflectionRay = Ray(its->itsPoint, wr, r.depth + 1);
			color = computeColor(reflectionRay, objList, lsList);
		}
		
		if (its->shape->getMaterial().hasTransmission()) {
			
			double nt = its->shape->getMaterial().getIndexOfRefraction();
			
			if (dot(r.d, its->normal)>0) { //We check if we are inside the material, if so, we invert things
				its->normal = -its->normal;
				nt = 1 / its->shape->getMaterial().getIndexOfRefraction();
			}


			double cos_alpha = dot(its->normal, -r.d);
			double sin2_alpha = 1 - std::pow(cos_alpha, 2);
			double radical = 1 - std::pow(nt, 2) * sin2_alpha; //Get indexOfRefraction ja agafa el ratio

			if (radical >= 0) {
				
				double first = - std::sqrt(radical);
				double second = nt * dot(-r.d, its->normal);
				Vector3D third = its->normal * (first + second);
				
				Vector3D wt = third - (-r.d) * nt;

				Ray refracRay = Ray(its->itsPoint, wt, r.depth + 1);
				color = computeColor(refracRay, objList, lsList);
				//color = Vector3D(1, 0, 0);
			}
			else {
				Vector3D wr = its->normal * 2 * (dot(-r.d, its->normal)) + r.d;
				Ray reflectionRay = Ray(its->itsPoint, wr, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
			}

		}

		
		return color;
		

	}
	else {

		return bgColor;
	}
}
