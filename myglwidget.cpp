#include <QtDebug>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <QQueue>
#include <QOpenGLFramebufferObject>

#include "myglwidget.h"
#include "gauss_filter.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget()
{
    // Make Current to enable for cleanup
    makeCurrent();
    delete this->logger;

    doneCurrent();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        this->m_cameraPos.setZ(this->m_cameraPos.z() - 0.2);
        break;
    case Qt::Key_A:
        this->m_cameraPos.setX(this->m_cameraPos.x() - 0.2);
        break;
    case Qt::Key_S:
        this->m_cameraPos.setZ(this->m_cameraPos.z() + 0.2);
        break;
    case Qt::Key_D:
        this->m_cameraPos.setX(this->m_cameraPos.x() + 0.2);
        break;
    case Qt::Key_Up:
        this->m_viewAngleVertical += 2;
        break;
    case Qt::Key_Down:
        this->m_viewAngleVertical -= 2;
        break;
    case Qt::Key_Left:
        this->m_viewAngleHorizontal += 2;
        break;
    case Qt::Key_Right:
        this->m_viewAngleHorizontal -= 2;
        break;
    case Qt::Key_P:
    {

        //QImage img = this->grabFramebuffer();
        //QImage img = this->m_fbo->toImage();
        QImage img{this->size(), QImage::Format_RGBA8888};
        makeCurrent();
        glBindTexture(GL_TEXTURE_2D, this->m_colorTex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        doneCurrent();
        img.mirrored(false, true).save("screenshot.png");
        break;
    }
//    case Qt::Key_O:
//    {
//        QImage img{this->size(), QImage::Format_RGBA8888};
//        makeCurrent();
//        glBindTexture(GL_TEXTURE_2D, this->m_depthTex);
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH, GL_UNSIGNED_BYTE, img.bits());
//        doneCurrent();
//        img.mirrored(false, true).save("depth.png");
//        break;
//    }
    default:
        QOpenGLWidget::keyPressEvent(event);
        break;
    }
    //qInfo() << m_cameraPos;
}

void MyGLWidget::initializeGL()
{
    // Lade OpenGL funktionen vom Treiber
    Q_ASSERT(initializeOpenGLFunctions());

    // Debugging aktivieren
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    Q_ASSERT(ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug")));
    this->logger = new QOpenGLDebugLogger(this);
    logger->initialize();

    //this->m_fbo = new QOpenGLFramebufferObject{this->width(), this->height()};
    glGenFramebuffers(1, &this->m_fboHandle);

    glGenTextures(1, &this->m_colorTex);
    glBindTexture(GL_TEXTURE_2D, this->m_colorTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA8,
                width(),
                height(),
                0,
                GL_BGRA,
                GL_UNSIGNED_BYTE,
                nullptr
    );

    glGenTextures(1, &this->m_depthTex);
    glBindTexture(GL_TEXTURE_2D, this->m_depthTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_DEPTH_COMPONENT16,
                width(),
                height(),
                0,
                GL_DEPTH_COMPONENT,
                GL_UNSIGNED_BYTE,
                nullptr
    );

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_fboHandle);
    //this->m_fbo->bind();

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_colorTex, 0
    );
    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->m_depthTex, 0
    );

