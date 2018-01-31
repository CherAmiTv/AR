//
// Created by julien on 10/01/18.
//

#include <window.h>
#include "CamCalibration.h"

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;

static void help()
{
    cout <<  "This is a camera calibration sample." << endl
         <<  "Usage: calibration configurationFile"  << endl
         <<  "Near the sample file you'll find the configuration file, which has detailed help of "
                 "how to edit it.  It may be any OpenCV supported file format XML/YAML." << endl;
}
class Settings
{
public:
    Settings() : goodInput(false) {}
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};

    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "BoardSize_Width"  << boardSize.width
           << "BoardSize_Height" << boardSize.height
           << "Square_Size"         << squareSize
           << "Calibrate_Pattern" << patternToUse
           << "Calibrate_NrOfFrameToUse" << nrFrames
           << "Calibrate_FixAspectRatio" << aspectRatio
           << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
           << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

           << "Write_DetectedFeaturePoints" << bwritePoints
           << "Write_extrinsicParameters"   << bwriteExtrinsics
           << "Write_outputFileName"  << outputFileName

           << "Show_UndistortedImage" << showUndistorsed

           << "Input_FlipAroundHorizontalAxis" << flipVertical
           << "Input_Delay" << delay
           << "Input" << input
           << "}";
    }
    void read(const FileNode& node)                          //Read serialization for this class
    {
        node["BoardSize_Width" ] >> boardSize.width;
        node["BoardSize_Height"] >> boardSize.height;
        node["Calibrate_Pattern"] >> patternToUse;
        node["Square_Size"]  >> squareSize;
        node["Calibrate_NrOfFrameToUse"] >> nrFrames;
        node["Calibrate_FixAspectRatio"] >> aspectRatio;
        node["Write_DetectedFeaturePoints"] >> bwritePoints;
        node["Write_extrinsicParameters"] >> bwriteExtrinsics;
        node["Write_outputFileName"] >> outputFileName;
        node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
        node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
        node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
        node["Show_UndistortedImage"] >> showUndistorsed;
        node["Input"] >> input;
        node["Input_Delay"] >> delay;
        interprate();
    }
    void interprate()
    {
        goodInput = true;
        if (boardSize.width <= 0 || boardSize.height <= 0)
        {
            cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
            goodInput = false;
        }
        if (squareSize <= 10e-6)
        {
//            squareSize
            cerr << "Invalid square size " << squareSize << endl;
//            goodInput = false;
        }
        if (nrFrames <= 0)
        {
            cerr << "Invalid number of frames " << nrFrames << endl;
            goodInput = false;
        }

        if (input.empty())      // Check for valid input
            inputType = INVALID;
        else
        {
            cameraID = streamCamera;
            inputCapture.open(cameraID);
        }
        if (inputType == INVALID)
        {
            cerr << " Inexistent input: " << input;
            goodInput = false;
        }

        flag = 0;
        if(calibFixPrincipalPoint) flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
        if(calibZeroTangentDist)   flag |= CV_CALIB_ZERO_TANGENT_DIST;
        if(aspectRatio)            flag |= CV_CALIB_FIX_ASPECT_RATIO;


        calibrationPattern = NOT_EXISTING;
        if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
        if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
        if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
        if (calibrationPattern == NOT_EXISTING)
        {
            cerr << " Inexistent camera calibration mode: " << patternToUse << endl;
            goodInput = false;
        }
        atImageList = 0;

    }
    Mat nextImage()
    {
        Mat result;
        if( inputCapture.isOpened() )
        {
            Mat view0;
            inputCapture >> view0;
            view0.copyTo(result);
        }
        else if( atImageList < (int)imageList.size() )
            result = imread(imageList[atImageList++], CV_LOAD_IMAGE_COLOR);

        return result;
    }

    static bool readStringList( const string& filename, vector<string>& l )
    {
        l.clear();
        FileStorage fs(filename, FileStorage::READ);
        if( !fs.isOpened() )
            return false;
        FileNode n = fs.getFirstTopLevelNode();
        if( n.type() != FileNode::SEQ )
            return false;
        FileNodeIterator it = n.begin(), it_end = n.end();
        for( ; it != it_end; ++it )
            l.push_back((string)*it);
        return true;
    }

    static bool isListOfImages( const string& filename)
    {
        string s(filename);
        // Look for file extension
        if( s.find(".xml") == string::npos && s.find(".yaml") == string::npos && s.find(".yml") == string::npos )
            return false;
        else
            return true;
    }
public:
    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->



    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;

private:
    string patternToUse;


};

static void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

