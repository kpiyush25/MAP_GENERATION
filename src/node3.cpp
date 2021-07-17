#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>  //for publishing and subscribing to the images in ROS
#include <opencv2/highgui/highgui.hpp>        //for OpenCV's GUI modules
#include <opencv2/imgproc/imgproc.hpp>        // for OpenCV's image processing modules
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>  // includes the ROS image message type
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
        // Now Subscribing to the input video feed and publishing the output video feed
        image_sub_ = it_.subscribe("/iris/camera_red_iris/image_raw", 1, &ImageConverter::imageCb, this);
        image_pub_ = it_.advertise("/image_converter/output_video", 1);

        cv::namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter() {
        cv::destroyWindow(OPENCV_WINDOW);  // destructor(destroys the display window on shutdown)
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            // sensor_msgs::image_encodings::BGR8 is simply a constant for bgr8.
            // OpenCV expects color images to use BGR channel order
        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        /*
            try statement allows to define a block of code to be tested for errors while it is being executed.
            catch statement allows to define a block of code to be executed if an error occurs in the try block.
            Here we are using try and catch just to catch some conversion errors because those functions don't
            check for the validity of our data.
        */

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