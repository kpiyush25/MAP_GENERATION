#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <string>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter {
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

  public:
    ImageConverter()
        : it_(nh_) {
        // Subscribe to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/iris/camera_red_iris/image_raw", 1, &ImageConverter::imageCb, this);
        image_pub_ = it_.advertise("/image_converter/output_video", 1);

        cv::namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter() {
        cv::destroyWindow(OPENCV_WINDOW);
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        // Draw an example circle on the video stream
        // if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
        // cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

        std::string gray_image = "/home/piyush/images/Gray_Image";
        static int count = 0;
        std::string count2 = std::to_string(count);
        // count2= to_string(count);
        gray_image.append(count2);
        gray_image.append(".jpg");
        count++;
        imwrite(gray_image, cv_ptr->image);
        // above is the code for saving the image.

        // Update GUI Window
        cv::imshow(OPENCV_WINDOW, cv_ptr->image);
        cv::waitKey(3);

        // Output modified video stream
        image_pub_.publish(cv_ptr->toImageMsg());
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "image_converter");
    ImageConverter ic;

    /*while (ros::ok()){
      ros::spinOnce();
    }*/
    // it is equivalent to ros::spin();

    ros::Rate r(0.5);
    while (ros::ok()) {
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}