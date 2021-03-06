#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include <QObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dsbFar->setValue(2.0);
    QObject::connect(
                // Sender
                ui->spFOV,
                // Signal
                qOverload<int>(&QSpinBox::valueChanged),
                // Empfänger
                ui->openGLWidget,
                // Ausführung
                [=](int value) {
        ui->openGLWidget->setFOV(value);
        //qInfo() << "FOV: " << value;
    });

    QObject::connect(
                ui->spAngle,
                qOverload<int>(&QSpinBox::valueChanged),
                ui->openGLWidget,
                [=](int value) {
        ui->openGLWidget->setAngle(value);
        ui->openGLWidget->setShininess((float)value * 100 / 360);
    });

    QObject::connect(
                ui->dsbFar,
                qOverload<double>(&QDoubleSpinBox::valueChanged),
                ui->openGLWidget,
                [=](double value) {
        ui->openGLWidget->setFar(value);
        //qInfo() << "Far: " << value;
    });

    QObject::connect(
                ui->openGLWidget,
                &MyGLWidget::farChanged,
                ui->dsbFar,
                [=](double value) {
        ui->dsbFar->setValue(value);
    });

    QObject::connect(
                ui->dsbNear,
                qOverload<double>(&QDoubleSpinBox::valueChanged),
                ui->openGLWidget,
                [=](double value) {
        ui->openGLWidget->setNear(value);
        //qInfo() << "Near: " << value;
    });

    QObject::connect(
                ui->openGLWidget,
                &MyGLWidget::nearChanged,
                ui->dsbNear,
                [=](double value) {
        ui->dsbNear->setValue(value);
    });


    QObject::connect(
                ui->openGLWidget,
                &MyGLWidget::farOVFL,
                ui->openGLWidget,
                [=](double value) {
        ui->dsbFar->setValue(value);
    });

    QObject::connect(
                ui->openGLWidget,
                &MyGLWidget::nearOVFL,
                ui->openGLWidget,
                [=](double value) {
        ui->dsbNear->setValue(value);
    });

    QObject::connect(
                ui->rbPerspective,
                &QRadioButton::toggled,
                ui->openGLWidget,
                [=](bool value) {
        ui->openGLWidget->setPerspective(value);
        //qInfo() << "Perspective: " << value;
    });

    QObject::connect(
                ui->rbOrthogonal,
                &QRadioButton::toggled,
                ui->openGLWidget,
                [=](bool value) {
        ui->openGLWidget->setOrthogonal(value);
        //qInfo() << "Orthogonal: " << value;
    });

    QObject::connect(
                ui->hsRotationA,
                &QSlider::valueChanged,
                ui->openGLWidget,
                [=](int value) {
        ui->openGLWidget->setRotationA(value);
        ui->openGLWidget->setAmbient((double)value / 360);

    });

    QObject::connect(
                ui->hsRotationB,
                &QSlider::valueChanged,
                ui->openGLWidget,
                [=](int value) {
        ui->openGLWidget->setRotationB(value);
        ui->openGLWidget->setDiffuse((double)value / 360);
    });

    QObject::connect(
                ui->hsRotationC,
                &QSlider::valueChanged,
                ui->openGLWidget,
                [=](int value) {
        ui->openGLWidget->setRotationC(value);
        ui->openGLWidget->setSpecular((double)value / 360);
    });

    QObject::connect(
                ui->rbReset,
                &QPushButton::clicked,
                this,
                [=]() {
        this->reset();
    });

    QObject::connect(
                ui->cbAnimate,
                &QCheckBox::clicked,
                this,
                [=](bool value) {
        ui->openGLWidget->setAnimate(value);
    });

    QObject::connect(
                ui->cbCamera,
                &QCheckBox::clicked,
                this,
                [=](bool value) {
        ui->openGLWidget->setCamera(value);
    });

    QObject::connect(
                ui->cbDepthData,
                &QCheckBox::clicked,
                this,
                [=](bool value) {
        ui->openGLWidget->setDepthData(value);
    });

    QObject::connect(
                ui->cbGaussFilter,
                &QCheckBox::clicked,
                this,
                [=](bool value) {
        ui->openGLWidget->setGaussFilter(value);
    });

    QObject::connect(
                ui->hsGauss,
                &QSlider::valueChanged,
                ui->openGLWidget,
                [=](int value) {
        if (value % 2 == 0) {
            value += 1;
        }
        ui->openGLWidget->setGaussStrength(value);
        //qInfo() << value;
    });

    QObject::connect(
                ui->cbColorFilter,
                &QCheckBox::clicked,
                this,
                [=](bool value) {
        ui->openGLWidget->setColorFilter(value);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset() {
    ui->spFOV->setValue(45);
    //ui->vsFOV->setValue(0);
    ui->spAngle->setValue(0);
    //ui->vsAngle->setValue(0);
    ui->rbOrthogonal->setChecked(false);
    ui->rbPerspective->setChecked(false);
    ui->hsRotationA->setValue(0);
    ui->hsRotationB->setValue(0);
    ui->hsRotationC->setValue(0);
    ui->dsbNear->setValue(0);
    ui->dsbFar->setValue(2);
    ui->cbAnimate->setChecked(false);
    ui->cbCamera->setChecked(false);
}
