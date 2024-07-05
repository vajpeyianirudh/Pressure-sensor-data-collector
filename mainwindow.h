#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QChart>
#include <QChartView>
#include <QScatterSeries>
#include <QFile>
#include <QDir>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/flann.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/reg/map.hpp>
#include <opencv2/aruco.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "library/include/CombinedApi.h"
#include "library/include/PortHandleInfo.h"
#include "library/include/ToolData.h"
#include "qcustomplot.h"
#include "ColonoscopeSensor.h"
#include "eigen-3.4.0/Eigen/Dense"

#define UPDATE_PERIOD    50 // milliseconds
#define FRAME_WIDTH      800
#define FRAME_HEIGHT     600
#define FRAME_WIDTH_COL  640
#define FRAME_HEIGHT_COL 480
#define FTDI_SERIALS  {"TACTUSB1", "TACTUSB2"} // FTDI serial codes for the USB interfaces

static CombinedApi capi = CombinedApi();

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace cv;
using namespace std;
using namespace Eigen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRecordClicked();
    void updateData();

private:
    void onErrorPrintDebugMessage(string methodName, int errorCode);
    bool connectNDI();
    void makeSplineQCP(QVector<QVector<double>> scopePosition);
    bool loadCameraIntrinsics(string name, Mat& cameraIntrinsics, Mat& distanceCoefficients);
    QColor gradient(double value, double low, double high);

    Ui::MainWindow *ui;
    QTimer* timer;
    bool record = false;

    VideoCapture capWebCam, capColCam;
    VideoWriter videoWebCam, videoColCam;
    Mat frameWebCam, frameColCam;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();
    QCPCurve *curveScope;
    QCPGraph *EMtrackerPoints;
    vector<ToolData> enabledTools;
    vector<QCPItemText*> EMtextLabels;
    QString EMFileName, webCamFileName, colCamFileName, pressureFileName, arucoFileName;

    // Camera parameters
    Mat cameraIntrinsicMatrix;
    Mat distanceCoefficients;
    const string cameraIntrinsicsFile = "IntrinsicMatrixOpenCV.txt";
    const float arucoSquareDim = 0.01499f;
    vector<Vec3d> rotVec, transVec;
    vector<int> markerIds;

    // Pressure sensor
    ColonoscopeSensor *pressureSensor;
    int currentReadings[CS_NUM_ELEMENTS] = {};
    QProgressBar *pressureBars[CS_NUM_ELEMENTS] = {};
};
#endif // MAINWINDOW_H
