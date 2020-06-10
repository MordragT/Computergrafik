#include "myglwidget.h"
#include <QtDebug>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget() {
    // Make Current to enable for cleanup
    makeCurrent();

    // Deletion
    /*
    this->m_vao.destroy();
    this->m_ibo->destroy();
    this->m_vbo->destroy();
    this->m_texture->destroy();

    delete this->m_ibo;
    delete this->m_vbo;
    delete this->m_texture;
    delete this->m_prog;
    */
    delete this->logger;

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
    //qInfo() << m_cameraPos;
}

void MyGLWidget::initializeGL() {
    // Lade OpenGL funktionen vom Treiber
    Q_ASSERT(initializeOpenGLFunctions());

    // Debugging aktivieren
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    Q_ASSERT(ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug")));
    this->logger = new QOpenGLDebugLogger(this);
    logger->initialize();

    glEnable ( GL_BLEND );
    // Enable Face Culling (Hide Faces shown from the back)
    //glEnable(GL_CULL_FACE);
    glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);

    // clear color einstellen: dunkelgrau
    glClearColor(0.2, 0.2, 0.2, 1.0);

    /*
    //####################################################
    // Texture
    //####################################################

    QImage texImg;
    Q_ASSERT(texImg.load(":/sample_texture.jpg"));
    this->m_texture = new QOpenGLTexture{QOpenGLTexture::Target2D};
    this->m_texture->setData(texImg,QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    Q_ASSERT(this->m_texture->isCreated());
    this->m_texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);

    //####################################################
    // Shader
    //####################################################

    this->m_prog = new QOpenGLShaderProgram{};
    Q_ASSERT(this->m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sample.vert"));
    Q_ASSERT(this->m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/texture.frag"));
    Q_ASSERT(this->m_prog->link());

    this->m_progColor = new QOpenGLShaderProgram{};
    Q_ASSERT(this->m_progColor->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sample.vert"));
    Q_ASSERT(this->m_progColor->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/color.frag"));
    Q_ASSERT(this->m_progColor->link());

    //######################################################
    // Buffer
    //######################################################

    this->m_ibo = new QOpenGLBuffer{QOpenGLBuffer::Type::IndexBuffer};
    this->m_ibo->create();
    Q_ASSERT(this->m_ibo->bind());
    this->m_ibo->allocate(this->m_indices, sizeof(this->m_indices));
    this->m_ibo->release();

    this->m_vbo = new QOpenGLBuffer{QOpenGLBuffer::Type::VertexBuffer};
    this->m_vbo->create();
    Q_ASSERT(this->m_vbo->bind());
    this->m_vbo->allocate(this->m_vertices, sizeof(this->m_vertices));
    this->m_vbo->release();

    Q_ASSERT(this->m_vao.create());
    // Folgende Modifikationen wie vbo werden in vao gespeichert
    this->m_vao.bind();
        this->m_vbo->bind();
        this->m_ibo->bind();

        int stride = 8 * sizeof(GLfloat);

        // vao muss gebindet sein, damit shader auf vbo zugreifen kann
        this->m_prog->bind();
            this->m_prog->enableAttributeArray(0);
            this->m_prog->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
            this->m_prog->enableAttributeArray(1);
            this->m_prog->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, stride);
            this->m_prog->enableAttributeArray(2);
            this->m_prog->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, stride);
            //this->m_prog->setUniformValue("tex", 0);

        this->m_progColor->bind();
            this->m_progColor->enableAttributeArray(0);
            this->m_progColor->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
            this->m_progColor->enableAttributeArray(1);
            this->m_progColor->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, stride);
            this->m_progColor->enableAttributeArray(2);
            this->m_progColor->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, stride);
            //this->m_progColor->setUniformValue("tex", 0);
    this->m_vao.release();
    this->m_vbo->release();
    this->m_ibo->release();
    this->m_prog->release();
    this->m_progColor->release();
    */

    this->m_sphere = new Model{};
    this->m_sphere->initGL(":/sphere.obj");

    this->m_innerGimbal = new Model{};
    this->m_innerGimbal->initGL(":/gimbal.obj");

    this->m_gimbal = new Model{};
    this->m_gimbal->initGL(":/gimbal.obj");

    this->m_outerGimbal = new Model{};
    this->m_outerGimbal->initGL(":/gimbal.obj");

    this->setFar(99.99);
    this->setNear(0.01);
    this->setFOV(45);
}

