#include "openglwidget.h"
#include <math.h>
#include "mainwindow.h"

#define PI 3.14159265

OpenGLWidget::OpenGLWidget(QWidget *parent):QOpenGLWidget(parent)
{
    this->R = 10;
    this->ang = 0.0;
    this->values_rcvd = false;
    openGLTimer = new QTimer(this);
    openGLTimer->setInterval(50);
    connect(openGLTimer, SIGNAL(timeout()), this, SLOT(update()));
    openGLTimer->start();
}

OpenGLWidget::~OpenGLWidget()
{
//    gluDeleteQuadric(drawObj);
    openGLTimer->stop();
}

void OpenGLWidget::initializeGL()
{
    glClearColor(0.2, 0.2, 0.5, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if(values_rcvd){
        num_of_objects = XYZEMvalues[0].size();
//        cout << XYZEMvalues[0][0] << ", " << XYZEMvalues[1][0] << ", " << XYZEMvalues[2][0] << ", " << XYZEMvalues[3][0] << endl;
//        cout << pressureReadings[0] << ", " << pressureReadings[1] << ", " << pressureReadings[2] << ", " << pressureReadings[3] << endl;
        int j = 0;
        GLUquadric** drawObj = new GLUquadric*[num_of_objects];
        int spheres_per_press = 11;
        for(int i=0; i<num_of_objects; i++){

            glLoadIdentity();
            gluLookAt(50,-240,250, 50,-240,0, 0,1,0);

            // Draw sphere
            drawObj[i] = gluNewQuadric();
            glTranslatef(XYZEMvalues[0][i], XYZEMvalues[1][i], XYZEMvalues[2][i]);
            if (i>(59+spheres_per_press*j) && i<=(59+spheres_per_press*(j+1)) && j==0){
                float press = (16384.0 - 0.7* pressureReadings[j] - 0.3*pressureReadings[j+1])/16384.0;
                if (press < 0) press = 0;
                if (press > 1) press = 1;
                if (press < 0.5)
                    glColor3f(2*press,1.0,0);
                else
                    glColor3f(1.0,2.0 - (press*2),0);
                if (i==59+spheres_per_press*(j+1)) j += 1;
            }
            else if (i>(59+spheres_per_press*j) && i<=(59+spheres_per_press*(j+1))){
                float press = (16384.0 - (0.2* pressureReadings[j-1] + 0.6* pressureReadings[j] + 0.2*pressureReadings[j+1]))/16384.0;
                if (press < 0) press = 0;
                if (press > 1) press = 1;
                if (press < 0.5)
                    glColor3f(2*press,1.0,0);
                else
                    glColor3f(1.0,2.0 - (press*2),0);
                if (i==59+spheres_per_press*(j+1)) j += 1;
            }
            else
                glColor3f(0,1.0,0);
            gluQuadricDrawStyle(drawObj[i], GLU_FILL);
            gluSphere(drawObj[i], 10, 12, 12);
            gluDeleteQuadric(drawObj[i]);

            // Draw cylinders
//            double d = sqrt(pow(XYZEMvalues[0][i+1] - XYZEMvalues[0][i], 2) + \
//                           pow(XYZEMvalues[1][i+1] - XYZEMvalues[1][i], 2) + \
//                           pow(XYZEMvalues[2][i+1] - XYZEMvalues[2][i], 2));
//            drawObj[i] = gluNewQuadric();
//            glTranslatef(XYZEMvalues[0][i], XYZEMvalues[1][i], XYZEMvalues[2][i]);
//            glRotatef(atan((XYZEMvalues[1][i+1] - XYZEMvalues[1][i])/(XYZEMvalues[2][i+1] - XYZEMvalues[2][i])), 1, 0, 0);
//            glRotatef(atan((XYZEMvalues[0][i+1] - XYZEMvalues[0][i])/(XYZEMvalues[2][i+1] - XYZEMvalues[2][i])), 0, 1, 0);
//            glColor3f(0.2,0.5,0);
//            gluQuadricDrawStyle(drawObj[i], GLU_FILL);
//            gluCylinder(drawObj[i], 10, 10, d, 10, 10);
//            gluDeleteQuadric(drawObj[i]);
        }
        delete [] drawObj;
    }
    else{
//        GLUquadric* drawObj;
//        drawObj = gluNewQuadric();
        }
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/h, 0.01, 600.0);

    update();
}

void OpenGLWidget::recvdValues()
{
    values_rcvd = true;
    cout << "values received" << endl;
}
