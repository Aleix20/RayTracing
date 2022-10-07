
#include "directshader.h"
#include "../core/utils.h"
DirectShader::DirectShader() :
	hitColor(Vector3D(0, 0, 0))
{ }

DirectShader::DirectShader(Vector3D hitColor_, Vector3D bgColor_) :
	Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection* its = new Intersection();
	//Check the closest intersection with all the object of the scene
	if (Utils::getClosestIntersection(r, objList, *its)) {
		Vector3D interesectionPoint = its->itsPoint;
		Vector3D wo = (r.o - interesectionPoint).normalized();
		Vector3D resultPhong_l0;

		//Check of the lightsources of the scene
		for (int s = 0; s < lsList.size(); s++)
		{
			PointLightSource actualSource_s = lsList.at(s);

			Vector3D actualSourcePos = actualSource_s.getPosition();
			//Vector to the lightsource
			Vector3D wi = (actualSourcePos - interesectionPoint).normalized();
			double maxT = std::sqrt(std::pow(wi.x, 2) + std::pow(wi.y, 2) + std::pow(wi.z, 2));
			//Ray to check if there are any object between the light and the hit point
			Ray wiTest = Ray(interesectionPoint, wi.normalized(), 0, Epsilon, maxT);
			//Check if there is any object between the light and the intersection point
			if (!Utils::hasIntersection(wiTest, objList)) {
				//Compute the phong color
				Vector3D reflectance = its->shape->getMaterial().getReflectance(its->normal, wo, wi);
				resultPhong_l0 += actualSource_s.getIntensity(interesectionPoint) * reflectance;
				
			}

		}


		 return resultPhong_l0;;

	}
	else {

		return bgColor;
	}
}
