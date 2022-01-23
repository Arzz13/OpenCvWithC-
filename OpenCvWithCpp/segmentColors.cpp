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
#include "readAndBasicFunctions.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//These 3 are the predefined colors that will be detected by the application

                            //hmin,smin,vmin,hmax,smax,vmax
vector<vector<int>> definedColors{ {124,48,117,143,170,255}, // HSV min, max values for Purple color                                                             found by using trackbar
    
                            {68,72,156,102,126,255} , // HSV min, max values for Green color found                              by using trackbar
    
                                {0,62,0,35,255,255}};// HSV min, max values for Orange color found                      by using trackbar


//Contours: Contours can be explained simply as a curve joining all the continuous points (along the boundary), having same color or intensity.
//In opencv,Finding contours is like finding white object from black background. So remember, object to be found should be white and background should be black.So,we use canny image or hsv.

Point getPointsToDraw(Mat &image) {
 
    //    The function retrieves contours from the binary image . The contours are a useful tool for shape analysis and object detection and recognition.
    
    vector<vector<Point>> contours;  //To store the points of contours of the object
    vector<Vec4i> hierarchy;
 
    //    We find all the points of a contour and store in contours vector.4th and 5th are the names of algos involved
    //CHAIN_APPROX_SIMPLE-> compresses horizontal, vertical, and diagonal segments and leaves only their end points. For example, an up-right rectangular contour is encoded with 4 points.
    //RETR_EXTERNAL-> retrieves only the extreme outer contours.
    //After finding points, we can draw the contour.
    findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    //Defining conpoly container which holds the approximated contour points
    vector<vector<Point>> conPoly(contours.size()); //The number of approximated contours is same as the number of original contours.So, construct it with contours.size()
    
    //Defining boundRect container which holds the approximated contour points
    vector<Rect> boundRect(contours.size());//The number of bounding rectangles for contours is same as the number of original contours.So, construct it with contours.size()
 
    
    Point myPoint(0, 0); //Defining initial points of circle to be drawn on contour.(0,0) -> Then starts from top left.
 
    for (int i = 0; i < contours.size(); i++)
    {
        //Finding areas of contour for filtration
        int area = contourArea(contours[i]);
        
 
        if (area > 1000)   //If area is less than 1000, then probably that contour is noise
        {
            
            //Finding perimter (Boundary(arcLenght) of each contour)
            float peri = arcLength(contours[i], true);
            
            //Find number of corner points or (curves)
            //Approximation of the points of contours(getting only corner points)
            //The function cv::approxPolyDP approximates a curve or a polygon with another curve/polygon with less vertices(points) so that the distance between them is less or equal to the specified precision.
            //3rd Parameter specifies the approximation accuracy. This is the maximum distance between the original curve and its approximation.
            //4th parameter, if true, the approximated curve is closed
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true); //If contour a triangle, then only 3 points.If rect, then 4 pts , instead of default number of points which is higher.
 
            
            //Bounding rectangle around contour (Calculates the up-right bounding rectangle)
            boundRect[i] = boundingRect(conPoly[i]);     //Drawing rectangle around contour.
            
            //Defining x and y axis points from where the circle should start appearing.
            //To draw from centre, we divide width by 2.
            //We will draw circle over the object,so we need to define circle's center and its radius.
            //To get center,we need to divide the rectangle's width(in x axis) by 2.
            //Remember when we used to draw circle with compass, we keep one point at center and other at top.Same needed here.Center and top point.
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;
 
        }
    }
    return myPoint;
}

//This function is used to find the colour of object in front of camera and find the point where the object is.

vector<vector<int>>findColor(Mat& img){
    
    Mat img_HSV;
    //Converting image from rgb model to hsv model.
    cvtColor(img, img_HSV, COLOR_BGR2HSV);  //img_HSV contains image in hsv format.
    
    //Using a loop we check if the detected object's color(hsv params) is same as that of any of predefined colors.
    for (int i = 0; i < definedColors.size(); i++)
        
        {
            
            Scalar lower(definedColors[i][0], definedColors[i][1], definedColors[i][2]);   //Defining lower hsv
            Scalar upper(definedColors[i][3], definedColors[i][4], definedColors[i][5]);   //Defining upper hsv
            Mat mask;
            
            
            // Checks if array elements lie between the elements of two other arrays.
            // inRange() function returns an array of elements equal to 255 if the elements of the given array lie between the two arrays
            // inRange() function returns an array of elements equal to 0 if the elements of the given array do not lie between the two arrays
            //Syntax:
            //void cv::inRange(cv::InputArray src,cv::InputArray upperb,cv::InputArray lowerb,cv::OutputArray dst);
            inRange(img_HSV, lower, upper, mask);
            
            //If mask is full of zeros, then its not ith color.
            if(countNonZero(mask)<=0){
                continue;
            }
            
            //Now we have the masked image of detected object's color.
            //Now, we need to find contour points(circle's center and top) of these masks to draw on canvas
            Point myPoint = getPointsToDraw(mask);
            
            if (myPoint.x != 0 && myPoint.y!=0) {  //If no contour detected, then don't add.
                newPoints.push_back({ myPoint.x,myPoint.y,i }); //Push the points of boundRect(center of circle and top) into  newPoints.'i' is for color.
            }
        }
        return newPoints;
    }
     

void drawOnCanvas(vector<vector<int>> &newPoints, vector<Scalar> &paintBrushColor,Mat &img)
    {
     
        for (int i = 0; i < newPoints.size(); i++)
        {
            //To draw a circle, you need its center coordinates(2nd param) and radius.
            //This function draws a circle with particular color(4th param) and radius
            circle(img, Point(newPoints[i][0],newPoints[i][1]), 10, paintBrushColor[newPoints[i][2]], FILLED);
        }
    }


