#include "myglwidget.h"
#include <QtDebug>
#include <QKeyEvent>
#include <QOpenGLTexture>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget() {
    //Make Current to enable for cleanup
    makeCurrent();

    // Deletion
    this->m_vao.destroy();
    this->m_ibo.destroy();
    this->m_vbo.destroy();
    this->m_texture->destroy();

    doneCurrent();
}

void MyGLWidget::keyPressEvent( QKeyEvent * event ) {
    switch(event->key()) {
    case Qt::Key_W:
        this->m_cameraPos.setZ(this->m_cameraPos.z() + 0.2);
        break;
    case Qt::Key_A:
        this->m_cameraPos.setX(this->m_cameraPos.x() - 0.2);
        break;
    case Qt::Key_S:
        this->m_cameraPos.setZ(this->m_cameraPos.z() - 0.2);
        break;
    case Qt::Key_D:
        this->m_cameraPos.setX(this->m_cameraPos.x() + 0.2);
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
        break;
    }
    qInfo() << m_cameraPos;
}

void MyGLWidget::initializeGL() {
    //Lade OpenGL funktionen vom Treiber
    Q_ASSERT(initializeOpenGLFunctions());

    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

    //clear color einstellen: dunkelgrau
    glClearColor(0.4, 0.4, 0.4, 1.0);

    QImage texImg;
    Q_ASSERT(texImg.load(":/sample_texture.jpg"));
    this->m_texture = new QOpenGLTexture{QOpenGLTexture::Target2D};
    this->m_texture->setData(texImg,QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    Q_ASSERT(this->m_texture->isCreated());
    this->m_texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);

    this->m_prog = new QOpenGLShaderProgram{};
    Q_ASSERT(this->m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sample.vert"));
    Q_ASSERT(this->m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sample.frag"));
    Q_ASSERT(this->m_prog->link());
    this->m_prog->bind();

    Q_ASSERT(this->m_vao.create());
    // Folgende Modifikationen wie vbo werden in vao gespeichert
    this->m_vao.bind();

    this->m_ibo.create();
    Q_ASSERT(this->m_ibo.bind());
    this->m_ibo.allocate(this->m_indices, this->m_indicesSize * sizeof(GLuint));

    this->m_vbo.create();
    //Bind to current context
    Q_ASSERT(this->m_vbo.bind());
    this->m_vbo.allocate(this->m_vertices, this->m_verticesSize * sizeof(Vertex));

    int stride = 8 * sizeof(GLfloat);

    this->m_prog->enableAttributeArray(0);
    this->m_prog->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
    this->m_prog->enableAttributeArray(1);
    this->m_prog->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, stride);
    this->m_prog->enableAttributeArray(2);
    this->m_prog->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, stride);
    this->m_prog->setUniformValue("tex", 0);

    this->m_vbo.release();
    this->m_ibo.release();
    this->m_vao.release();
    this->m_prog->release();

}

void MyGLWidget::paintGL() {
    //Render in clear color
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(this->m_vao.objectId());
    this->m_prog->bind();
    {
        this->m_prog->setUniformValue("uAlpha", this->m_transparency);
        this->m_prog->setUniformValue("uvCoordinatesAdd", this->m_uvCoordinatesAdd);
        this->m_texture->bind();
        this->m_vao.bind();

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        this->m_vao.release();
        this->m_texture->release();
    }
    this->m_prog->release();

    update();
}

void MyGLWidget::resizeGL(int w, int h) {

}

//----------------------------------------
// Slots
//----------------------------------------

void MyGLWidget::setFOV(int value) {
    this->m_FOV = value;
    emit fovChanged(value);
}

void MyGLWidget::setAngle(int angle) {
    this->m_angle = angle;
    emit angleChanged(angle);
}

void MyGLWidget::setPerspective(bool toggle) {
    this->m_perspective = toggle;
    emit perspectiveChanged(toggle);
}

void MyGLWidget::setOrthogonal(bool toggle) {
    this->m_orthogonal = toggle;
    emit orthogonalChanged(toggle);
}

void MyGLWidget::setNear(double value) {
    double diff = this->m_far - value;
    if(diff >= 2.0) {
        qInfo() << "near" << value << "diff" << diff;
        this->m_near = value;
        emit nearChanged(value);
    } else emit nearOVFL(this->m_near);
}

void MyGLWidget::setFar(double value) {
    double diff = value - this->m_near;
    if(diff >= 2.0) {
        qInfo() << "far" << value << "diff" << diff;
        this->m_far = value;
        emit farChanged(value);
    } else emit farOVFL(this->m_far);
}

void MyGLWidget::setRotationA(int value) {
    this->m_rotationA = value;
    this->m_transparency = (float)value/100;
    qInfo() << this->m_transparency;
    emit rotationAChanged(value);
}

void MyGLWidget::setRotationB(int value) {
    this->m_rotationB = value;
    this->m_uvCoordinatesAdd = (float)value/100;
    emit rotationBChanged(value);
}

void MyGLWidget::setRotationC(int value) {
    this->m_rotationC = value;
    emit rotationCChanged(value);
}