void MyGLWidget::paintGL() {
    // Render in clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear Depth Buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    /*
    //glBindVertexArray(this->m_vao.objectId());
    this->m_prog->bind();
    {
        this->m_prog->setUniformValue("uRotMat", rotMatrix);
        this->m_prog->setUniformValue("uvCoordinatesAdd", this->m_uvCoordinatesAdd);
        this->m_texture->bind();
        this->m_vao.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
        this->m_vao.release();
        this->m_texture->release();
    }
    this->m_prog->release();

    this->m_progColor->bind();
    {
        this->m_prog->setUniformValue("uRotMat", rotMatrix);
        this->m_progColor->setUniformValue("uAlpha", this->m_transparency);
        this->m_vao.bind();
        void* const offset = reinterpret_cast<void* const>(sizeof( GLushort )*3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, offset);
        this->m_vao.release();
    }
    this->m_progColor->release();
    */

    QMatrix4x4 view;
    view.lookAt(
        QVector3D{0.0,0.0,10.0},  //eye
        QVector3D{0.0,0.0,0.0},   //center
        QVector3D{0.0,1.0,0.0}    //upvector
    );

    int outerGimbalAngle = 0;
    int gimbalAngle = 0;
    int innerGimbalAngle = 0;
    int sphereAngle = 0;

    if (this->m_animate) {
        this->m_timer += 0.5;
        outerGimbalAngle = this->m_timer / 0.2;
        gimbalAngle = this->m_timer;
        innerGimbalAngle = this->m_timer / 2;
        sphereAngle = this->m_timer;
    } else {
        outerGimbalAngle = this->m_rotationA;
        gimbalAngle = this->m_rotationB;
        innerGimbalAngle = this->m_rotationC;
        sphereAngle = this->m_angle;
    }

    QMatrix4x4 outerGimbalAnimation;
    outerGimbalAnimation.rotate(outerGimbalAngle, QVector3D{1.0, 0.0, 0.0});

    QMatrix4x4 gimbalAnimation;
    gimbalAnimation.rotate(gimbalAngle, QVector3D{0.0, 1.0, 0.0});
    gimbalAnimation = outerGimbalAnimation * gimbalAnimation;

    QMatrix4x4 innerGimbalAnimation;
    innerGimbalAnimation.rotate(innerGimbalAngle, QVector3D{1.0, 0.0, 0.0});
    innerGimbalAnimation = gimbalAnimation * innerGimbalAnimation;

    QMatrix4x4 sphereAnimation;
    sphereAnimation.rotate(sphereAngle, QVector3D{0, 0, 1});
    sphereAnimation.translate(QVector3D{0.0, 10.0, 2.0});
    sphereAnimation = gimbalAnimation * sphereAnimation;
    sphereAnimation.rotate(this->m_angle, QVector3D{0, -1, 0}); // beim rollen drehen

    this->m_outerGimbal->m_prog->bind();
    this->m_outerGimbal->m_prog->setUniformValue("uRingColor", QVector4D{1.0, 0.0, 0.0, 1.0});
    this->m_outerGimbal->drawElements(
                this->m_FOV,
                this->m_far,
                this->m_near,
                (float)(this->width() / this->height()),
                1.75,
                outerGimbalAnimation,
                view
    );

    this->m_gimbal->m_prog->bind();
    this->m_gimbal->m_prog->setUniformValue("uRingColor", QVector4D{0.0, 1.0, 0.0, 1.0});
    this->m_gimbal->drawElements(
                this->m_FOV,
                this->m_far,
                this->m_near,
                (float)(this->width() / this->height()),
                2.25,
                gimbalAnimation,
                view
    );

    this->m_innerGimbal->m_prog->bind();
    this->m_innerGimbal->m_prog->setUniformValue("uRingColor", QVector4D{0.0, 0.0, 1.0, 1.0});
    this->m_innerGimbal->drawElements(
                this->m_FOV,
                this->m_far,
                this->m_near,
                (float)(this->width() / this->height()),
                2.75,
                innerGimbalAnimation,
                view
    );

    this->m_sphere->m_prog->bind();
    this->m_sphere->m_prog->setUniformValue("uRingColor", QVector4D{1.0, 1.0, 1.0, 1.0});
    this->m_sphere->drawElements(
                this->m_FOV,
                this->m_far,
                this->m_near,
                (float)(this->width() / this->height()),
                0.2,
                sphereAnimation,
                view
    );

    // Read the internal OpenGL Debug Log
    const QList<QOpenGLDebugMessage> messages = this->logger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;

    update();
}

void MyGLWidget::resizeGL(int w, int h) {

}

//####################################################
// Slots
//####################################################

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
        //qInfo() << "near" << value << "diff" << diff;
        this->m_near = value;
        emit nearChanged(value);
    } else emit nearOVFL(this->m_near);
}

void MyGLWidget::setFar(double value) {
    double diff = value - this->m_near;
    if(diff >= 2.0) {
        //qInfo() << "far" << value << "diff" << diff;
        this->m_far = value;
        emit farChanged(value);
    } else emit farOVFL(this->m_far);
}

void MyGLWidget::setRotationA(int value) {
    this->m_rotationA = value;
    //this->m_transparency = (float)value/100;
    //qInfo() << this->m_transparency;
    emit rotationAChanged(value);
}

void MyGLWidget::setRotationB(int value) {
    this->m_rotationB = value;
    //this->m_uvCoordinatesAdd = (float)value/100;
    emit rotationBChanged(value);
}

void MyGLWidget::setRotationC(int value) {
    this->m_rotationC = value;
    emit rotationCChanged(value);
}

void MyGLWidget::setAnimate(bool value) {
    this->m_animate = value;
    emit animateChanged(value);
}

void MyGLWidget::setCamera(bool value) {
    this->m_camera = value;
    emit cameraChanged(value);
}

