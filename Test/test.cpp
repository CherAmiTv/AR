#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>

using namespace cv;

int main( int argc, char* argv[])
{
    VideoCapture cam(0);
    Mat view;
    Size2i s = {7,4};
    std::vector<Point2f> pointImage;
    std::vector<Point3f> pointMire;

    Mat camera_matrix;
    Mat dist_coeffs;

    Mat rotation_vector;
    Mat translation_vector;

    bool flag = false;
    for(;;){
        cam >> view;
        flag = findChessboardCorners( view, s, pointImage, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
        if(flag)
            cv::solvePnP(pointMire, pointImage, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

        char key = (char)waitKey(50);

        if( key  == 27 )
            break;
        imshow(" ", view);
    }



    cam.release();
    return 0;
}
