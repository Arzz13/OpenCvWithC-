#include<string>
#include<iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "readAndBasicFunctions.hpp"
#include "segmentColors.hpp"
using namespace std;
using namespace cv;

int main() {
   char path[]="Resources/lambo.png";
    //webCam();
  detectShape(path);


}

