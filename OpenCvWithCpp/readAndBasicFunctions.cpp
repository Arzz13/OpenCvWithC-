#include<string>
#include<iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "readAndBasicFunctions.hpp"

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


///////////////////////////////////////////  Video //////////////////////////////////
Mat videoRead(){
    char path[]="path";
VideoCapture cap(path); //VideoCapture object should be constructed by passing the path.Then the VideoCapture object should be read frame by frame. Finally those frames should be displayed in a window sequentially.(Video is sequence of images right?)

Mat img;

while (true) {

    cap.read(img);
    return img;
    //imshow("Image", img);//Reading each frame from video capture object and storing in img Mat file
    //waitKey(20);  //Each frame waits for 20ms.
}
}

void webCam(){

    //This line will initialize the VideoCapture object with the default camera(Index=0)
VideoCapture cap(0);//Give the index of your camera/webcam to the constructor of the VideoCapture object
        
    Mat img;

        while (true) {
            cap.read(img);
            
            imshow("Image", img);
            waitKey(1);
        }
}


