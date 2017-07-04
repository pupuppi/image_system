#include<iostream>
#include<opencv2\opencv.hpp>
#include<vector>

#include"frame_analyzer.h"
#include"speed_checker.h"


using namespace std;
using namespace cv;

auto main() -> int {
    
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        exit(1);
    }

    FrameAnalyzer fa;
    //ScreenBroker sb("back.png");

    Mat frame;

    while (true) {
        cap >> frame;

        const double skinRate = fa.analyze(frame);
        //sb.update_vector(skinRate);

        if (skinRate == -1) break;
    }

    return 0;
}