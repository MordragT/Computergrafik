#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QObject>
#include <QKeyEvent>
#include <QVector3D>


class MyGLWidget: public QOpenGLWidget {

    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    void keyPressEvent( QKeyEvent * event );

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
    int m_FOV;
    int m_Angle;
    bool m_Perspective;
    bool m_Orthogonal;
    double m_Near;
    double m_Far;
    int m_RotationA;
    int m_RotationB;
    int m_RotationC;
    QVector3D m_CameraPos{0,0,0};
};

#endif // MYGLWIDGET_H
