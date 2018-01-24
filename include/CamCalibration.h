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
    void start(std::string filePath, bool calibrate = false); // Call load
    cv::Mat getIntrinsicParametersMatrix() {return cameraMatrix;}
    cv::Mat getDistortionCoefficientsMatrix() {return distCoeffs;}

private :
    cv::Mat cameraMatrix;       // Intrinsic parameters
    cv::Mat distCoeffs;         // Distortions coefficients
    cv::Mat rotationMatrix;     // Matrix of rotation
    cv::Mat translationMatrix;  // Matrix of translation


    void calibrate(); // Calibrate camera et write parameter
    void load(std::string filePath = "out_camera_data.xml"); // Load calibration parameters from a file
};


#endif //AR_CAMCALIBRATION_H
