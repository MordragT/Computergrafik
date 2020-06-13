#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
#include <QMatrix4x4>
#include "skybox.h"

struct Material {
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;
};

struct ObjectProperties {
    QVector3D position;
    QVector3D baseColor{0.0, 0.0, 0.0};
    QMatrix4x4 rotation;
    float scale = 1;
    float shininess = 0;
};

struct Scene {
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QVector3D viewPos{0.0, 0.0, 0.0};
    Skybox* skybox;
};

struct LightSource {
    alignas (16) QVector3D position ;
    alignas (16) QVector3D color ;
    float ka;
    float kd;
    float ks;
    float constant;
    float linear;
    float quadratic;
};

#endif // MATERIAL_H
