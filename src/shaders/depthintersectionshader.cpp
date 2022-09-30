
#include "depthintersectionshader.h"
#include "../core/utils.h"
DepthIntersectionShader::DepthIntersectionShader() :
    hitColor(Vector3D(0, 0, 0))
{ }

DepthIntersectionShader::DepthIntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DepthIntersectionShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection* its = new Intersection();
    
    if (Utils::getClosestIntersection(r, objList, *its)) {
        double const c = 1.0 - (r.maxT/ 7.0);
        Vector3D resultColor; 
        if (c >0) {
            resultColor.y = c;
        }
        return resultColor;
    }
    else {

        return bgColor;
    }
}
