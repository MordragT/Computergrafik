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

signals:
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
    int m_verticesSize = 4;
    GLuint m_indices[6] = { 0, 1, 2, 1, 2, 3 };
    int m_indicesSize = 6;
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ibo{QOpenGLBuffer::IndexBuffer};
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram* m_prog;
    float m_transparency = 1.0f;
    float m_uvCoordinatesAdd = 0.0f;
    QOpenGLTexture* m_texture;
    int m_FOV;
    int m_angle;
    bool m_perspective;
    bool m_orthogonal;
    double m_near;
    double m_far;
    int m_rotationA;
    int m_rotationB;
    int m_rotationC;
    QVector3D m_cameraPos{0,0,0};
};

#endif // MYGLWIDGET_H
