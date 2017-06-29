#include<iostream>
#include<opencv2\opencv.hpp>

#include"frame_analyzer.h"

using namespace std;
using namespace cv;

auto main() -> int {
    
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        exit(1);
    }

    FrameAnalyzer fa;

    Mat frame;
    while (true) {
        cap >> frame;

        auto result = fa.analyze(frame);
        cout << result.skinRate << endl;

        imshow("window", frame);
        int key = waitKey(2);
        if (key == 27) {
            break;
        }
    }

    return 0;
}