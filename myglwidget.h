#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QObject>
#include <QKeyEvent>
#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <QVector>

#include "skybox.h"
#include "model.h"
#include "common.h"

const unsigned NUM_LS = 5;

class MyGLWidget: public QOpenGLWidget, private QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
    void keyPressEvent( QKeyEvent * event ) override;
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

private:
    Skybox* m_skybox;
    Model* m_sphere;
    Model* m_innerGimbal;
    Model* m_gimbal;
    Model* m_outerGimbal;
    LightSource m_lightSources[NUM_LS];
    Model* m_lightModels[NUM_LS];
    QVector<ObjectProperties> m_lightProps;
    unsigned m_uboLights;
    double m_timer = 0;
    QOpenGLDebugLogger* logger;
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
    QVector3D m_cameraPos{0,0,0};
};

#endif // MYGLWIDGET_H
