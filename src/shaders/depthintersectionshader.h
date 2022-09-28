#ifndef DEPTHINTERSECTIONSHADER_H
#define DEPTHINTERSECTIONSHADER_H
#include "shader.h"
class DepthIntersectionShader : public Shader
{
public:
    DepthIntersectionShader();
    DepthIntersectionShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList,  Intersection& its) const;

    Vector3D hitColor;
};

#endif // DEPTHINTERSECTIONSHADER_H