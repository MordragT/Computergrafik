#include "myglwidget.h"
#include <QtDebug>
#include <QKeyEvent>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::setFOV(int value) {
    this->m_FOV = value;
    emit fovChanged(value);
}

void MyGLWidget::setAngle(int angle) {
    this->m_Angle = angle;
    emit angleChanged(angle);
}

void MyGLWidget::setPerspective(bool toggle) {
    this->m_Perspective = toggle;
    emit perspectiveChanged(toggle);
}

void MyGLWidget::setOrthogonal(bool toggle) {
    this->m_Orthogonal = toggle;
    emit orthogonalChanged(toggle);
}

void MyGLWidget::setNear(double value) {
    double diff = this->m_Far - value;
    if(diff >= 2.0) {
        qInfo() << "near" << value << "diff" << diff;
        this->m_Near = value;
        emit nearChanged(value);
    } else emit nearOVFL(this->m_Near);
}

void MyGLWidget::setFar(double value) {
    double diff = value - this->m_Near;
    if(diff >= 2.0) {
        qInfo() << "far" << value << "diff" << diff;
        this->m_Far = value;
        emit farChanged(value);
    } else emit farOVFL(this->m_Far);
}

void MyGLWidget::setRotationA(int value) {
    this->m_RotationA = value;
    emit rotationAChanged(value);
}

void MyGLWidget::setRotationB(int value) {
    this->m_RotationB = value;
    emit rotationBChanged(value);
}

void MyGLWidget::setRotationC(int value) {
    this->m_RotationC = value;
    emit rotationCChanged(value);
}

void MyGLWidget::keyPressEvent( QKeyEvent * event )
{
    switch(event->key()) {
    case Qt::Key_W:
        this->m_CameraPos.setZ(this->m_CameraPos.z() + 0.2);
        break;
    case Qt::Key_A:
        this->m_CameraPos.setX(this->m_CameraPos.x() - 0.2);
        break;
    case Qt::Key_S:
        this->m_CameraPos.setZ(this->m_CameraPos.z() - 0.2);
        break;
    case Qt::Key_D:
        this->m_CameraPos.setX(this->m_CameraPos.x() + 0.2);
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
        break;
    }
    qInfo() << m_CameraPos;
}

