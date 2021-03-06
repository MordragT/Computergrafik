#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QObject>
#include <QKeyEvent>
#include <QVector3D>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <QVector>
#include <QOpenGLFramebufferObject>

#include "skybox.h"
#include "model.h"
#include "common.h"

const unsigned NUM_LS = 5;

class MyGLWidget : public QOpenGLWidget, private QOpenGLFunctions_4_3_Core
{

    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
    void keyPressEvent(QKeyEvent *event) override;
    //Initialisierung: Sets up OpenGL ressources and state
    void initializeGL() override;
    //Render: Renders the OpenGL Scene
    void paintGL() override;
    //Reinitialisierung: Setups OpenGL viewport, gets called when widget is resized
    void resizeGL(int w, int h) override;

public slots:
    void setFOV(int value);
    void setAngle(int angle);
    void setPerspective(bool toggle);
    void setOrthogonal(bool toggle);
    void setNear(double value);
    void setFar(double value);
    void setRotationA(int value);
    void setRotationB(int value);
    void setRotationC(int value);
    void setAnimate(bool value);
    void setCamera(bool value);
    void setAmbient(double value);
    void setDiffuse(double value);
    void setSpecular(double value);
    void setShininess(float value);
    void setDepthData(bool value);
    void setGaussFilter(bool value);
    void setGaussStrength(int value);
    void setColorFilter(bool value);

signals:
    void shininessChanged(float value);
    void ambientChanged(double value);
    void diffuseChanged(double value);
    void specularChanged(double value);
    void animateChanged(bool value);
    void cameraChanged(bool value);
    void fovChanged(int value);
    void angleChanged(int value);
    void perspectiveChanged(bool value);
    void orthogonalChanged(bool value);
    void nearChanged(double value);
    void farChanged(double value);
    void rotationAChanged(int value);
    void rotationBChanged(int value);
    void rotationCChanged(int value);
    void farOVFL(double value);
    void nearOVFL(double value);
    void depthDataChanged(bool value);
    void gaussFilterChanged(bool value);
    void gaussStrengthChanged(int value);
    void colorFilterChanged(bool value);

private:
    QOpenGLShaderProgram *m_colorComputeShader;
    QOpenGLShaderProgram *m_gaussComputeShader;
    Model *m_rect;
    GLuint m_tex = 0;
    GLuint m_colorTex = 0;
    GLuint m_depthTex = 0;
    QOpenGLFramebufferObject *m_fbo;
    GLuint m_fboHandle = 0;
    Skybox *m_skybox;
    Model *m_sphere;
    Model *m_gimbal;
    LightSource m_lightSources[NUM_LS];
    Model *m_light;
    QVector<ObjectProperties> m_lightProps;
    unsigned m_uboLights;
    double m_timer = 0;
    QOpenGLDebugLogger *logger;
    int m_FOV = 0;
    int m_angle = 0;
    float m_shininess = 0;
    bool m_perspective = false;
    bool m_orthogonal = false;
    double m_near = 0;
    double m_far = 0;
    int m_rotationA = 0;
    int m_rotationB = 0;
    int m_rotationC = 0;
    double m_ambient = 0.0;
    double m_diffuse = 0.0;
    double m_specular = 0.0;
    bool m_animate = false;
    bool m_camera = false;
    bool m_gaussFilter = false;
    bool m_colorFilter = false;
    int m_gaussStrength = 3;
    QVector3D m_cameraPos{0, 0, 0};
    float m_viewAngleVertical = 0.0;
    float m_viewAngleHorizontal = 0.0;
    bool m_depthData = false;
};

#endif // MYGLWIDGET_H