//    Q_ASSERT(this->m_fbo->isValid());
//    Q_ASSERT(this->m_fbo->hasOpenGLFramebufferBlit());
//    this->m_fboHandle = this->m_fbo->handle();
    Q_ASSERT(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);


    glEnable(GL_BLEND);
    // Enable Face Culling (Hide Faces shown from the back)
    //glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // clear color einstellen: dunkelgrau
    glClearColor(0.2, 0.2, 0.2, 1.0);

    this->m_skybox = new Skybox{};

    this->m_sphere = new Model{};
    this->m_sphere->initGL(":/obj/sphere.obj", ":/shader/vert/gimbal.vert", ":/shader/frag/gimbal.frag");
    this->m_sphere->addTex(":/tex/gimbal_wood.jpg");

    this->m_gimbal = new Model{};
    this->m_gimbal->initGL(":/obj/gimbal.obj", ":/shader/vert/sample.vert", ":/shader/frag/sample.frag");
    this->m_gimbal->addTex(":/tex/gimbal_wood.jpg");

    this->m_lightProps = {
        ObjectProperties{.position = QVector3D{18.0, 0.0, 0.0}, .baseColor = QVector3D{1.0, 0.0, 0.0}},
        ObjectProperties{.position = QVector3D{0.0, 18.0, 0.0}, .baseColor = QVector3D{0.0, 1.0, 0.0}},
        ObjectProperties{.position = QVector3D{0.0, 0.0, 18.0}, .baseColor = QVector3D{0.0, 0.0, 1.0}},
        ObjectProperties{.position = QVector3D{-18.0, 0.0, 0.0}, .baseColor = QVector3D{1.0, 1.0, 0.0}},
        ObjectProperties{.position = QVector3D{0.0, -18.0, 0.0}, .baseColor = QVector3D{0.0, 1.0, 1.0}},
    };

    // Generate Uniform Buffer Object
    glGenBuffers(1, &this->m_uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, this->m_uboLights);
    glBufferData(GL_UNIFORM_BUFFER, NUM_LS * sizeof(LightSource), nullptr, GL_STATIC_DRAW); // Set Buer size, 64 Byte for each LS
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->m_uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->m_light = new Model{};
    this->m_light->initGL(":/obj/sphere.obj", ":/shader/vert/light.vert", ":/shader/frag/light.frag");

    for (unsigned i = 0; i < NUM_LS; i += 1)
    {
        ObjectProperties lightProps = this->m_lightProps.at(i);
        this->m_lightSources[i].position = lightProps.position;
        this->m_lightSources[i].color = lightProps.baseColor;
        this->m_lightSources[i].ka = 0.0;
        this->m_lightSources[i].kd = 0.0;
        this->m_lightSources[i].ks = 0.0;
        this->m_lightSources[i].constant = 1.0;
        this->m_lightSources[i].linear = 0.0014;
        this->m_lightSources[i].quadratic = 0.000007;
    }

    this->setFar(99.99);
    this->setNear(0.01);
    this->setFOV(45);

    Vertex position[4] = {
        Vertex{
            .position = {-1.0, -1.0},
            .texCoordinates = {0.0, 0.0},
        },
        Vertex{
            .position = {1.0, -1.0},
            .texCoordinates = {1.0, 0.0},
        },
        Vertex{
            .position = {-1.0, 1.0},
            .texCoordinates = {0.0, 1.0},
        },
        Vertex{
            .position = {1.0, 1.0},
            .texCoordinates = {1.0, 1.0},
        },
    };
    GLuint indices[6] {
        0, 1, 2,
        1, 2, 3
    };

    this->m_rect = new Model{};
    this->m_rect->initGL(position, 4, indices, 6, ":/shader/vert/rect.vert", ":/shader/frag/rect.frag");

    this->m_gaussComputeShader = new QOpenGLShaderProgram{};
    this->m_gaussComputeShader->addShaderFromSourceFile(QOpenGLShader::Compute, ":/shader/compute/compute_gauss.glsl");
    Q_ASSERT(this->m_gaussComputeShader->link());

    this->m_colorComputeShader = new QOpenGLShaderProgram{};
    this->m_colorComputeShader->addShaderFromSourceFile(QOpenGLShader::Compute, ":/shader/compute/compute_filter.glsl");
    Q_ASSERT(this->m_colorComputeShader->link());
}

