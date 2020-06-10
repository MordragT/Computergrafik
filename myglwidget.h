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

#include "model.h"

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

signals:
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
    /*
    struct Vertex {
        GLfloat position[3];
        GLfloat color[3];
        GLfloat texCoordinates[2];
    };
    Vertex m_vertices[4] = {
        Vertex {
            .position = {-0.5, 0.5, 0.0},
            .color = {0.5, 1.0, 0.0},
            .texCoordinates = {0, 1}
        },
        Vertex {
            .position = {0.5, 0.5, 0.0},
            .color = {1.0, 0.5, 0.0},
            .texCoordinates = {1, 1}
        },
        Vertex {
            .position = {-0.5, -0.5, 0.0},
            .color = {0.0, 1.0, 0.5},
            .texCoordinates = {0, 0}
        },
        Vertex {
            .position = {0.5, -0.5, 0.0},
            .color = {1.0, 0.0, 1.0},
            .texCoordinates = {1, 0}
        }
    };

    GLushort m_indices[6] = { 0, 1, 2, 1, 2, 3 };
    QOpenGLBuffer* m_vbo;//{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer* m_ibo;//{QOpenGLBuffer::IndexBuffer};
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram* m_prog;
    QOpenGLShaderProgram* m_progColor;
    float m_transparency = 1.0f;
    float m_uvCoordinatesAdd = 0.0f;
    QOpenGLTexture* m_texture;
    */
    Model* m_sphere;
    Model* m_innerGimbal;
    Model* m_gimbal;
    Model* m_outerGimbal;
    double m_timer = 0;
    QOpenGLDebugLogger* logger;
    int m_FOV = 0;
    int m_angle = 0;
    bool m_perspective = false;
    bool m_orthogonal = false;
    double m_near = 0;
    double m_far = 0;
    int m_rotationA = 0;
    int m_rotationB = 0;
    int m_rotationC = 0;
    bool m_animate = false;
    bool m_camera = false;
    QVector3D m_cameraPos{0,0,0};
};

#endif // MYGLWIDGET_H
