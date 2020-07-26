#include <fstream>
#include <iostream>
// Include header files from OpenCV directory
// required to stitch images.
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

// Define mode for stitching as panoroma
// (One out of many functions of Stitcher)
Stitcher::Mode mode = Stitcher::SCANS;

// Array for pictures
vector<Mat> imgs;

int main(int argc, char* argv[]) {
    int i;
    for (i = 0; i < 1000; ++i) {
        // Read the ith argument or image
        // and push into the image array
        std::string gray_image = "/home/piyush/images/Gray_Image";
        std::string num_images = std::to_string(i);
        gray_image.append(num_images);
        gray_image.append(".jpg");
        Mat img = imread(gray_image);
        if (img.empty()) {
            // Exit if image is not present
            cout << "Can't read image "
                 << "\n";
            // return -1;
            break;
        }
        imgs.push_back(img);
        cout << imgs.size() << "\n";
    }
    cout << i << endl;
    // Define object to store the stitched image
    Mat stitched_img;

    // Create a Stitcher class object with mode scans.
    Ptr<Stitcher> stitcher = Stitcher::create(mode, false);

    // Command to stitch all the images present in the image array
    Stitcher::Status status = stitcher->stitch(imgs, stitched_img);

    if (status != Stitcher::OK) {
        // Check if images could not be stiched
        // status is OK if images are stiched successfully
        cout << "Can't stitch images\n";
        return -1;
    }

    // Store a new image stiched from the given
    // set of images as "result.jpg"
    imwrite("result.jpg", stitched_img);

    // Show the result
    imshow("Result", stitched_img);

    waitKey(0);
    return 0;
}
