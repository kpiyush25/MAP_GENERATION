
// CPP program to Stitch 
// input images (panorama) using OpenCV  
#include <iostream> 
#include <fstream> 
//#include <filesystem>
  
// Include header files from OpenCV directory 
// required to stitch images. 
#include "opencv2/imgcodecs.hpp" 
#include "opencv2/highgui.hpp" 
#include "opencv2/stitching.hpp" 
  
using namespace std; 
using namespace cv; 
  
// Define mode for stitching as panoroma  
// (One out of many functions of Stitcher) 
Stitcher::Mode mode = Stitcher::SCANS; 
  
// Array for pictures 
vector<Mat> imgs; 
  
int main(int argc, char* argv[]) 
{ 
    // Get all the images that need to be  
    // stitched as arguments from command line
    //std::cout << std::filesystem::current_path;
    int i;
    for (i = 0; i < 1000; ++i) 
    { 
            // Read the ith argument or image  
            // and push into the image array 
             std::string gray_image = "/home/piyush/images/Gray_Image";
      //static int i=0;
      std::string i2= std::to_string(i);
      //count2= to_string(count);
      gray_image.append(i2);
      gray_image.append(".jpg"); 
      //i++;
            Mat img = imread(gray_image); 
            if (img.empty()) 
            { 
                // Exit if image is not present 
                //cout << "Can't read image '" <<  << "'\n"; 
                //return -1;
                break; 
            } 
            imgs.push_back(img); 
            cout << imgs.size() << "\n";
    } 
    cout << i << endl;
    // Define object to store the stitched image 
    Mat pano;

      
    // Create a Stitcher class object with mode panoroma 
    Ptr<Stitcher> stitcher = Stitcher::create(mode, false); 
      
    // Command to stitch all the images present in the image array 
    Stitcher::Status status = stitcher->stitch(imgs, pano); 
  
    if (status != Stitcher::OK) 
    { 
        // Check if images could not be stiched 
        // status is OK if images are stiched successfully 
        cout << "Can't stitch images\n"; 
        return -1; 
    } 
      
    // Store a new image stiched from the given  
    //set of images as "result.jpg" 
    imwrite("result.jpg", pano); 
      
    // Show the result 
    imshow("Result", pano); 
      
    waitKey(0); 
    return 0; 
} 