bool CamCalibration::load(std::string filePath) {
    FileStorage fs(filePath, FileStorage::READ); // Read the settings

    if(!fs.isOpened())
        return false;
    fs["Distortion_Coefficients"] >> distCoeffs;
    fs["Camera_Matrix"]  >> cameraMatrix;
    return true;
}

void CamCalibration::computeFrustum() {
    invCameraMatrix = Mat::ones(3, 3, CV_64F);
    invert(cameraMatrix, invCameraMatrix);
    Mat xmin, xmax, ymin, ymax;
    Mat a, b, c, d;

    xmin = Mat::ones(3, 1, CV_64F);
    xmax = Mat::ones(3, 1, CV_64F);
    ymin = Mat::ones(3, 1, CV_64F);
    ymax = Mat::ones(3, 1, CV_64F);

    a = Mat::ones(3, 1, CV_64F);
    b = Mat::ones(3, 1, CV_64F);
    c = Mat::ones(3, 1, CV_64F);
    d = Mat::ones(3, 1, CV_64F);

    xmin.at<double>(0) = 0;
    xmin.at<double>(1) = (window_height() / 2);

    xmax.at<double>(0) = window_width();
    xmax.at<double>(1) = (window_height() / 2);

    ymin.at<double>(0) = window_width() / 2;
    ymin.at<double>(1) = 0;

    ymax.at<double>(0) = window_width() / 2;
    ymax.at<double>(1) = window_height();

    a = invCameraMatrix * xmin;
    b = invCameraMatrix * ymax;
    c = invCameraMatrix * xmax;
    d = invCameraMatrix * ymin;

    double near = 0.1;
    double far = 1000.0;

    a = a * near; //left
    b = b * near; //top
    c = c * near; //right
    d = d * near; //bot

    double distX = norm(c - a);
    double distY = norm(b - d);

    double left = -distX/2;
    double right = distX/2;
    double top = distY/2;
    double bot = -distX/2;

    frustum.m[0][0] = (2*near)/ distX;
    frustum.m[0][2] = (right+left)/(distX);

    frustum.m[1][1] = (2*near) / distY;
    frustum.m[1][2] = (top+bot)/(distY);

    frustum.m[2][2] = -((far+near)/(far-near));
    frustum.m[2][3] = -((2*(far*near))/(far-near));

    frustum.m[3][2] = -1;

    Vec3f eye = {0.f, 0.f, 0.f};
    Vec3f center = {0.f, 0.f, 10.f};
    Vec3f up = {0.f, -1.f, 0.f};

    view = lookat(eye, center, up);
    view = Transpose(view);
}

void CamCalibration::start(std::string filePath) {

    if(!load(filePath))
        cout << "run ./calibrage first !" << endl;

    computeFrustum();

    VideoCapture cam(streamCamera);
    Mat view;
    Size2i s = {7,4};
    std::vector<Point2f> pointImage;
    std::vector<Point3f> pointMire = initPoint3D(7, 4, 35.f);

    Mat rotMatrix;
    bool flag;
    bool first = false;
    for(;;) {
        cam >> view;
        // chessboard
        flag = findChessboardCorners(view, s, pointImage, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK);
        if (flag) {
            drawChessboardCorners(view, s, Mat(pointImage), flag);

            solvePnP(pointMire, pointImage, cameraMatrix, distCoeffs, rvec, tvec, first, CV_EPNP);
            Rodrigues(rvec, rotMatrix);

            getEulerAngle(rotMatrix, euler);
            transform = tvec;

            computeTransform(rotMatrix, tvec);
        }

        // magic wand detection
        findMagicWand(view);

        char key = (char)waitKey(50);

        if( key  == 27 )
            break;
        imshow(" ", view);
    }


    cam.release();
}

void CamCalibration::getEulerAngle(Mat &rotCamerMatrix,Vec3d &eulerAngles){

    Mat cameraMatrix,rotMatrix,transVect,rotMatrixX,rotMatrixY,rotMatrixZ;
    double* _r = rotCamerMatrix.ptr<double>();
    double projMatrix[12] = {_r[0],_r[1],_r[2],0,
                             _r[3],_r[4],_r[5],0,
                             _r[6],_r[7],_r[8],0};

    decomposeProjectionMatrix( Mat(3,4,CV_64FC1,projMatrix),
                               cameraMatrix,
                               rotMatrix,
                               transVect,
                               rotMatrixX,
                               rotMatrixY,
                               rotMatrixZ,
                               eulerAngles);
}

