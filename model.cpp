#include <vector>
#include <QImage>
#include <QMatrix4x4>
#include <QtDebug>

#include "model.h"
#include "modelloader/modelloader.h"

Model::Model()
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer)
{}

void Model::initGL(const QString &filename) {
    initializeOpenGLFunctions();

    m_vbo.create();
    m_ibo.create();
    m_vao.create();

    m_vao.bind();
    m_vbo.bind();
    m_ibo.bind();

    ModelLoader ml;
    ml.loadObjectFromFile(filename);
    Q_ASSERT(ml.hasScene());

    unsigned vboLength = ml.lengthOfVBO();
    m_iboCount = ml.lengthOfIndexArray();

    // There's two ways to accomplish the buffer uploads;
    // the pointer mapping one appears more complex but
    // saves one needless allocation each
#define BUFFER_UPLOAD_COPY 0
#if BUFFER_UPLOAD_COPY == 1
    std::vector<GLfloat> vboData;
    std::vector<GLuint> iboData;

    vboData.resize(vboLength);
    iboData.resize(m_iboCount);

    ml.genVBO(vboData.data());
    ml.genIndexArray(iboData.data());

    m_vbo.allocate(vboData.data(), vboData.size()*sizeof(GLfloat));
    m_ibo.allocate(iboData.data(), iboData.size()*sizeof(GLuint));
#else
    m_vbo.allocate(vboLength*sizeof(GLfloat));
    m_ibo.allocate(m_iboCount*sizeof(GLuint));

    GLvoid *vboPtr = m_vbo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(vboPtr);

    ml.genVBO(static_cast<GLfloat*>(vboPtr));
    m_vbo.unmap();

    GLvoid *iboPtr = m_ibo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(iboPtr);

    ml.genIndexArray(static_cast<GLuint*>(iboPtr));
    m_ibo.unmap();
#endif

    size_t stride = (ML_VERTPOS_COUNT+ML_NORMAL_COUNT+ML_TEXCOOR_COUNT)*sizeof(GLfloat);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*6));

    m_vao.release();

    //#######################################
    // Own Code
    //#######################################

    // Texture
    QImage texImg;
    Q_ASSERT(texImg.load(":/gimbal_wood.jpg"));
    this->m_texture = new QOpenGLTexture{QOpenGLTexture::Target2D};
    this->m_texture->setData(texImg,QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    Q_ASSERT(this->m_texture->isCreated());

    // Shader
    this->m_prog = new QOpenGLShaderProgram{};
    this->m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gimbal.vert");
    this->m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gimbal.frag");
    Q_ASSERT(this->m_prog->link());
}

void Model::drawElements(
        float fov,
        double far,
        double near,
        float aspectRatio,
        float scale,
        QMatrix4x4 rotation,
        QMatrix4x4 view
) {
    QMatrix4x4 model;
    model.scale(QVector3D{scale, scale, scale});
    model *= rotation;

    QMatrix4x4 projection;
    projection.perspective(fov, aspectRatio, near, far);

    QMatrix4x4 mvp = projection * view * model;

    this->m_prog->bind();
    this->m_prog->setUniformValue("uMvp", mvp);
    this->m_texture->bind();
    this->m_vao.bind();

    glDrawElements(GL_TRIANGLES, m_iboCount, GL_UNSIGNED_INT, nullptr);

    this->m_vao.release();
    this->m_prog->release();
}

void Model::finiGL() {
    m_vao.destroy();
    m_ibo.destroy();
    m_vbo.destroy();
}
