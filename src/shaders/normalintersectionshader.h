#ifndef NORMALINTERSECTIONSHADER_H
#define NORMALINTERSECTIONSHADER_H
#include "shader.h"
class NormalIntersectionShader : public Shader
{
public:
    NormalIntersectionShader();
    NormalIntersectionShader(Vector3D hitColor, Vector3D bgColor_);
    Vector3D hitColor;

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

    
};

#endif // NORMALINTERSECTIONSHADER_H