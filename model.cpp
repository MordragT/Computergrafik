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

void Model::initGL(
        const QString &filename,
        const QString &vertSrc,
        const QString &fragSrc
) {
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

    // Shader
    this->m_prog = new QOpenGLShaderProgram{};
    this->m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, vertSrc);
    this->m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, fragSrc);
    Q_ASSERT(this->m_prog->link());

    // Default Texture
    QImage texImg;
    Q_ASSERT(texImg.load(":/tex/base_tex.png"));
    this->m_texture = new QOpenGLTexture{QOpenGLTexture::Target2D};
    this->m_texture->setData(texImg,QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    Q_ASSERT(this->m_texture->isCreated());
}

void Model::addTex(const QString &tex) {
    QImage texImg;
    Q_ASSERT(texImg.load(tex));
    this->m_texture = new QOpenGLTexture{QOpenGLTexture::Target2D};
    this->m_texture->setData(texImg,QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    Q_ASSERT(this->m_texture->isCreated());
}

void Model::drawElements(
        const ObjectProperties &obj,
        const Material &mat,
        const Scene &scene
) {
    QMatrix4x4 model;
    model.scale(QVector3D{obj.scale, obj.scale, obj.scale});
    model = model * obj.rotation;

    QMatrix4x4 mvp = scene.projection * scene.view * model;

    this->m_prog->bind();
    this->m_prog->setUniformValue("uMvp", mvp);
    this->m_prog->setUniformValue("uModel", model);
    this->m_prog->setUniformValue("uModelTransposed", model.inverted().transposed());
    this->m_prog->setUniformValue("uViewPos", scene.viewPos);
    this->m_prog->setUniformValue("uBaseColor", obj.baseColor);
    this->m_prog->setUniformValue("uShininess", obj.shininess);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skybox->m_cubeTex);
    this->m_prog->setUniformValue("uCubeMap", 0);
    this->m_prog->setUniformValue("uMaterial.ambient", mat.ambient);
    this->m_prog->setUniformValue("uMaterial.diffuse", mat.diffuse);
    this->m_prog->setUniformValue("uMaterial.specular", mat.specular);
    this->m_prog->setUniformValue("uMaterial.shininess", mat.shininess);

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
