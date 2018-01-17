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
        cv::Mat getIntrinsicParameters();
        cv::Mat getDistortionCoefficients() {return m_distCoeffs;}
        cv::Mat getGlobalToCameraTransformation() {return m_cameraMatrix;}

    private :
        std::vector<cv::Mat> v_m_rvecs;
        std::vector<cv::Mat> v_m_tvecs;
        cv::Mat m_distCoeffs;
        cv::Mat m_cameraMatrix;

    };


#endif //AR_CAMCALIBRATION_H
