
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

    if (Utils::getClosestIntersection(r, objList, *its)) {
        Vector3D interesectionPoint = its->itsPoint;

        for (size_t i = 0; i < objList.size(); i++)
        {
            Shape* actualObject = objList.at(i);
            for (int s = 0; s < lsList.size(); s++)
            {
                PointLightSource actualSource_s = lsList.at(s);
               
                Vector3D resultPhong_l0;
               
                Vector3D actualSourcePos = actualSource_s.getPosition();
                Vector3D interesectionPoint = its->itsPoint;
                Vector3D wi = actualSourcePos - interesectionPoint;
                double maxT = std::sqrt(std::pow(wi.x, 2) + std::pow(wi.y, 2) + std::pow(wi.z, 2));
                
                Ray wiTest = Ray(interesectionPoint, -wi.normalized(), 0, Epsilon, maxT);
                
                if (Utils::hasIntersection(wiTest, objList)) {

                  Vector3D reflectance= actualObject->getMaterial().getReflectance(its->normal, -r.d, wi);
                  resultPhong_l0 += actualSource_s.getIntensity(interesectionPoint) * reflectance;
                  return resultPhong_l0;
                }
                
            }

        }
        return bgColor;
        
    }
    else {

        return bgColor;
    }
}
