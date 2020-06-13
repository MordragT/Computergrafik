#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include "common.h"


//#include "common.h"

class Model : QOpenGLFunctions_3_3_Core
{
public:
    Model();

    void initGL(
            const QString &filename,
            const QString &vertSrc,
            const QString &fragSrc
    );
    void addTex(const QString &tex);
    void drawElements(
            const ObjectProperties &obj,
            const Material &mat,
            const Scene &scene
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
