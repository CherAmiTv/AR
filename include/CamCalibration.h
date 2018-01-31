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


const std::string calibrationFileName = "out_camera_data.xml";

class CamCalibration {

public:
    void start(std::string filePath = calibrationFileName, bool needCalibration = false); // Call load
    cv::Mat getIntrinsicParametersMatrix() {return cameraMatrix;}
    cv::Mat getDistortionCoefficientsMatrix() {return distCoeffs;}

    cv::Vec3d getRot()const{return rot;}
    cv::Mat gettVec()const{return transform;}
    Transform getProjection()const{ return frustum;}
    Transform getTransform()const{return transformation;};

private :
    cv::Mat invCameraMatrix;
    cv::Mat cameraMatrix;       // Intrinsic parameters
    cv::Mat distCoeffs;         // Distortions coefficients
    Transform frustum;
    Transform transformation;
    cv::Mat rvec;
    cv::Mat tvec;

    cv::Vec3d rot;
    cv::Mat transform;

    std::vector<cv::Point3f> initPoint3D(int x, int y, float squareSize);
    void calibrate(); // Calibrate camera et write parameter
    void load(std::string filePath = calibrationFileName); // Load calibration parameters from a file
    void getEulerAngle(cv::Mat &rotCamerMatrix,cv::Vec3d &eulerAngles);
    void computeFrustum();
    void computeTransform(cv::Mat rodri, cv::Mat translation);
    bool findMagicWand(cv::Mat& view);

};


#endif //AR_CAMCALIBRATION_H
