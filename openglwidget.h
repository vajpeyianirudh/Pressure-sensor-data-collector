#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
//#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QTimer>
#include <ColonoscopeSensor.h>

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void recvdValues();

    float x =0, y =0, z =0, R, ang;
    int num_of_objects=0;
    int num_of_obj = 10;
    bool values_rcvd = false;
    QVector<QVector<double>> XYZEMvalues = {{0}, {0}, {0}, {0}};
    QVector<bool> sensAvailable = {0,0,0,0,0,0,0,0};

    QTimer* openGLTimer;

    GLUquadric* drawObj2;

    int pressureReadings[CS_NUM_ELEMENTS] = {};
};

#endif // OPENGLWIDGET_H
