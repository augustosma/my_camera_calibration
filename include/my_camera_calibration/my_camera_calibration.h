#ifndef MY_CAMERA_CALIBRATION_H
#define MY_CAMERA_CALIBRATION_H

#include <vector>
#include <math.h>
#include <Eigen/Dense>

#include <iostream>
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/features2d.hpp>
//#include "opencv2/xfeatures2d.hpp"

using std::vector;


namespace my_camera_calibration
{
class CamCalib
{
public:
    float image_sf = 0.5f;      // image scaling factor
    float delay = 1.f;
    int board_w = 6;
    int board_h = 7;

    //board_w = atoi(argv[1]);
    //board_h = atoi(argv[2]);
    //n_boards = atoi(argv[3]);
    int board_n = board_w * board_h;
    cv::Size board_sz = cv::Size(board_w, board_h);
    cv::Size image_size;
    // ALLOCATE STORAGE
    //
    vector<vector<cv::Point2f> > image_points;
    vector<vector<cv::Point3f> > object_points;

    CamCalib();
    ~CamCalib();

    void sendImage(cv::Mat&);//,cv::Mat&);

	//cv::Mat intrinsic_matrix, distortion_coeffs;
    void calibrate(cv::Mat&, cv::Mat&);

private:

};
}
#endif
