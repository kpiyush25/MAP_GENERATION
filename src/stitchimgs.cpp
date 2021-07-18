#include "opencv2/highgui.hpp"    // for OpenCV's GUI modules
#include "opencv2/imgcodecs.hpp"  // provides methods to read and write images
#include "opencv2/stitching.hpp"  // header that provides the feature of image stitching
#include <fstream>                // including this allows us to create files, write information to existing files
                                  // and read information from the files
#include <iostream>

using namespace std;
using namespace cv;

Stitcher::Mode mode = Stitcher::SCANS;

vector<Mat> images;  // Vector for storing the pictures
int main(int argc, char* argv[]) {
    int i;
    // Now storing all the images taken by the drone into a vector in order to use the required OpenCV functions
    for (i = 0; i < 1000; ++i) {
        std::string gray_image = "/home/piyush/images/Gray_Image";
        std::string count = std::to_string(i);
        gray_image.append(count);
        gray_image.append(".jpg");
        Mat img = imread(gray_image);
        if (img.empty()) {
            cout << "Not able to read images anymore :(\n";
            break;
        }
        images.push_back(img);
        cout << images.size() << "\n";
    }
    cout << i << endl;
    Mat stitched_img;  // this object will store the stitched image

    Ptr<Stitcher> stitcher = Stitcher::create(mode, false);  // Creating a Stitcher class object with mode scans.

    // this will stitch all the images present in the image array
    Stitcher::Status status = stitcher->stitch(images, stitched_img);

    if (status != Stitcher::OK) {
        // if the images are not stitched successfully then the status will not be OK
        cout << "Can't stitch images\n";
        return -1;
    }

    // Now storing the stitched image as result.jpg
    imwrite("result.jpg", stitched_img);

    imshow("Result", stitched_img);  // displaying the stitched image

    waitKey(0);  // when the parameter is 0 here then it shows the image window infinitely until we press any key
    return 0;
}
