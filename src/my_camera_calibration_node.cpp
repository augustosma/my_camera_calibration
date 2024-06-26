#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<my_camera_calibration/my_camera_calibration.h>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
	my_camera_calibration::CamCalib cam_calib_;

	public:
	ImageConverter() : it_(nh_) ,cam_calib_()
	{
		// Subscrive to input video feed and publish output video feed
		image_sub_ = it_.subscribe("/cam0/image_raw", 1,
				&ImageConverter::imageCb, this);
		image_pub_ = it_.advertise("/image_converter/output_video", 1);

		cv::namedWindow(OPENCV_WINDOW);
	}

	~ImageConverter()
	{
		cv::destroyWindow(OPENCV_WINDOW);
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		cv_bridge::CvImagePtr cv_ptr;
		//cv::Mat out;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}

		//out.create( cv_ptr->image.size(), cv_ptr->image.type() );

		cam_calib_.sendImage(cv_ptr->image);//,out);

		//if last image of stream, calibrate
		ROS_INFO("%d", msg->header.seq);
		//*****        
		if(msg->header.seq > 550)
		{
			image_sub_.shutdown();
			//cv::Mat intrinsic_matrix, distortion_coeffs;
			cv::Mat intrinsic_matrix, distortion_coeffs;
			cam_calib_.calibrate(intrinsic_matrix, distortion_coeffs);

			std::cout << " *** DONE!\n\nReprojection error is " //<< err
				<< "\nStoring Intrinsics.xml and Distortions.xml files\n\n";

			std::cout << "intrinsic_matrix" << intrinsic_matrix << "\n"
				<<"distortion_coeffs" << distortion_coeffs << "\n";


			cv::FileStorage fs("cam_calib.yml", cv::FileStorage::WRITE);
			/* ESTRUTURA DA MENSAGEM
			   
			   std_msgs/Header header
			   uint32 height
			   uint32 width
			   string distortion_model
			   float64[] D
			   float64[9] K
			   float64[9] R
			   float64[12] P
			   uint32 binning_x
			   uint32 binning_y
			   sensor_msgs/RegionOfInterest roi
			   */
			
			fs << "image_width" << cam_calib_.image_size.width;
			fs << "image_height" << cam_calib_.image_size.height;
			fs << "camera_matrix" << intrinsic_matrix; 
			fs << "distortion_coefficients" << distortion_coeffs;
			fs << "rectification_matrix" << "" ;
			fs << "projection_matrix" << "";
			fs << "distortion_model" << "";		
			fs.release();

		}

		/*
		// Draw an example circle on the video stream
		if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
		cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
		*/
		// Update GUI Window
		cv::imshow(OPENCV_WINDOW, cv_ptr->image);//out);
		cv::waitKey(3);

		// Output modified video stream
		image_pub_.publish(cv_ptr->toImageMsg());
	}
};

int main(int argc, char** argv)
{
	ros::init(argc, argv, "my_camera_calibration_node");
	ImageConverter ic;
	ros::spin();
	return 0;
}
