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

    constexpr char* slides[] = {
        "res/slide1.PNG",
        "res/slide2.PNG",
        "res/slide3.PNG",
        "res/slide4.PNG"
    };
    int state = 0;


    FrameAnalyzer fa;
    ScreenBroker sb(slides[0]);

    Mat frame;
    while (true) {
        if (state != 0) {
            cap >> frame;


            const double skinRate = fa.analyze(frame);
            sb.update_vector(skinRate);
        } else {
            sb.update_vector(0);
        }
        
        int key = waitKey(2);
        if (key == 27) {
            break;
        } else if ((key == 13 || key =='a') && state < 3) {
            ++state;
            sb = ScreenBroker(slides[state]);
        } else if (key == 'b' && state > 0) {
            --state;
            sb = ScreenBroker(slides[state]);
        }
    }

    return 0;
}