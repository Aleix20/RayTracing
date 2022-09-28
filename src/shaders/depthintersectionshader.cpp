
#include "depthintersectionshader.h"
#include "../core/utils.h"
DepthIntersectionShader::DepthIntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

DepthIntersectionShader::DepthIntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DepthIntersectionShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList,  Intersection& its) const
{
    if (Utils::getClosestIntersection(r, objList, its))
        return hitColor;
    else
        return bgColor;
}
