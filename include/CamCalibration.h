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


class CamCalibration {

public:
    void start();
    cv::Mat getIntrinsicParametersMatrix() {return cameraMatrix;}
    cv::Mat getDistortionCoefficientsMatrix() {return distCoeffs;}
    cv::Mat getRotationTransformMatrix(int i) {return rvecs.at(i);}
    cv::Mat getTranslationTransformMatrix(int i) {return tvecs.at(i);}

private :
    cv::Mat cameraMatrix;       // Intrinsic parameters
    cv::Mat distCoeffs;         // Distortions coefficients
    std::vector<cv::Mat> rvecs; // Rotations transform (transform the object point to the image point)
    std::vector<cv::Mat> tvecs; // Translation transform (transform the object point to the image point)
};


#endif //AR_CAMCALIBRATION_H
