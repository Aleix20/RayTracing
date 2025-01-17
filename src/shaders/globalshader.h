#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#define _USE_MATH_DEFINES

#include "shader.h"

class GlobalShader : public Shader
{
public:
    
    GlobalShader();
    GlobalShader(Vector3D bgColor_);
    
    Vector3D ambientTerm = Vector3D(0.4);
    int nSamples = 50;//Number of rays that we will throw trough the hemisphere
    int maxDepth = 3;//Number of bounces
    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

};

#endif // GLOBALSHADER_H