std::vector<Point3f> CamCalibration::initPoint3D(int x, int y, float squareSize) {

    std::vector<cv::Point3f> ret;

    for(int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            Point3f tmp;

            tmp.x = j*squareSize;
            tmp.y = i*squareSize;
            tmp.z = 0.f;

            ret.push_back(tmp);
        }
    }

    return ret;
}

void CamCalibration::computeTransform(cv::Mat rodri, cv::Mat translation) {

//    for(int i = 0; i < 3; ++i)
//        for(int j = 0; j < 3; ++j)
//            transformation.m[i][j] = rodri.at<double>(j ,i);
//
//    for(int i = 0; i < 3; ++i)
//        transformation.m[i][3] = translation.at<double>(i);
    float x = translation.at<double>(0);
    float y = translation.at<double>(1) + 35.f;
    float z = translation.at<double>(2);

    transformation = Translation(x, y, z) * RotationX(euler[0]) * RotationY(euler[1]) * RotationZ(euler[2]);

}

Transform CamCalibration::lookat(const Vec3f eye, const Vec3f center, const Vec3f up) {

    Transform Matrix;
    Transform M1, M2;

    Vec3f X, Y, Z;

    normalize(eye - center, Z);
    normalize(up.cross(Z), X);
    Y = Z.cross(X);

    M1.m[0][0] = X[0];
    M1.m[1][0] = X[1];
    M1.m[2][0] = X[2];
    M1.m[3][0] = 0;

    M1.m[0][1] = Y[0];
    M1.m[1][1] = Y[1];
    M1.m[2][1] = Y[2];
    M1.m[3][1] = 0;

    M1.m[0][2] = Z[0];
    M1.m[1][2] = Z[1];
    M1.m[2][2] = Z[2];
    M1.m[3][2] = 0;

    M1.m[0][3] = 0;
    M1.m[1][3] = 0;
    M1.m[2][3] = 0;
    M1.m[3][3] = 1;

    M2.m[0][0] = 1;
    M2.m[1][0] = 0;
    M2.m[2][0] = 0;
    M2.m[3][0] = eye[0];

    M2.m[0][1] = 0;
    M2.m[1][1] = 1;
    M2.m[2][1] = 0;
    M2.m[3][1] = eye[1];

    M2.m[0][2] = 0;
    M2.m[1][2] = 0;
    M2.m[2][2] = 1;
    M2.m[3][2] = eye[2];

    M2.m[0][3] = 0;
    M2.m[1][3] = 0;
    M2.m[2][3] = 0;
    M2.m[3][3] = 1;

    Matrix = M1 * M2;

    return Matrix;
}

bool CamCalibration::findMagicWand(Mat& view) {
    std::vector< std::vector< cv::Point > > contours;
    std::vector<cv::Vec4i> hierarchy;

    // create a mask to filter the red color
    cv::Mat hsv_foreground, mask_color, low_mask_color, high_mask_color;
    cv::cvtColor(view, hsv_foreground, cv::COLOR_BGR2HSV);

    /*
    // red color mask in HSV
    // first mask : low red in hsv
    cv::Scalar low_color_hsv = cv::Scalar(160,200,70,0);
    cv::Scalar middle_low_color_hsv = cv::Scalar(180,255,255,0);
    // second mask : upper red in hsv
    cv::Scalar middle_high_color_hsv = cv::Scalar(0,200,70,0);
    cv::Scalar high_color_hsv = cv::Scalar(20,255,255,0);
    */

    // yellow color
    cv::Scalar low_color_hsv = cv::Scalar(20, 100, 100);
    cv::Scalar high_color_hsv = cv::Scalar(30, 255, 255);

    inRange(hsv_foreground, low_color_hsv, high_color_hsv, mask_color);

    /*
    // keep (white) the pixels which are between the 2 hsv values
    inRange(hsv_foreground, low_color_hsv, middle_low_color_hsv, low_mask_color); // first mask : lower
    inRange(hsv_foreground, middle_high_color_hsv, high_color_hsv, high_mask_color); // second mask : upper
    bitwise_or(low_mask_color, high_mask_color, mask_color); // merge of the two masks
    */

    imshow("mask", mask_color);

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(3, 3,CV_32F), cv::Point(-1,-1), 1, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(9,9,CV_32F), cv::Point(-1,-1), 2, 1, 1);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // search the center of the red torso
    if(contours.size() > 0) {
        cv::Moments mu = moments(contours[0]);
        cv::Point center(mu.m10/mu.m00 , mu.m01/mu.m00);
        cv::rectangle(view, cv::Point(center.x-5, center.y-5), cv::Point(center.x+5, center.y+5), cv::Scalar(0,255,0), 1, 8, 0);
    }
}

