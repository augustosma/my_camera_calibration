#include <my_camera_calibration/my_camera_calibration.h>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;

namespace my_camera_calibration
{
CamCalib::CamCalib() {
    /*
    //int n_boards = 0;           // will be set by input list
    float image_sf = 0.5f;      // image scaling factor
    float delay = 1.f;
    int board_w = 6;
    int board_h = 7;

    //board_w = atoi(argv[1]);
    //board_h = atoi(argv[2]);
    //n_boards = atoi(argv[3]);
    int board_n = board_w * board_h;
    cv::Size board_sz = cv::Size(board_w, board_h);

    // ALLOCATE STORAGE
    //
    vector<vector<cv::Point2f> > image_points;
    vector<vector<cv::Point3f> > object_points;

    // Capture corner views: loop until we've got n_boards successful
    // captures (all corners on the board are found).
    //
    */

}

CamCalib::~CamCalib() {}

void CamCalib::sendImage(cv::Mat& image)//0, cv::Mat& image)
{
    //cv::Size image_size;

    //cv::Mat image0, image;
    //cv::Mat image;
    //capture >> image0;
    //image_size = image0.size();
    image_size = image.size();
    //cv::resize(image0, image, cv::Size(), image_sf, image_sf, cv::INTER_LINEAR);

    // Find the board
    //
    vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(image, board_sz, corners);

    // Draw it
    //


    // If we got a good board, add it to our data
    //
    if (found) {
        drawChessboardCorners(image, board_sz, corners, found);

        //image ^= cv::Scalar::all(255);
        cv::Mat mcorners(corners);

        // do not copy the data
        mcorners *= (1.0 / image_sf);

        // scale the corner coordinates
        image_points.push_back(corners);
        object_points.push_back(vector<cv::Point3f>());
        vector<cv::Point3f> &opts = object_points.back();

        opts.resize(board_n);
        for (int j = 0; j < board_n; j++) {
            opts[j] = cv::Point3f(static_cast<float>(j / board_w),
                                  static_cast<float>(j % board_w), 0.0f);
        }
        // cout << "Collected our " << static_cast<uint>(image_points.size())
        //    << " of " << n_boards << " needed chessboard images\n" << endl;
    }
    //cv::imshow("Calibration", image);

    // show in color if we did collect the image
    /*if ((cv::waitKey(30) & 255) == 27)
            return -1;
*/

}//sendImage

void CamCalib::calibrate(cv::Mat& intrinsic_matrix,cv::Mat& distortion_coeffs){

    cout << "\n\n*** CALIBRATING THE CAMERA...\n" << endl;

    // CALIBRATE THE CAMERA!
    //
//    cv::Mat intrinsic_matrix, distortion_coeffs;
    
    double err = cv::calibrateCamera(
                object_points, 
                image_points, 
                image_size, 
                intrinsic_matrix,
                distortion_coeffs, 
                cv::noArray(), 
                cv::noArray(),
                cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_FIX_PRINCIPAL_POINT,
                cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001)
                //cv::TermCriteria(cv::TermCriteria::MAX_ITER, 2, 0.001)
                );

    // SAVE THE INTRINSICS AND DISTORTIONS
    cout << " *** DONE!\n\nReprojection error is " << err
         << "\nStoring Intrinsics.xml and Distortions.xml files\n\n";
    
    cout << "intrinsic_matrix" << intrinsic_matrix << "\n"
    	<<"distortion_coeffs" << distortion_coeffs << "\n";
    //cv::FileStorage fs("intrinsics.xml", cv::FileStorage::WRITE);
    //cout << fs.isOpened();
    //fs << "qualquer coisa teste";
    /*fs << "image_width" << image_size.width << "image_height" << image_size.height
       << "camera_matrix" ;/*<< intrinsic_matrix << "distortion_coefficients"
       << distortion_coeffs;*/
    //fs.release();
    /*
    // EXAMPLE OF LOADING THESE MATRICES BACK IN:
    fs.open("intrinsics.xml", cv::FileStorage::READ);
    cout << "\nimage width: " << static_cast<int>(fs["image_width"]);
    cout << "\nimage height: " << static_cast<int>(fs["image_height"]);
    cv::Mat intrinsic_matrix_loaded, distortion_coeffs_loaded;
    fs["camera_matrix"] >> intrinsic_matrix_loaded;
    fs["distortion_coefficients"] >> distortion_coeffs_loaded;
    cout << "\nintrinsic matrix:" << intrinsic_matrix_loaded;
    cout << "\ndistortion coefficients: " << distortion_coeffs_loaded << endl;

    // Build the undistort map which we will use for all
    // subsequent frames.
    //
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(intrinsic_matrix_loaded, distortion_coeffs_loaded,
                                cv::Mat(), intrinsic_matrix_loaded, image_size,
                                CV_16SC2, map1, map2);

    // Just run the camera to the screen, now showing the raw and
    // the undistorted image.
    //
    for (;;) {
        cv::Mat image, image0;
        capture >> image0;

        if (image0.empty()) {
            break;
        }
        cv::remap(image0, image, map1, map2, cv::INTER_LINEAR,
                  cv::BORDER_CONSTANT, cv::Scalar());
        cv::imshow("Undistorted", image);
        if ((cv::waitKey(30) & 255) == 27) {
            break;
        }
    }
*/

}//calibrate

}//namespace


