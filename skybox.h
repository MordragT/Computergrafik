#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Skybox : QOpenGLFunctions_3_3_Core
{
public:
    GLuint m_cubeTex;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ibo;

    QOpenGLShaderProgram *m_prog;

    Skybox();
    ~Skybox();

    void render(const QMatrix4x4 &projection, QMatrix4x4 view);
};

#endif // SKYBOX_H
