#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

auto main() -> int {
    
    VideoCapture cap(0);

    Mat frame;
    while (true) {
        cap >> frame;

        imshow("window", frame);
        int key = waitKey(2);
        if (key == 27) {
            break;
        }
    }

    return 0;
}