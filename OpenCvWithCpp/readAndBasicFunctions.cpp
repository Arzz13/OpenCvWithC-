#include<string>
#include<iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "readAndBasicFunctions.hpp"
#include "segmentColors.hpp"

using namespace std;
using namespace cv; //All OpenCV classes and functions are in cv namespace



/*
 Waitkey:
    waitKey(0) function waits for a key press forever. When any key is pressed, this function returns the ASCII value of the key and your program will continue.
If positive value is passed in param,then it waits for a key press for only the time duration, specified by passed value in milliseconds
 */


////////////////////////  Images  //////////////////////

Mat imageRead(char p[]){   //Path where the image is found
    Mat img = imread(p); //Reading from the path and returning as matrix object which holds the image.
   // imshow("Image", img);  //Displaying the matrix in image format.
   // waitKey(0);
    return img;             //Returning to the caller.
}



vector<vector<int>> newPoints;  //These are the points where the object appears on the image(image from video) .So, we need to paint on these points only using drawCanvas function.
//This container also has the index of color.So, we know which color to draw.

vector<Scalar> paintBrushColor{ {255,0,255}, // When Purple detected,then use this value                                                       (purple's bgr) to draw on canvas
                                {0,255,0},     // When Green detected,then use this value(green's                   bgr) to draw on canvas
                                {255,0,0}   // When Blue detected,then use this value(blue's                 bgr) to draw on canvas
};    

void webCam(){

    //This line will initialize the VideoCapture object with the default camera(Index=0)
VideoCapture cap(0);//Give the index of your camera/webcam to the constructor of the VideoCapture object
        
    Mat img;

        while (true) {
            cap.read(img); //Read each image into the img object
            
            //Finds each object's position on the screen and its color
            newPoints=findColor(img); //Returns the center,top and color of circle to be drawn.
            drawOnCanvas(newPoints,paintBrushColor,img); //Painting on the place where the object is on the canvas(screen) with the color same as object.
            
            imshow("Image", img);
            waitKey(1);
        }
}


