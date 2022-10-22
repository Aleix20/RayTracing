#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H
#include "shader.h"

class GlobalShader : public Shader
{
public:
    GlobalShader();
    GlobalShader(Vector3D bgColor_, Vector3D ambientTerm_ );
    Vector3D ambientTerm;
    int n_directions = 30;
    int n_bounces = 2;
    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

};

#endif // GLOBALSHADER_H