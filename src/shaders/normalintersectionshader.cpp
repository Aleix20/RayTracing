
#include "normalintersectionshader.h"
#include "../core/utils.h"
NormalIntersectionShader::NormalIntersectionShader() :
    hitColor(Vector3D(0, 0, 0))
{ }

NormalIntersectionShader::NormalIntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D NormalIntersectionShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection* its = new Intersection();
    
    if (Utils::getClosestIntersection(r, objList, *its)) {
       
        Vector3D resultColor = (its->normal + Vector3D(1.0, 1.0, 1.0)) / 2;
        return resultColor;
    }
    else {
        return bgColor;
    }
    
}