void MyGLWidget::resizeGL(int w, int h)
{
//    delete this->m_fbo;
//    this->m_fbo = new QOpenGLFramebufferObject{w, h, QOpenGLFramebufferObject::Attachment::Depth};
//    this->m_fbo->addColorAttachment(w, h);
//    this->m_fboHandle = this->m_fbo->handle();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_fboHandle);
    glBindTexture(GL_TEXTURE_2D, this->m_colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, this->m_depthTex);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr
    );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MyGLWidget::paintGL()
{
    int outerGimbalAngle = 0;
    int gimbalAngle = 0;
    int innerGimbalAngle = 0;

    this->m_timer += 0.5;
    if (this->m_animate)
    {
        outerGimbalAngle = this->m_timer / 0.2;
        gimbalAngle = this->m_timer;
        innerGimbalAngle = this->m_timer / 2;
    }
    else
    {
        outerGimbalAngle = this->m_rotationA;
        gimbalAngle = this->m_rotationB;
        innerGimbalAngle = this->m_rotationC;
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
    sphereAnimation.rotate(this->m_timer, QVector3D{0, 0, 1});
    sphereAnimation.translate(QVector3D{0.0, 10.0, 2.0});
    sphereAnimation = gimbalAnimation * sphereAnimation;
    // Beim rollen drehen
    sphereAnimation.rotate(this->m_timer * 7, QVector3D{0, -1, 0});

    QMatrix4x4 view;
    if (this->m_camera)
    {
        view = innerGimbalAnimation;
    }
    else
    {
        view.lookAt(
            QVector3D{this->m_cameraPos.x(), this->m_cameraPos.y(), this->m_cameraPos.z() + (float)10.0},
            QVector3D{this->m_cameraPos.x(), 0.0, 0.0},
            QVector3D{0.0, 1.0, 0.0});
        view.rotate(this->m_viewAngleVertical, QVector3D{1.0, 0.0, 0.0});
        view.rotate(this->m_viewAngleHorizontal, QVector3D{0.0, 1.0, 0.0});
    }

    QMatrix4x4 projection;
    projection.perspective(
        this->m_FOV,
        (float)(this->width() / this->height()),
        this->m_near,
        this->m_far);

    Scene scene{
        .projection = projection,
        .view = view,
        .viewPos = this->m_cameraPos,
        .skybox = this->m_skybox};

    Material mat{
        .ambient = QVector3D{0.25, 0.2, 0.0},
        .diffuse = QVector3D{0.75, 0.6, 0.2},
        .specular = QVector3D{0.6, 0.5, 0.3},
        .shininess = 0.4};

    ObjectProperties innerGimbalProps{
        .baseColor = QVector3D{0.5, 0.5, 1.0},
        .rotation = innerGimbalAnimation,
        .scale = 1.75,
        .shininess = this->m_shininess};

    ObjectProperties gimbalProps{
        .baseColor = QVector3D{0.5, 1.0, 0.5},
        .rotation = gimbalAnimation,
        .scale = 2.25,
        .shininess = this->m_shininess};

    ObjectProperties outerGimbalProps{
        .baseColor = QVector3D{1.0, 0.5, 0.5},
        .rotation = outerGimbalAnimation,
        .scale = 2.75,
        .shininess = this->m_shininess};

    ObjectProperties sphereProps{
        .baseColor = QVector3D{1.0, 1.0, 1.0},
        .rotation = sphereAnimation,
        .scale = 0.215,
        .shininess = this->m_shininess};

    //this->m_fbo->bind();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_fboHandle);

    // Render in clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear Depth Buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_UNIFORM_BUFFER, this->m_uboLights); //Bind Buffer
    // Calculate something
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->m_uboLights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(this->m_lightSources), this->m_lightSources); // Übergebe LS an den Shader

    this->m_skybox->render(projection, view);

    for (unsigned i = 0; i < NUM_LS; i += 1)
    {
        ObjectProperties lightProps = this->m_lightProps.at(i);
        lightProps.scale = 0.2;

        QMatrix4x4 lightAnimation;
        lightAnimation.rotate((int)(this->m_timer * 2) % 360, QVector3D{0, 1, 1});
        lightAnimation.translate(lightProps.position);

        lightProps.rotation = lightAnimation;

        this->m_lightSources[i].ka = this->m_ambient;
        this->m_lightSources[i].kd = this->m_diffuse;
        this->m_lightSources[i].ks = this->m_specular;

        // TODO überarbeiten code wird mehrfach benutzt
        QMatrix4x4 model;
        model.scale(QVector3D{lightProps.scale, lightProps.scale, lightProps.scale});
        model = model * lightProps.rotation;
        QMatrix4x4 mvp = scene.projection * scene.view * model;
        this->m_lightSources[i].position = mvp * lightProps.position;
        //
        this->m_light->drawElements(lightProps, mat, scene);
    }

    this->m_gimbal->drawElements(outerGimbalProps, mat, scene);
    this->m_gimbal->drawElements(gimbalProps, mat, scene);
    this->m_gimbal->drawElements(innerGimbalProps, mat, scene);
    this->m_sphere->drawElements(sphereProps, mat, scene);

    if (this->m_gaussFilter) {
        this->m_gaussComputeShader->bind();
        std::vector<double> gauss = generate1DGauss(this->m_gaussStrength);
        for(int i = 0; i < (int)gauss.size(); i += 1) {
            this->m_gaussComputeShader->setUniformValue(i, (float) gauss[i]);
        }
        for(int i = gauss.size(); i < 15; i += 1) {
            this->m_gaussComputeShader->setUniformValue(i, 1.0f);
        }
        this->m_gaussComputeShader->setUniformValue("uNumber", this->m_gaussStrength);

        glGenTextures(1, &this->m_tex);
        glBindTexture(GL_TEXTURE_2D, this->m_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

        this->m_gaussComputeShader->setUniformValue("uHorizontal", true);
        glCopyImageSubData(this->m_colorTex, GL_TEXTURE_2D, 0, 0, 0, 0, this->m_tex, GL_TEXTURE_2D, 0, 0, 0, 0, width(), height(), 1);
        glBindImageTexture(0, this->m_colorTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
        glBindImageTexture(1, this->m_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
        glDispatchCompute(width(), height(), 1);

        // Caclulate Vertical Gauss with Horizontal Gauss Image
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        this->m_gaussComputeShader->setUniformValue("uHorizontal", false);
        glCopyImageSubData(this->m_colorTex, GL_TEXTURE_2D, 0, 0, 0, 0, this->m_tex, GL_TEXTURE_2D, 0, 0, 0, 0, width(), height(), 1);
        glDispatchCompute(width(), height(), 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glDeleteTextures(1, &this->m_tex);
    }

    if (this->m_colorFilter) {
        this->m_colorComputeShader->bind();
        glBindImageTexture(0, this->m_colorTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
        glDispatchCompute(width(), height(), 1);
    }

    if (this->m_depthData) {
        //this->m_fbo->bindDefault();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
        this->m_rect->drawElements(this->m_depthTex);
    } else {
        //QRect rect{0, 0, this->width(), this->height()};
        //QOpenGLFramebufferObject::blitFramebuffer(0, rect, this->m_fbo, rect);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_fboHandle);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
        glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    // Read the internal OpenGL Debug Log
    const QList<QOpenGLDebugMessage> messages = this->logger->loggedMessages();
    /*for (const QOpenGLDebugMessage &message : messages)
        qDebug() << message;
*/
    update();
}

//####################################################
// Slots
//####################################################

void MyGLWidget::setFOV(int value)
{
    this->m_FOV = value;
    emit fovChanged(value);
}

void MyGLWidget::setAngle(int angle)
{
    this->m_angle = angle;
    emit angleChanged(angle);
}

void MyGLWidget::setPerspective(bool toggle)
{
    this->m_perspective = toggle;
    emit perspectiveChanged(toggle);
}

void MyGLWidget::setOrthogonal(bool toggle)
{
    this->m_orthogonal = toggle;
    emit orthogonalChanged(toggle);
}

void MyGLWidget::setNear(double value)
{
    double diff = this->m_far - value;
    if (diff >= 2.0)
    {
        //qInfo() << "near" << value << "diff" << diff;
        this->m_near = value;
        emit nearChanged(value);
    }
    else
        emit nearOVFL(this->m_near);
}

void MyGLWidget::setFar(double value)
{
    double diff = value - this->m_near;
    if (diff >= 2.0)
    {
        //qInfo() << "far" << value << "diff" << diff;
        this->m_far = value;
        emit farChanged(value);
    }
    else
        emit farOVFL(this->m_far);
}

void MyGLWidget::setRotationA(int value)
{
    this->m_rotationA = value;
    //this->m_transparency = (float)value/100;
    //qInfo() << this->m_transparency;
    emit rotationAChanged(value);
}

void MyGLWidget::setRotationB(int value)
{
    this->m_rotationB = value;
    //this->m_uvCoordinatesAdd = (float)value/100;
    emit rotationBChanged(value);
}

void MyGLWidget::setRotationC(int value)
{
    this->m_rotationC = value;
    emit rotationCChanged(value);
}

void MyGLWidget::setAnimate(bool value)
{
    this->m_animate = value;
    emit animateChanged(value);
}

void MyGLWidget::setCamera(bool value)
{
    this->m_camera = value;
    emit cameraChanged(value);
}

void MyGLWidget::setDiffuse(double value)
{
    this->m_diffuse = value;
    emit diffuseChanged(value);
}

void MyGLWidget::setAmbient(double value)
{
    this->m_ambient = value;
    emit ambientChanged(value);
}

void MyGLWidget::setSpecular(double value)
{
    this->m_specular = value;
    emit specularChanged(value);
}

void MyGLWidget::setShininess(float value)
{
    this->m_shininess = value;
    emit shininessChanged(value);
}

void MyGLWidget::setDepthData(bool value)
{
    this->m_depthData = value;
    emit depthDataChanged(value);
}

void MyGLWidget::setGaussFilter(bool value)
{
    this->m_gaussFilter = value;
    emit gaussFilterChanged(value);
}

void MyGLWidget::setGaussStrength(int value)
{
    this->m_gaussStrength = value;
    emit gaussStrengthChanged(value);
}

void MyGLWidget::setColorFilter(bool value)
{
    this->m_colorFilter = value;
    emit colorFilterChanged(value);
}
