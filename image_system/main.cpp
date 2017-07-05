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

    /*constexpr char* slides[] = {
        "res/slide1.PNG",
        "res/slide2.PNG",
        "res/slide3.PNG",
        "res/slide4.PNG"
    };*/
    //for 4:3
    constexpr char* slides[] = {
        "res/s1.PNG",
        "res/s2.PNG",
        "res/s3.PNG",
        "res/s4.PNG"
    };

    bool isBreakable = false;
    int pos = 0;

    // --- 2nd window ‚Éo‚·Žž‚¾‚¯moveWindow‚ð—LŒø‚É
    namedWindow("back", WINDOW_NORMAL);
    //moveWindow("back", 1920, 0);
    setWindowProperty("back", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

    FrameAnalyzer fa;
    ScreenBroker sb(slides[0]);

    Mat frame;
    while (true) {
        if (isBreakable) {
            cap >> frame;

            const double skinRate = fa.analyze(frame);
            sb.update_vector(skinRate);
        } else {
            sb.update_vector(0);
        }
        
        int key = waitKey(2);
        if (key == 27) {
            break;
        } else if ((key == 13 || key =='a') && pos < 3) {
            sb = ScreenBroker(slides[++pos]);
        } else if (key == 'b' && pos > 0) {
            sb = ScreenBroker(slides[--pos]);
        } else if (key == 'c') {
            isBreakable = !isBreakable;
            cout << "isBreakble: " << isBreakable << endl;
        }
    }

    return 0;
}