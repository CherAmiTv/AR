//
// Created by julien on 10/01/18.
//

#ifndef AR_CAMCALIBRATION_H
#define AR_CAMCALIBRATION_H

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <mat.h>
#include "vec.h"
#include <glcore.h>
#include <color.h>

static const float SQUARESIZE = 31.6;
static const int STREAMCAMERA = 0; // 0 : default camera, 1 or 2 : other camera

class CamCalibration {
public:
    void start(std::string filePath = "out_camera_data.xml"); // Call load

    cv::Vec3d getRot()const{return euler;}
    cv::Mat gettVec()const{return transform;}
    Transform getProjection()const{ return frustum;}
    Transform getTransform()const{return transformation;};
    Transform getView()const{return view;};
    Point getMagicWand()const{return Point(magicWand.x, magicWand.y, 0.0);};

    cv::Mat& getMat() {return image;}
    bool getFlag()const {return flag;}

private :

    Transform view;
    cv::Mat invCameraMatrix;
    cv::Mat cameraMatrix;       // Intrinsic parameters
    cv::Mat distCoeffs;         // Distortions coefficients
    Transform frustum;
    Transform transformation;
    cv::Mat rvec;
    cv::Mat tvec;
    cv::Vec3d euler;
    cv::Mat transform;
    cv::VideoCapture cam;
    cv::Mat image;
    bool flag;

    cv::Point magicWand;

    Transform lookat(const cv::Vec3f eye, const cv::Vec3f center, const cv::Vec3f up);
    std::vector<cv::Point3f> initPoint3D(int x, int y, float squareSize);
    void calibrate(); // Calibrate camera et write parameter
    bool load(std::string filePath = "out_camera_data.xml"); // Load calibration parameters from a file
    void getEulerAngle(cv::Mat &rotCamerMatrix,cv::Vec3d &eulerAngles);
    void computeFrustum();
    void computeTransform(cv::Mat rodri, cv::Mat translation);
    bool findMagicWand(cv::Mat& view);

};


#endif //AR_CAMCALIBRATION_H
