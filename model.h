#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>


//#include "common.h"

class Model : QOpenGLFunctions_3_3_Core
{
public:
    Model();

    void initGL(const QString &filename);
    void drawElements(
            float fov,
            double far,
            double near,
            float aspectRatio,
            float scale,
            QMatrix4x4 rotation,
            QMatrix4x4 view
    );
    void finiGL();
    QOpenGLShaderProgram *m_prog;

private:
    QOpenGLTexture* m_texture;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    QOpenGLVertexArrayObject m_vao;

    unsigned m_iboCount = 0;
};

#endif // MODEL_H
