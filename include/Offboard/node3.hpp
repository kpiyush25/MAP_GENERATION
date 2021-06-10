#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <string>

class ImageConverter {
    ros::NodeHandle nh_;

  public:
    ImageConverter()
        : it_(nh_) {
    }
    ~ImageConverter();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);

  private:
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
};