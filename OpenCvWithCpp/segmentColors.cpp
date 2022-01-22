//
//  segmentColors.cpp
//  OpenCvWithCpp
//
//  Created by Arzz on 21/01/22.
/*
 The easiest way to detect and segment an object from an image is the color based methods . The object and the background should have a significant color difference in order to successfully  segment objects using color based methods.
The images will be in rgb format by default.But to distinguish the colors ,it is better for the image to be in hsv format (hue, saturation, vue)
    * hue -> Hue is the color portion of the model.(red,yellow,green,blue,cyan,magenta,.....)
    * saturation -> Saturation describes the amount of gray in a particular color
    * vue->Vue describes the brightness or intensity of the color
    By varying these 3 params in a image,we can determine the color,intensity,..
 */
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void findColor(char path[]){
    Mat img=imread(path),img_HSV;
    //Converting image from rgb model to hsv model.
    cvtColor(img, img_HSV, COLOR_BGR2HSV);  //img_HSV contains image in hsv format.
    
    Mat mask;
    
    //Defining the params of hsv model with initial values.
    int hmin=0,smin=0,vmin=0;
    int hmax=255,smax=255,vmax=255;
    
    //Creating trackbar.
    //It enables users to change various parameters while the OpenCV application is running.
    
    //Creating a window called Trackbar in which trackbar will be present
    namedWindow("Trackbar", (static_cast<void>(640), 200));
    createTrackbar("Hue Min TrackBar", "Trackbar", &hmin, 179);  //179 is the max value of trackbar.Minimum is always zero

    createTrackbar("Hue Max TrackBar", "Trackbar", &hmax, 179,0);
    createTrackbar("Sat Min TrackBar", "Trackbar", &smin, 255,0);
    createTrackbar("Sat Max TrackBar", "Trackbar", &smax, 255,0);
    createTrackbar("Val Min TrackBar", "Trackbar", &vmin, 255,0);
    createTrackbar("Val Max TrackBar", "Trackbar", &vmax, 255,0);
 
    while(true){
       


        //Scalar-> Template class for a 4-element vector derived from Vector.
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax,smax,vmax);
        
        // inRange function():
        
       // Checks if array elements lie between the elements of two other arrays.
     //Syntax:
      //void cv::inRange(cv::InputArray src,cv::InputArray upperb,cv::InputArray lowerb,cv::OutputArray dst);

        inRange(img_HSV, lower, upper, mask);
        
        imshow("Original Image",img);
        imshow("Image in HSV model",img_HSV);
        imshow("Image Mask", mask);
        waitKey(1);
    }
}


//Contours: Contours can be explained simply as a curve joining all the continuous points (along the boundary), having same color or intensity.
//In opencv,Finding contours is like finding white object from black background. So remember, object to be found should be white and background should be black.So,we use canny image


void detectContours(Mat imgDil,Mat img){
    
    //    The function retrieves contours from the binary image . The contours are a useful tool for shape analysis and object detection and recognition.
    
    vector<vector<Point>> contours;
    vector<Vec4i>hierarchy; //Vector of 4 integers
    
    
//    We find all the contour's points and store in contour vector
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //Function to draw contours of purple color and thicknees 2 to the img. -1 means draw all the contours
    
    for(int i=0;i<contours.size();i++){
        
        //This function gets the area of each contour
        int area=contourArea(contours[i]);
        
        //To draw the bounding box to contours
        float peri=arcLength(contours[i], true);
        
        drawContours(img, contours, -1, Scalar(255,0,255),2);

    }
}





//we identify the shape and position of an object using contours
//Using contours with OpenCV, you can get a sequence of points of vertices for each white patch
//(White patch-> polygon -> shape formed with straight lines)
void detectShape(char path[]){
    Mat img = imread(path);
    Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;
    //Converting to a greyscale image
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    //Gaussian blur/smoothing is the most commonly used smoothing technique to eliminate noises in images and videos. In this technique, an image should be convolved with a Gaussian kernel to produce the smoothed image. A kernel is a single-channel floating point matrix

    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0); //3 x 3 Gaussian filter
    
    //Canny Edge Detection is a popular edge detection algorithm.It gives a image with all its edges marked.
    Canny(imgBlur, imgCanny, 25, 75);
    /*
     Morphological operations:
        A set of operations that process images based on shapes. Morphological operations apply a structuring element to an input image and generate an output image.
     The most basic morphological operations are: Erosion and Dilation
     Useful in
     1)Removing noise
     2)Isolation of individual elements and joining disparate elements in an image.
     3)Finding of intensity bumps or holes in an image
     Dilation:This maximizing operation causes bright regions within an image to "grow" (therefore the name dilation).
     Erosion:Bright areas of the image get thinner, whereas the dark zones gets bigger.
     
     */
   // Returns a structuring element of the specified size and shape for morphological operations.
//  The function constructs and returns the structuring element that can be further passed to erode, dilate
// Format-> cv::getStructuringElement(int shape,Size ksize,Point anchor = Point(-1,-1))
  // Anchor-> It indicates the relative position of a filtered point within the kernel; the anchor should lie within the kernel;  Default anchor-(-1,-1)
    
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    
    dilate(imgCanny, imgDil, kernel);

    detectContours(imgDil,img);
     
        imshow("Image", img);
    waitKey(0);
}
